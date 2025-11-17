#ifndef STUDENT_H
#define STUDENT_H

// 去掉 QObject 继承，改为普通类
#include <QVariantMap>
#include <QDate>
#include <QString>
#include "DatabaseManager.h"
#include <QDebug>

// 普通 C++ 类（无 QObject 继承）
class Student { // 移除 : public QObject
public:
    // 构造函数保持不变（去掉 QObject 相关参数）
    Student();
    Student(const QString& studentId, const QString& name, const QString& gender,
            const QString& className, const QDate& birthDate = QDate(),
            const QString& phone = "");
    Student(const QVariantMap& dataMap);

    ~Student() = default;

    // Getter/Setter 方法保持不变
    int id() const { return m_id; }
    QString studentId() const { return m_studentId; }
    QString name() const { return m_name; }
    QString gender() const { return m_gender; }
    QString className() const { return m_className; }
    QDate birthDate() const { return m_birthDate; }
    QString phone() const { return m_phone; }
    QString createBy() const { return m_createBy; }
    QDateTime createTime() const { return m_createTime; }
    QString updateBy() const { return m_updateBy; }
    QDateTime updateTime() const { return m_updateTime; }
    int isDeleted() const { return m_isDeleted; }

    void setName(const QString& name) { m_name = name; }
    void setGender(const QString& gender) { m_gender = gender; }
    void setClassName(const QString& className) { m_className = className; }
    void setBirthDate(const QDate& birthDate) { m_birthDate = birthDate; }
    void setPhone(const QString& phone) { m_phone = phone; }
    void setUpdateBy(const QString& updateBy) { m_updateBy = updateBy; }

    // 序列化方法保持不变
    QVariantMap toVariantMap() const;

    // 静态数据库接口保持不变
    static bool insert(const Student& student);
    static bool update(const Student& student);
    static bool remove(const QString& studentId);
    static Student queryByStudentId(const QString& studentId);
    static QList<Student> queryAll(int page = 1, int pageSize = 50);
    static QList<Student> queryByCondition(const QString& className = "", const QString& name = "");

private:
    // 字段定义保持不变
    int m_id;
    QString m_studentId;
    QString m_name;
    QString m_gender;
    QString m_className;
    QDate m_birthDate;
    QString m_phone;
    QString m_createBy;
    QDateTime m_createTime;
    QString m_updateBy;
    QDateTime m_updateTime;
    int m_isDeleted;
};

#endif // STUDENT_H