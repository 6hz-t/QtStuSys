#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QWidget>
#include <QLabel>
#include "sidemenuwidget.h"
#include "studentmanage.h"
#include "systemmanage.h"
#include "coursemanage.h"
#include "scoremanage.h"
#include <QApplication>
#include "xlsxdocument.h"
#include "databasemanager.h"
#include <QTimer>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// 前向声明
class Sidemenuwidget;
class StudentManage;
class SystemManage;
class CourseManage;
class ScoreManage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void StudentManageSlot();
    void SystemManageSlot();
    void CourseManageSlot();
    void ScoreManageSlot();
    
    // 菜单和工具栏槽函数
    void newFile();
    void openFile();
    void saveFile();
    void about();

private:
    void initApp();
    void createMenus();
    
    void createStatusBar();
    
    DatabaseManager* dbMgr;
    QDockWidget *dockwidget;

    StudentManage *studentmanage; 
    SystemManage *systemmanage;
    CourseManage *coursemanage;
    ScoreManage *scoremanage;

    QMenuBar *menuBar;
    
    // 菜单栏相关
    QMenu *fileMenu;
    

    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    
    
    
    // 状态栏相关
    QLabel *statusLabel;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H