#include "databasemanager.h"

// 初始化静态成员
DatabaseManager* DatabaseManager::m_instance = nullptr;
QMutex DatabaseManager::m_instanceMutex;

// 单例获取（线程安全）
DatabaseManager* DatabaseManager::getInstance() {
    if (m_instance == nullptr) {
        QMutexLocker locker(&m_instanceMutex);
        if (m_instance == nullptr) {
            m_instance = new DatabaseManager();
        }
    }
    return m_instance;
}

// 构造函数：初始化连接池
DatabaseManager::DatabaseManager(QObject *parent)
    : QObject(parent), m_isInitSuccess(false) {
    initConnectionPool();
}

// 析构函数：释放所有连接
DatabaseManager::~DatabaseManager() {
    QMutexLocker locker(&m_poolMutex);
    m_connPool.clear(); // 智能指针自动释放连接
    qDebug() << "数据库管理类析构，释放所有连接";
}

// 初始化连接池（创建 1-3 个初始连接）
void DatabaseManager::initConnectionPool() {

    qDebug() << QSqlDatabase::drivers();
    QMutexLocker locker(&m_poolMutex);
    qDebug() << "初始化数据库连接池...";

    // 创建 1-3 个初始连接（避免启动时连接过多）
    for (int i = 0; i < 3; i++) {
        QSqlDatabase db = createNewConnection();
        if (db.isOpen()) {
            m_connPool.append(QSharedPointer<QSqlDatabase>(new QSqlDatabase(db)));
            qDebug() << "创建连接成功，当前连接数：" << m_connPool.size();
        }
    }

    m_isInitSuccess = !m_connPool.isEmpty();
    if (m_isInitSuccess) {
        qDebug() << "连接池初始化成功，初始连接数：" << m_connPool.size();
    } else {
        qCritical() << "连接池初始化失败！";
        emit errorOccurred("连接池初始化失败，程序可能无法正常运行");
    }
}

// 创建新连接（内部使用）
QSqlDatabase DatabaseManager::createNewConnection() {
    // 加载 MySQL 驱动
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", QString("conn_%1").arg(QDateTime::currentMSecsSinceEpoch()));
    if (!db.isValid()) {
        qCritical() << "加载 MySQL 驱动失败：" << db.lastError().text();
        return db;
    }

    // 配置连接参数（带自动重连）
    db.setHostName(DB_HOST);
    db.setDatabaseName(DB_NAME);
    db.setUserName(DB_USER);
    db.setPassword(DB_PWD);
    db.setPort(DB_PORT);
    db.setConnectOptions("MYSQL_OPT_RECONNECT=1|MYSQL_READ_DEFAULT_GROUP=client");

    // 打开连接
    if (db.open()) {
        qDebug() << "创建新数据库连接成功";
        return db;
    } else {
        qCritical() << "创建连接失败：" << db.lastError().text();
        return db;
    }
}

// 获取连接（从连接池取，无空闲则创建，最多 MAX_CONN_COUNT 个）
QSharedPointer<QSqlDatabase> DatabaseManager::getConnection() {
    QMutexLocker locker(&m_poolMutex);

    // 1. 检查连接池是否有空闲连接
    for (int i = 0; i < m_connPool.size(); i++) {
        QSharedPointer<QSqlDatabase> db = m_connPool.at(i);
        if (db->isOpen() && checkConnectionValid(*db)) {
            // 移除并返回空闲连接（避免重复使用）
            m_connPool.removeAt(i);
            qDebug() << "获取空闲连接，剩余连接数：" << m_connPool.size();
            return db;
        }
    }

    // 2. 连接池无空闲，创建新连接（不超过最大连接数）
    if (m_connPool.size() < MAX_CONN_COUNT) {
        QSqlDatabase newDb = createNewConnection();
        if (newDb.isOpen()) {
            qDebug() << "创建新连接，当前总连接数：" << m_connPool.size() + 1;
            return QSharedPointer<QSqlDatabase>(new QSqlDatabase(newDb));
        }
    }

    // 3. 连接池满且无可用连接，返回空
    qCritical() << "数据库连接池已满，无可用连接！";
    emit errorOccurred("数据库连接池已满，操作失败");
    return QSharedPointer<QSqlDatabase>();
}

