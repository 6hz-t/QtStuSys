#include "scoremanage.h"
#include "databasemanager.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>

ScoreManage::ScoreManage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    initConnections();
    refreshScoreTable();
}

ScoreManage::~ScoreManage()
{
}

void ScoreManage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // 搜索栏
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchCombo = new QComboBox();
    searchCombo->addItems({"学号", "课程号", "考试类型"});
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
    table->setColumnCount(11);  // 更新列数
    QStringList headers;
    headers << "ID" << "学号" << "课程号" << "考试类型" << "分数" 
            << "状态" << "备注" << "创建人" << "创建时间" 
            << "更新人" << "更新时间";
    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setStretchLastSection(true);
    
    mainLayout->addLayout(searchLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(table);
    
    setLayout(mainLayout);
}


void ScoreManage::initConnections()
{
    connect(addButton, &QPushButton::clicked, this, &ScoreManage::addScore);
    connect(editButton, &QPushButton::clicked, this, &ScoreManage::editScore);
    connect(deleteButton, &QPushButton::clicked, this, &ScoreManage::deleteScore);
    connect(importButton, &QPushButton::clicked, this, &ScoreManage::importScore);
    connect(exportButton, &QPushButton::clicked, this, &ScoreManage::exportScore);
    connect(refreshButton, &QPushButton::clicked, this, &ScoreManage::refreshScoreTable);
}

void ScoreManage::refreshScoreTable()
{
    table->setRowCount(0);
    DatabaseManager* dbMgr = DatabaseManager::getInstance();
    if (!dbMgr->isConnected()) {
        QMessageBox::critical(this, "错误", "数据库未连接");
        return;
    }
    
    QString sql = "SELECT id, student_id, course_id, exam_type, score, "
                 "score_status, remark, create_by, create_time, update_by, update_time "
                 "FROM scores";
                 
    QList<QVariantMap> results = dbMgr->querySql(sql);
    
    for (const QVariantMap& row : results) {
        int currentRow = table->rowCount();
        table->insertRow(currentRow);
        
        table->setItem(currentRow, 0, new QTableWidgetItem(row["id"].toString()));
        table->setItem(currentRow, 1, new QTableWidgetItem(row["student_id"].toString()));
        table->setItem(currentRow, 2, new QTableWidgetItem(row["course_id"].toString()));
        table->setItem(currentRow, 3, new QTableWidgetItem(row["exam_type"].toString()));
        table->setItem(currentRow, 4, new QTableWidgetItem(row["score"].toString()));
        table->setItem(currentRow, 5, new QTableWidgetItem(row["score_status"].toString()));
        table->setItem(currentRow, 6, new QTableWidgetItem(row["remark"].toString()));
        table->setItem(currentRow, 7, new QTableWidgetItem(row["create_by"].toString()));
        table->setItem(currentRow, 8, new QTableWidgetItem(row["create_time"].toString()));
        table->setItem(currentRow, 9, new QTableWidgetItem(row["update_by"].toString()));
        table->setItem(currentRow, 10, new QTableWidgetItem(row["update_time"].toString()));
    }
}


void ScoreManage::addScore()
{
    QMessageBox::information(this, "提示", "新增成绩功能待实现");
}

void ScoreManage::editScore()
{
    QMessageBox::information(this, "提示", "编辑成绩功能待实现");
}

void ScoreManage::deleteScore()
{
    QMessageBox::information(this, "提示", "删除成绩功能待实现");
}

void ScoreManage::importScore()
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
    refreshScoreTable();
}

void ScoreManage::exportScore()
{
    QMessageBox::information(this, "提示", "导出成绩功能待实现");
}
