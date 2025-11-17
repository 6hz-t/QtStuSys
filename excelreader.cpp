#include "excelreader.h"
#include <QXlsx/xlsxdocument.h>
#include <QXlsx/xlsxworksheet.h>
#include <QXlsx/xlsxcellrange.h>
#include <QDate>
#include <QDebug>
#include "student.h"

ExcelReader::ExcelReader(QObject *parent) : QObject(parent) {}

// 验证 Excel 表头（必须包含：学号、姓名、性别、班级、出生日期、联系方式）
bool ExcelReader::validateHeader(const QList<QString>& header) {
    QList<QString> requiredHeaders = {"学号", "姓名", "性别", "班级", "出生日期", "联系方式"};
    foreach (const QString& reqHeader, requiredHeaders) {
        if (!header.contains(reqHeader)) {
            qCritical() << "Excel 表头错误：缺少必填列" << reqHeader;
            return false;
        }
    }
    return true;
}

// 批量读取学生数据
QList<Student> ExcelReader::readStudents(const QString& excelPath) {
    QList<Student> studentList;

    // 1. 打开 Excel 文件
    QXlsx::Document xlsxDoc(excelPath);
    if (!xlsxDoc.isOpen()) {
        qCritical() << "打开 Excel 文件失败：" << excelPath;
        return studentList;
    }

    // 2. 获取第一个工作表（默认读取第一个sheet）
    QXlsx::Worksheet *worksheet = xlsxDoc.currentWorksheet();
    if (!worksheet) {
        qCritical() << "Excel 文件中无工作表";
        return studentList;
    }

    // 3. 获取表格范围（总行数、总列数）
    QXlsx::CellRange range = worksheet->usedRange();
    int totalRows = range.rowCount(); // 总行数（包含表头）
    int totalCols = range.columnCount(); // 总列数
    qDebug() << "Excel 总行数：" << totalRows << "，总列数：" << totalCols;

    if (totalRows < 2) { // 至少需要 1 行表头 + 1 行数据
        qCritical() << "Excel 文件中无有效数据（至少需要表头+1行数据）";
        return studentList;
    }

    // 4. 读取表头（第 1 行）
    QList<QString> header;
    for (int col = 1; col <= totalCols; col++) {
        QString headerText = worksheet->read(1, col).toString().trimmed(); // 第1行是表头
        header.append(headerText);
    }

    // 5. 验证表头合法性
    if (!validateHeader(header)) {
        return studentList;
    }

    // 6. 批量读取数据行（从第 2 行开始）
    int headerCount = header.size();
    for (int row = 2; row <= totalRows; row++) {
        // 发送进度信号（可选）
        emit readProgress(row - 1, totalRows - 1); // row-1：当前数据行索引，totalRows-1：总数据行数

        // 读取当前行的所有列数据
        QList<QString> rowData;
        for (int col = 1; col <= headerCount; col++) {
            QString cellValue = worksheet->read(row, col).toString().trimmed();
            rowData.append(cellValue);
        }

        // 解析数据（按表头顺序：学号、姓名、性别、班级、出生日期、联系方式）
        QString studentId = rowData[header.indexOf("学号")];
        QString name = rowData[header.indexOf("姓名")];
        QString gender = rowData[header.indexOf("性别")];
        QString className = rowData[header.indexOf("班级")];
        QString birthDateStr = rowData[header.indexOf("出生日期")];
        QString phone = rowData[header.indexOf("联系方式")];

        // 验证必填字段（学号、姓名、性别、班级不能为空）
        if (studentId.isEmpty() || name.isEmpty() || gender.isEmpty() || className.isEmpty()) {
            qWarning() << "第" << row << "行数据不完整，跳过：" << rowData;
            continue;
        }

        // 解析出生日期（支持 "yyyy-MM-dd"、"yyyy/MM/dd" 格式）
        QDate birthDate = QDate::fromString(birthDateStr, "yyyy-MM-dd");
        if (!birthDate.isValid()) {
            birthDate = QDate::fromString(birthDateStr, "yyyy/MM/dd");
        }
        if (!birthDate.isValid()) {
            qWarning() << "第" << row << "行出生日期格式错误，设为空：" << birthDateStr;
            birthDate = QDate();
        }

        // 创建 Student 对象并添加到列表
        Student student(studentId, name, gender, className, birthDate, phone);
        studentList.append(student);
    }

    qDebug() << "Excel 读取完成，共读取" << studentList.size() << "个学生数据";
    return studentList;
}