#include "Student.h"
#include <QDateTime>
#include <QDebug>

// 修正构造函数（去掉 QObject 相关参数）
Student::Student()
    : m_id(0), m_isDeleted(0),
      m_createBy("dev"), m_createTime(QDateTime::currentDateTime()),
      m_updateBy("dev"), m_updateTime(QDateTime::currentDateTime()) {}

Student::Student(const QString& studentId, const QString& name, const QString& gender,
                 const QString& className, const QDate& birthDate, const QString& phone)
    : m_id(0), m_studentId(studentId), m_name(name), m_gender(gender),
      m_className(className), m_birthDate(birthDate), m_phone(phone),
      m_createBy("dev"), m_createTime(QDateTime::currentDateTime()),
      m_updateBy("dev"), m_updateTime(QDateTime::currentDateTime()),
      m_isDeleted(0) {}

Student::Student(const QVariantMap& dataMap) {
    m_id = dataMap["id"].toInt();
    m_studentId = dataMap["student_id"].toString();
    m_name = dataMap["name"].toString();
    m_gender = dataMap["gender"].toString();
    m_className = dataMap["class_name"].toString();
    m_birthDate = QDate::fromString(dataMap["birth_date"].toString(), "yyyy-MM-dd");
    m_phone = dataMap["phone"].toString();
    m_createBy = dataMap["create_by"].toString();
    m_createTime = QDateTime::fromString(dataMap["create_time"].toString(), "yyyy-MM-dd hh:mm:ss");
    m_updateBy = dataMap["update_by"].toString();
    m_updateTime = QDateTime::fromString(dataMap["update_time"].toString(), "yyyy-MM-dd hh:mm:ss");
    m_isDeleted = dataMap["is_deleted"].toInt();
}



// 序列化：转换为 QVariantMap
QVariantMap Student::toVariantMap() const {
    QVariantMap map;
    map["student_id"] = m_studentId;
    map["name"] = m_name;
    map["gender"] = m_gender;
    map["class_name"] = m_className;
    map["birth_date"] = m_birthDate.toString("yyyy-MM-dd");
    map["phone"] = m_phone;
    map["create_by"] = m_createBy;
    map["create_time"] = m_createTime.toString("yyyy-MM-dd hh:mm:ss");
    map["update_by"] = m_updateBy;
    map["update_time"] = m_updateTime.toString("yyyy-MM-dd hh:mm:ss");
    map["is_deleted"] = m_isDeleted;
    return map;
}

// 数据库操作：新增学生
bool Student::insert(const Student& student) {
    DatabaseManager* dbMgr = DatabaseManager::getInstance();
    if (!dbMgr->isConnected()) {
        qCritical() << "数据库未连接，新增学生失败";
        return false;
    }

    // 构造 SQL 和参数（对应 students 表字段）
    QString sql = R"(
        INSERT INTO students (student_id, name, gender, class_name, birth_date, phone, create_by, create_time, update_by, update_time)
        VALUES (:student_id, :name, :gender, :class_name, :birth_date, :phone, :create_by, :create_time, :update_by, :update_time)
    )";

    QVariantMap params = student.toVariantMap();
    // 确保创建时间和更新时间为当前时间（覆盖可能的旧值）
    params[":create_time"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    params[":update_time"] = params[":create_time"];

    // 执行 SQL（开启事务，确保原子性）
    return dbMgr->executeSql(sql, params, true);
}

// 数据库操作：修改学生（根据学号更新）
bool Student::update(const Student& student) {
    DatabaseManager* dbMgr = DatabaseManager::getInstance();
    if (!dbMgr->isConnected()) {
        qCritical() << "数据库未连接，修改学生失败";
        return false;
    }

    // 构造 SQL（仅更新可修改字段）
    QString sql = R"(
        UPDATE students
        SET name = :name, gender = :gender, class_name = :class_name, birth_date = :birth_date,
            phone = :phone, update_by = :update_by, update_time = :update_time
        WHERE student_id = :student_id AND is_deleted = 0
    )";

    QVariantMap params = student.toVariantMap();
    params[":update_time"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    return dbMgr->executeSql(sql, params, true);
}

