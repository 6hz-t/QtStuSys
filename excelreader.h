#ifndef EXCELREADER_H
#define EXCELREADER_H

#include <QObject>
#include <QList>
#include <QString>
#include "Student.h" // 你的学生数据模型类

class ExcelReader : public QObject {
    Q_OBJECT
public:
    explicit ExcelReader(QObject *parent = nullptr);

    // 批量读取 Excel 中的学生数据（返回学生列表）
    QList<Student> readStudents(const QString& excelPath);

signals:
    // 读取进度信号（可选，用于显示进度条）
    void readProgress(int currentRow, int totalRows);

private:
    // 验证 Excel 表头是否合法（避免导入格式错误的文件）
    bool validateHeader(const QList<QString>& header);
};

#endif // EXCELREADER_H