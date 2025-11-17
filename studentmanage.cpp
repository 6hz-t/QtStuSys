#include "studentmanage.h"
#include "student.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QDebug>

StudentManage::StudentManage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    initConnections();
    refreshStudentTable();
}

StudentManage::~StudentManage()
{
}

void StudentManage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // 搜索栏
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchCombo = new QComboBox();
    searchCombo->addItems({"学号", "姓名", "班级"});
    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("请输入搜索内容");
    searchLayout->addWidget(searchCombo);
    searchLayout->addWidget(searchEdit);
    
    // 按钮栏
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    addButton = new QPushButton("新增");
    editButton = new QPushButton("编辑");
    deleteButton = new QPushButton("删除");
    importButton = new QPushButton("导入");
    exportButton = new QPushButton("导出");
    refreshButton = new QPushButton("刷新");
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(importButton);
    buttonLayout->addWidget(exportButton);
    buttonLayout->addWidget(refreshButton);
    
    // 表格
    table = new QTableWidget();
    table->setColumnCount(11);
    QStringList headers;
    headers << "ID" << "学号" << "姓名" << "性别" << "班级" << "出生日期" 
           << "联系方式" << "创建人" << "创建时间" << "更新人" << "更新时间";
    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setStretchLastSection(true);
    
    mainLayout->addLayout(searchLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(table);
    
    setLayout(mainLayout);
}

void StudentManage::initConnections()
{
    connect(addButton, &QPushButton::clicked, this, &StudentManage::addStudent);
    connect(editButton, &QPushButton::clicked, this, &StudentManage::editStudent);
    connect(deleteButton, &QPushButton::clicked, this, &StudentManage::deleteStudent);
    connect(importButton, &QPushButton::clicked, this, &StudentManage::importStudent);
    connect(exportButton, &QPushButton::clicked, this, &StudentManage::exportStudent);
    connect(refreshButton, &QPushButton::clicked, this, &StudentManage::refreshStudentTable);
    
}

void StudentManage::refreshStudentTable()
{
    table->setRowCount(0);
    QList<Student> students = Student::queryAll(1, 20);
    qDebug() << "从数据库获取到的学生数量:" << students.size();
    
    for (int i = 0; i < students.size(); i++) {
        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(QString::number(students[i].id())));
        table->setItem(i, 1, new QTableWidgetItem(students[i].studentId()));
        table->setItem(i, 2, new QTableWidgetItem(students[i].name()));
        table->setItem(i, 3, new QTableWidgetItem(students[i].gender()));
        table->setItem(i, 4, new QTableWidgetItem(students[i].className()));
        table->setItem(i, 5, new QTableWidgetItem(students[i].birthDate().toString("yyyy-MM-dd")));
        table->setItem(i, 6, new QTableWidgetItem(students[i].phone()));
        table->setItem(i, 7, new QTableWidgetItem(students[i].createBy()));
        table->setItem(i, 8, new QTableWidgetItem(students[i].createTime().toString("yyyy-MM-dd hh:mm:ss")));
        table->setItem(i, 9, new QTableWidgetItem(students[i].updateBy()));
        table->setItem(i, 10, new QTableWidgetItem(students[i].updateTime().toString("yyyy-MM-dd hh:mm:ss")));
    }
}

void StudentManage::addStudent()
{
    QMessageBox::information(this, "提示", "新增学生功能待实现");
}

void StudentManage::editStudent()
{
    QMessageBox::information(this, "提示", "编辑学生功能待实现");
}

void StudentManage::deleteStudent()
{
    QMessageBox::information(this, "提示", "删除学生功能待实现");
}

void StudentManage::importStudent()
{
    QString excelPath = QFileDialog::getOpenFileName(
        this, 
        "选择Excel文件", 
        QDir::homePath(), 
        "Excel Files (*.xlsx);;All Files (*.*)"
    );
    if (excelPath.isEmpty()) {
        return;
    }
    QMessageBox::information(this, "提示", "Excel导入功能待实现，文件路径：" + excelPath);
    refreshStudentTable();
}

void StudentManage::exportStudent()
{
    QMessageBox::information(this, "提示", "导出学生功能待实现");
}
