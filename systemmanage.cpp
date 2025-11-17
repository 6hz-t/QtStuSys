#include "systemmanage.h"
#include "databasemanager.h"
#include <QHeaderView>
#include <QMessageBox>

SystemManage::SystemManage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    initConnections();
    refreshOperationLogs();
}

SystemManage::~SystemManage()
{
}

void SystemManage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // 搜索栏
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchCombo = new QComboBox();
    searchCombo->addItems({"用户名", "角色"});
    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("请输入搜索内容");
    searchLayout->addWidget(searchCombo);
    searchLayout->addWidget(searchEdit);
    
    // 按钮栏
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    addButton = new QPushButton("新增");
    editButton = new QPushButton("编辑");
    deleteButton = new QPushButton("删除");
    refreshButton = new QPushButton("刷新");
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(refreshButton);
    
    // 表格
    table = new QTableWidget();
    table->setColumnCount(7);
    QStringList headers;
    headers << "ID" << "模块名称" << "操作类型" << "操作内容" 
           << "操作人" << "操作IP" << "操作时间";
    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setStretchLastSection(true);
    
    mainLayout->addLayout(searchLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(table);
    
    setLayout(mainLayout);
}

void SystemManage::initConnections()
{
    connect(addButton, &QPushButton::clicked, this, &SystemManage::addUser);
    connect(editButton, &QPushButton::clicked, this, &SystemManage::editUser);
    connect(deleteButton, &QPushButton::clicked, this, &SystemManage::deleteUser);
    connect(refreshButton, &QPushButton::clicked, this, &SystemManage::refreshOperationLogs);
}
void SystemManage::refreshOperationLogs()
{
    table->setRowCount(0);
    DatabaseManager* dbMgr = DatabaseManager::getInstance();
    if (!dbMgr->isConnected()) {
        QMessageBox::critical(this, "错误", "数据库未连接");
        return;
    }
    QString sql = "SELECT id, module_name, operation_type, operation_content, "
                 "operated_by, operated_ip, operated_time "
                 "FROM operation_logs ORDER BY operated_time DESC";
    QList<QVariantMap> results = dbMgr->querySql(sql);
    
    for (const QVariantMap& row : results) {
        int currentRow = table->rowCount();
        table->insertRow(currentRow);
        
        table->setItem(currentRow, 0, new QTableWidgetItem(row["id"].toString()));
        table->setItem(currentRow, 1, new QTableWidgetItem(row["module_name"].toString()));
        table->setItem(currentRow, 2, new QTableWidgetItem(row["operation_type"].toString()));
        table->setItem(currentRow, 3, new QTableWidgetItem(row["operation_content"].toString()));
        table->setItem(currentRow, 4, new QTableWidgetItem(row["operated_by"].toString()));
        table->setItem(currentRow, 5, new QTableWidgetItem(row["operated_ip"].toString()));
        table->setItem(currentRow, 6, new QTableWidgetItem(row["operated_time"].toString()));
    }
}


void SystemManage::addUser()
{
    QMessageBox::information(this, "提示", "新增用户功能待实现");
}

void SystemManage::editUser()
{
    QMessageBox::information(this, "提示", "编辑用户功能待实现");
}

void SystemManage::deleteUser()
{
    QMessageBox::information(this, "提示", "删除用户功能待实现");
}