// 释放连接（归还到连接池）
void DatabaseManager::releaseConnection(QSharedPointer<QSqlDatabase> db) {
    if (!db) return;

    QMutexLocker locker(&m_poolMutex);
    // 检查连接是否有效，无效则丢弃
    if (db->isOpen() && checkConnectionValid(*db)) {
        m_connPool.append(db);
        qDebug() << "连接归还成功，当前连接数：" << m_connPool.size();
    } else {
        qDebug() << "连接无效，已丢弃";
        db->close();
    }
}

// 检查连接有效性（避免断网后连接失效）
bool DatabaseManager::checkConnectionValid(QSqlDatabase& db) {
    QSqlQuery query("SELECT 1", db);
    return query.exec();
}

// 执行 SQL（写操作，支持事务）
bool DatabaseManager::executeSql(const QString& sql, const QVariantMap& params, bool isTransaction) {
    QElapsedTimer timer;
    timer.start();

    // 1. 获取连接
    QSharedPointer<QSqlDatabase> db = getConnection();
    if (!db) return false;

    QSqlQuery query(*db);
    bool success = false;

    try {
        // 2. 开启事务（如需）
        if (isTransaction && !beginTransaction(db)) {
            throw QString("事务开启失败：%1").arg(db->lastError().text());
        }

        // 3. 预处理 SQL（复用解析结果，提升性能）
        query.prepare(sql);
        // 绑定参数（避免 SQL 注入，提升性能）
        for (auto it = params.begin(); it != params.end(); ++it) {
            query.bindValue(it.key(), it.value());
        }

        // 4. 执行 SQL
        success = query.exec();
        if (!success) {
            throw QString("SQL 执行失败：%1，SQL：%2").arg(query.lastError().text()).arg(sql);
        }

        // 5. 提交事务（如需）
        if (isTransaction && !commitTransaction(db)) {
            throw QString("事务提交失败：%1").arg(db->lastError().text());
        }

        // 6. 日志记录（性能监控）
        logOperation(sql, timer.elapsed());
    } catch (const QString& error) {
        // 7. 异常处理：回滚事务
        if (isTransaction) {
            rollbackTransaction(db);
        }
        qCritical() << error;
        emit errorOccurred(error);
        success = false;
    }

    // 8. 释放连接
    releaseConnection(db);
    return success;
}

// 执行查询（读操作）
QList<QVariantMap> DatabaseManager::querySql(const QString& sql, const QVariantMap& params) {
    QElapsedTimer timer;
    timer.start();

    QList<QVariantMap> result;
    QSharedPointer<QSqlDatabase> db = getConnection();
    if (!db) return result;

    QSqlQuery query(*db);
    try {
        // 预处理 + 绑定参数
        query.prepare(sql);
        for (auto it = params.begin(); it != params.end(); ++it) {
            query.bindValue(it.key(), it.value());
        }

        // 执行查询
        if (!query.exec()) {
            throw QString("查询失败：%1，SQL：%2").arg(query.lastError().text()).arg(sql);
        }

        // 解析结果
        QSqlRecord record = query.record();
        while (query.next()) {
            QVariantMap row;
            for (int i = 0; i < record.count(); i++) {
                QString fieldName = record.fieldName(i);
                row[fieldName] = query.value(i);
            }
            result.append(row);
        }

        // 日志记录
        logOperation(sql, timer.elapsed());
        qDebug() << "查询成功，返回" << result.size() << "条数据，耗时" << timer.elapsed() << "ms";
    } catch (const QString& error) {
        qCritical() << error;
        emit errorOccurred(error);
        result.clear();
    }

    // 释放连接
    releaseConnection(db);
    return result;
}

