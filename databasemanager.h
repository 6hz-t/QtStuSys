#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QList>              // 添加这个头文件
#include <QMap>               // 添加这个头文件
#include <QVariant>           // 添加这个头文件
#include <QSqlRecord>      // 添加这一行 - 解决 QSqlRecord 问题
#include <QThread>         // 添加这一行 - 解决 QThread 问题
#include <QApplication>    // 添加这一行 - 解决 qApp 问题
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QList>
#include <QMutex>
#include <QMutexLocker>
#include <QDebug>
#include <QElapsedTimer>
#include <QDateTime>
#include <QSharedPointer>
#include <QSqlRecord>  
#include <QSqlDriver>  

// 数据库连接配置（可根据实际环境修改）

#define DB_HOST "124.221.15.13"
#define DB_NAME "qtserver"
#define DB_USER "qtserver"
#define DB_PWD "Xz2CaJnWGtYdY4YP"
#define DB_PORT 3306
#define MAX_CONN_COUNT 5
#define BATCH_SIZE 1000

// 单例数据库管理类
class DatabaseManager : public QObject {
    Q_OBJECT
public:
    // 1. 单例获取（线程安全）
    static DatabaseManager* getInstance();

    // 2. 基础操作：获取连接/释放连接
    QSharedPointer<QSqlDatabase> getConnection(); // 返回智能指针，自动释放连接
    void releaseConnection(QSharedPointer<QSqlDatabase> db);

    // 3. 单条 CRUD 操作（带事务支持）
    bool executeSql(const QString& sql, const QVariantMap& params = QVariantMap(), bool isTransaction = false);
    QList<QVariantMap> querySql(const QString& sql, const QVariantMap& params = QVariantMap());

    // 4. 批量操作（分块处理，控制内存）
    bool batchExecute(const QString& sql, const QList<QVariantMap>& paramsList);

    // 5. 事务操作（手动控制事务，适用于多步操作原子性）
    bool beginTransaction(QSharedPointer<QSqlDatabase> db);
    bool commitTransaction(QSharedPointer<QSqlDatabase> db);
    bool rollbackTransaction(QSharedPointer<QSqlDatabase> db);

    // 6. 状态查询
    bool isConnected();
    int getConnCount(); // 获取当前连接数

signals:
    void errorOccurred(const QString& errorMsg); // 错误信号（主线程处理）

private:
    // 私有构造/析构（禁止外部实例化）
    DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    // 禁止拷贝赋值
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    // 辅助方法
    QSqlDatabase createNewConnection(); // 创建新连接
    void initConnectionPool();          // 初始化连接池
    bool checkConnectionValid(QSqlDatabase& db); // 检查连接有效性
    void logOperation(const QString& sql, qint64 elapsed); // 操作日志（性能监控）

private:
    static DatabaseManager* m_instance;       // 单例实例
    static QMutex m_instanceMutex;            // 单例锁（线程安全）
    QList<QSharedPointer<QSqlDatabase>> m_connPool; // 连接池
    QMutex m_poolMutex;                       // 连接池锁（线程安全）
    bool m_isInitSuccess;                     // 初始化状态
};

#endif // DATABASEMANAGER_H
