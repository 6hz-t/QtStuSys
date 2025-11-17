// 侧边菜单实现：使用单个 QTreeWidget + QTreeWidgetItem
#include "sidemenuwidget.h"
#include <QVBoxLayout>
#include <QTreeWidgetItem>

Sidemenuwidget::Sidemenuwidget(QWidget *parent)
    : QWidget(parent)
{
    menuTree = new QTreeWidget(this);
    menuTree->setHeaderHidden(true);

    // 学生管理
    QTreeWidgetItem *stuTop = new QTreeWidgetItem(menuTree);
    stuTop->setText(0, QStringLiteral("学生管理"));
    QTreeWidgetItem *stuInfo = new QTreeWidgetItem(stuTop);
    stuInfo->setText(0, QStringLiteral("学生信息列表"));
   
    
    QTreeWidgetItem *stuImpExp = new QTreeWidgetItem(stuTop);
    stuImpExp->setText(0, QStringLiteral("批量导入/导出"));

    // 课程管理
    QTreeWidgetItem *courseTop = new QTreeWidgetItem(menuTree);
    courseTop->setText(0, QStringLiteral("课程管理"));
    QTreeWidgetItem *courseInfo = new QTreeWidgetItem(courseTop);
    courseInfo->setText(0, QStringLiteral("课程信息列表"));
   
    
    
    QTreeWidgetItem *courseOption = new QTreeWidgetItem(courseTop);
    courseOption->setText(0, QStringLiteral("选课管理"));

    // 成绩管理
    QTreeWidgetItem *scoreTop = new QTreeWidgetItem(menuTree);
    scoreTop->setText(0, QStringLiteral("成绩管理"));

    QTreeWidgetItem *scoreQuery = new QTreeWidgetItem(scoreTop);
    scoreQuery->setText(0, QStringLiteral("成绩查询/录入"));
   
    
    QTreeWidgetItem *scoreModify = new QTreeWidgetItem(scoreTop);
    scoreModify->setText(0, QStringLiteral("成绩修改"));
    QTreeWidgetItem *scoreImpExp = new QTreeWidgetItem(scoreTop);
    scoreImpExp->setText(0, QStringLiteral("成绩批量导入/导出"));
    QTreeWidgetItem *scoreDiscard = new QTreeWidgetItem(scoreTop);
    scoreDiscard->setText(0, QStringLiteral("成绩作废/恢复"));



    // 系统管理
    QTreeWidgetItem *sysTop = new QTreeWidgetItem(menuTree);
    sysTop->setText(0, QStringLiteral("系统管理"));
    QTreeWidgetItem *sysLog = new QTreeWidgetItem(sysTop);
    sysLog->setText(0, QStringLiteral("操作日志查看"));
    
    QTreeWidgetItem *sysConfig = new QTreeWidgetItem(sysTop);
    sysConfig->setText(0, QStringLiteral("系统配置"));
    QTreeWidgetItem *sysRole = new QTreeWidgetItem(sysTop);
    sysRole->setText(0, QStringLiteral("角色权限设置"));

   
    connect(menuTree, &QTreeWidget::itemClicked, this, [this, stuInfo, courseInfo, scoreQuery, sysLog, stuImpExp](QTreeWidgetItem* item, int column) {
        Q_UNUSED(column)
        
        if (item == stuInfo) {
            emit studentmanage();
        } else if (item == courseInfo) {
            emit coursemanage();
        } else if (item == scoreQuery) {
            emit scoremanage();
        } else if (item == sysLog) {
            emit systemmanage();
        }
    });
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(menuTree);
    setMinimumWidth(200);
    setLayout(layout);






}

Sidemenuwidget::~Sidemenuwidget()
{
}