// 批量执行 SQL（分块处理，控制内存）
bool DatabaseManager::batchExecute(const QString& sql, const QList<QVariantMap>& paramsList) {
    if (paramsList.isEmpty()) return true;

    QElapsedTimer timer;
    timer.start();

    // 分块处理：每 BATCH_SIZE 条为一批
    int totalCount = paramsList.size();
    int batchCount = (totalCount + BATCH_SIZE - 1) / BATCH_SIZE; // 向上取整

    for (int i = 0; i < batchCount; i++) {
        // 1. 获取当前批数据
        int startIdx = i * BATCH_SIZE;
        int endIdx = std::min((i + 1) * BATCH_SIZE, totalCount);
        QList<QVariantMap> batchParams = paramsList.mid(startIdx, endIdx - startIdx);

        // 2. 获取连接
        QSharedPointer<QSqlDatabase> db = getConnection();
        if (!db) return false;

        QSqlQuery query(*db);
        bool batchSuccess = false;

        try {
            // 3. 开启事务（批量操作原子性）
            if (!beginTransaction(db)) {
                throw QString("批量事务开启失败：%1").arg(db->lastError().text());
            }

            // 4. 预处理 SQL（复用解析结果）
            query.prepare(sql);

            // 5. 绑定批量参数
            for (const QVariantMap& params : batchParams) {
                query.addBindValue(params); // 批量绑定（MySQL 支持）
            }

            // 6. 执行批量操作（比逐条执行快 10-100 倍）
            batchSuccess = query.execBatch();
            if (!batchSuccess) {
                throw QString("批量执行失败：%1，批次：%2").arg(query.lastError().text()).arg(i + 1);
            }

            // 7. 提交事务
            if (!commitTransaction(db)) {
                throw QString("批量事务提交失败：%1").arg(db->lastError().text());
            }

            qDebug() << "批次" << (i + 1) << "执行成功，处理" << batchParams.size() << "条数据";
        } catch (const QString& error) {
            // 8. 回滚事务
            rollbackTransaction(db);
            qCritical() << error;
            emit errorOccurred(error);
            releaseConnection(db);
            return false;
        }

        // 9. 释放连接 + 释放当前批内存
        releaseConnection(db);
        batchParams.clear();
        qApp->processEvents(); // 触发垃圾回收，控制内存

        // 10. 降低 CPU 占用（每批延时 10ms）
        if (i % 5 == 0) {
            QThread::msleep(10);
        }
    }

    qDebug() << "批量操作完成，总处理" << totalCount << "条数据，总耗时" << timer.elapsed() << "ms";
    return true;
}

// 开启事务
bool DatabaseManager::beginTransaction(QSharedPointer<QSqlDatabase> db) {
    if (!db || !db->isOpen()) return false;
    return db->transaction();
}

// 提交事务
bool DatabaseManager::commitTransaction(QSharedPointer<QSqlDatabase> db) {
    if (!db || !db->isOpen()) return false;
    return db->commit();
}

// 回滚事务
bool DatabaseManager::rollbackTransaction(QSharedPointer<QSqlDatabase> db) {
    if (!db || !db->isOpen()) return false;
    bool ret = db->rollback();
    qDebug() << "事务已回滚";
    return ret;
}

// 检查整体连接状态
bool DatabaseManager::isConnected() {
    QMutexLocker locker(&m_poolMutex);
    // 只要有一个有效连接，就认为整体连接正常
    for (auto& db : m_connPool) {
        if (db->isOpen() && checkConnectionValid(*db)) {
            return true;
        }
    }
    return false;
}

// 获取当前连接数
int DatabaseManager::getConnCount() {
    QMutexLocker locker(&m_poolMutex);
    return m_connPool.size();
}

// 操作日志（性能监控 + 问题排查）
void DatabaseManager::logOperation(const QString& sql, qint64 elapsed) {
    // 仅记录耗时＞50ms 的 SQL（排查慢查询）
    if (elapsed > 50) {
        qDebug() << "慢查询警告：SQL 耗时" << elapsed << "ms，SQL：" << sql;
    } else {
        qDebug() << "SQL 执行成功，耗时" << elapsed << "ms";
    }

    // 可扩展：将日志写入文件或数据库（便于 24 小时监控）
}
