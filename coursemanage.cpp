#include "coursemanage.h"
#include "databasemanager.h"
#include <QHeaderView>
#include <QSqlQuery>
#include <QSqlError>

CourseManage::CourseManage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    initConnections();
    refreshCourseTable();
}

CourseManage::~CourseManage()
{
}

void CourseManage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // 搜索栏
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchCombo = new QComboBox();
    searchCombo->addItems({"课程号", "课程名", "教师名", "学期"});
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
    table->setColumnCount(9);
    QStringList headers;
    headers << "ID" << "课程号" << "课程名" << "学分" << "教师名" 
            << "学期" << "创建时间" << "更新时间" << "状态";
    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setStretchLastSection(true);
    
    mainLayout->addLayout(searchLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(table);
    
    setLayout(mainLayout);
}

void CourseManage::initConnections()
{
    connect(addButton, &QPushButton::clicked, this, &CourseManage::addCourse);
    connect(editButton, &QPushButton::clicked, this, &CourseManage::editCourse);
    connect(deleteButton, &QPushButton::clicked, this, &CourseManage::deleteCourse);
    connect(importButton, &QPushButton::clicked, this, &CourseManage::importCourse);
    connect(exportButton, &QPushButton::clicked, this, &CourseManage::exportCourse);
    connect(refreshButton, &QPushButton::clicked, this, &CourseManage::refreshCourseTable);
}

void CourseManage::refreshCourseTable()
{
    table->setRowCount(0);
    DatabaseManager* dbMgr = DatabaseManager::getInstance();
    if (!dbMgr->isConnected()) {
        QMessageBox::critical(this, "错误", "数据库未连接");
        return;
    }
    
    QString sql = "SELECT id, course_id, course_name, credit, teacher_name, "
                 "semester, create_time, update_time, is_deleted "
                 "FROM courses WHERE is_deleted = 0";
                 
    // 使用querySql替代executeQuery
    QList<QVariantMap> results = dbMgr->querySql(sql);
    
    // 遍历结果集
    for (const QVariantMap& row : results) {
        int currentRow = table->rowCount();
        table->insertRow(currentRow);
        
        table->setItem(currentRow, 0, new QTableWidgetItem(row["id"].toString()));
        table->setItem(currentRow, 1, new QTableWidgetItem(row["course_id"].toString()));
        table->setItem(currentRow, 2, new QTableWidgetItem(row["course_name"].toString()));
        table->setItem(currentRow, 3, new QTableWidgetItem(row["credit"].toString()));
        table->setItem(currentRow, 4, new QTableWidgetItem(row["teacher_name"].toString()));
        table->setItem(currentRow, 5, new QTableWidgetItem(row["semester"].toString()));
        table->setItem(currentRow, 6, new QTableWidgetItem(row["create_time"].toString()));
        table->setItem(currentRow, 7, new QTableWidgetItem(row["update_time"].toString()));
        table->setItem(currentRow, 8, new QTableWidgetItem(row["is_deleted"].toString()));
    }
}


void CourseManage::addCourse()
{
    // TODO: 实现新增课程功能
    QMessageBox::information(this, "提示", "新增课程功能待实现");
}

void CourseManage::editCourse()
{
    // TODO: 实现编辑课程功能
    QMessageBox::information(this, "提示", "编辑课程功能待实现");
}

void CourseManage::deleteCourse()
{
    // TODO: 实现删除课程功能
    QMessageBox::information(this, "提示", "删除课程功能待实现");
}

void CourseManage::importCourse()
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
    refreshCourseTable();
}

void CourseManage::exportCourse()
{
    // TODO: 实现导出课程功能
    QMessageBox::information(this, "提示", "导出课程功能待实现");
}
