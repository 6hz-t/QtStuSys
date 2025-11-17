// ▶ 学生管理
//   - 学生信息列表
//   - 新增学生
//   - 批量导入/导出
// ▶ 课程管理
//   - 课程信息列表
//   - 新增课程
//   - 选课管理（中间表操作）
// ▶ 成绩管理
//   - 成绩录入（按课程/班级）
//   - 成绩查询/修改
//   - 成绩批量导入/导出
//   - 成绩作废/恢复
// ▶ 系统管理
//   - 操作日志查看
//   - 角色权限设置（可选）
//   - 系统配置
#ifndef SIDEMENUWIDGET_H
#define SIDEMENUWIDGET_H

#include <QTreeWidget>
#include <QWidget>
#include "studentmanage.h"
#include "scoremanage.h"
#include "systemmanage.h"
#include "coursemanage.h"

class Sidemenuwidget : public QWidget
{
    Q_OBJECT

public:
    explicit Sidemenuwidget(QWidget *parent = nullptr);
    ~Sidemenuwidget();

private:
    QTreeWidget *menuTree;


signals:

void scoremanage();
void systemmanage();
void coursemanage();
void studentmanage();

};

#endif // SIDEMENUWIDGET_H