// 数据库操作：逻辑删除学生（设置 is_deleted=1）
bool Student::remove(const QString& studentId) {
    DatabaseManager* dbMgr = DatabaseManager::getInstance();
    if (!dbMgr->isConnected()) {
        qCritical() << "数据库未连接，删除学生失败";
        return false;
    }

    QString sql = R"(
        UPDATE students
        SET is_deleted = 1, update_by = :update_by, update_time = :update_time
        WHERE student_id = :student_id AND is_deleted = 0
    )";

    QVariantMap params;
    params[":student_id"] = studentId;
    params[":update_by"] = "dev"; // 可改为当前登录用户
    params[":update_time"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    return dbMgr->executeSql(sql, params, true);
}

// 数据库操作：根据学号查询学生
Student Student::queryByStudentId(const QString& studentId) {
    DatabaseManager* dbMgr = DatabaseManager::getInstance();
    if (!dbMgr->isConnected()) {
        qCritical() << "数据库未连接，查询学生失败";
        return Student();
    }

    QString sql = R"(
        SELECT * FROM students
        WHERE student_id = :student_id AND is_deleted = 0
    )";

    QVariantMap params;
    params[":student_id"] = studentId;

    // 执行查询
    QList<QVariantMap> result = dbMgr->querySql(sql, params);
    if (result.isEmpty()) {
        qDebug() << "未查询到学号为" << studentId << "的学生";
        return Student();
    }

    // 转换为 Student 对象
    return Student(result.first());
}

// 数据库操作：分页查询所有未删除学生
QList<Student> Student::queryAll(int page, int pageSize) {
    DatabaseManager* dbMgr = DatabaseManager::getInstance();
    if (!dbMgr->isConnected()) {
        qCritical() << "数据库未连接，查询所有学生失败";
        return QList<Student>();
    }

    // 分页计算：LIMIT offset, pageSize
    int offset = (page - 1) * pageSize;
    QString sql = QString(R"(
        SELECT * FROM students
        WHERE is_deleted = 0
        ORDER BY student_id ASC
        LIMIT :offset, :pageSize
    )");

    QVariantMap params;
    params[":offset"] = offset;
    params[":pageSize"] = pageSize;

    // 执行查询并转换为 Student 列表
    QList<QVariantMap> result = dbMgr->querySql(sql, params);
    QList<Student> studentList;
    for (const QVariantMap& data : result) {
        //qDebug() << "查询到学生：" << data;
        studentList.append(Student(data));
        
    }
    qDebug() << "查询到size" << studentList.size() << "个学生";
    return studentList;
}

// 数据库操作：按条件查询（班级/姓名模糊查询）
QList<Student> Student::queryByCondition(const QString& className, const QString& name) {
    DatabaseManager* dbMgr = DatabaseManager::getInstance();
    if (!dbMgr->isConnected()) {
        qCritical() << "数据库未连接，条件查询学生失败";
        return QList<Student>();
    }

    // 动态构造查询条件
    QString sql = "SELECT * FROM students WHERE is_deleted = 0";
    QVariantMap params;

    if (!className.isEmpty()) {
        sql += " AND class_name = :class_name";
        params[":class_name"] = className;
    }

    if (!name.isEmpty()) {
        sql += " AND name LIKE :name";
        params[":name"] = "%" + name + "%"; // 模糊查询
    }

    sql += " ORDER BY student_id ASC";

    // 执行查询并转换为 Student 列表
    QList<QVariantMap> result = dbMgr->querySql(sql, params);
    QList<Student> studentList;
    for (const QVariantMap& data : result) {
        studentList.append(Student(data));
    }

    return studentList;
}