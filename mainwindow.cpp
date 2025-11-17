#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sidemenuwidget.h"
#include <QDockWidget>
#include "databasemanager.h"
#include <QMessageBox>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QAction>
#include <QFileDialog>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), dbMgr(nullptr), dockwidget(nullptr), studentmanage(nullptr), systemmanage(nullptr), coursemanage(nullptr), scoremanage(nullptr), fileMenu(nullptr), editMenu(nullptr), viewMenu(nullptr), helpMenu(nullptr), statusLabel(nullptr)
{
    dbMgr = DatabaseManager::getInstance();
    ui->setupUi(this);
    initApp();
}

void MainWindow::initApp()
{

    try
    {
        studentmanage = new StudentManage(this);
        coursemanage = new CourseManage(this);
        scoremanage = new ScoreManage(this);
        systemmanage = new SystemManage(this);
        // ... 其他初始化代码
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "错误", QString("初始化失败: %1").arg(e.what()));
        qApp->quit();
    }
    setWindowTitle(QStringLiteral("Qt学生管理系统"));
    // 创建学生管理模块作为默认中央部件

    setCentralWidget(studentmanage);

    // 创建侧边栏菜单
    dockwidget = new QDockWidget(tr("菜单"), this);
    dockwidget->setAllowedAreas(Qt::LeftDockWidgetArea);
    Sidemenuwidget *side = new Sidemenuwidget(dockwidget);
    dockwidget->setWidget(side);
    addDockWidget(Qt::LeftDockWidgetArea, dockwidget);

    // 设置窗口最小尺寸
    setMinimumSize(1400, 800);

    // 创建菜单栏、工具栏和状态栏
    createMenus();

    createStatusBar();

    // 检查数据库连接状态
    if (!dbMgr->isConnected())
    {
        QMessageBox::critical(this, "错误", "数据库连接失败！请检查配置后重试。");
        qApp->quit();
        return;
    }

    // 连接数据库错误信号
    connect(dbMgr, &DatabaseManager::errorOccurred, this, [=](const QString &error)
            {
        QMessageBox::warning(this, "数据库错误", error);
        statusLabel->setText("数据库错误: " + error); });

    // 连接侧边栏菜单信号
    connect(side, &Sidemenuwidget::studentmanage, this, &MainWindow::StudentManageSlot);
    connect(side, &Sidemenuwidget::systemmanage, this, &MainWindow::SystemManageSlot);
    connect(side, &Sidemenuwidget::coursemanage, this, &MainWindow::CourseManageSlot);
    connect(side, &Sidemenuwidget::scoremanage, this, &MainWindow::ScoreManageSlot);
}

void MainWindow::createMenus()
{
    menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    // 文件菜单
    fileMenu = new QMenu(tr("文件"), this);
    QAction *newAction = new QAction(tr("新建(&N)"), this);
    QAction *openAction = new QAction(tr("打开(&O)"), this);
    QAction *saveAction = new QAction(tr("保存(&S)"), this);

    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);

    menuBar->addMenu(fileMenu);

    // 编辑菜单
    editMenu = new QMenu(tr("编辑"), this);
    QAction *cutAction = new QAction(tr("剪切(&T)"), this);
    QAction *copyAction = new QAction(tr("复制(&C)"), this);
    QAction *pasteAction = new QAction(tr("粘贴(&P)"), this);

    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    menuBar->addMenu(editMenu);

    // 视图菜单
    viewMenu = new QMenu(tr("视图"), this);
    QAction *zoomInAction = new QAction(tr("放大(&Z)"), this);
    QAction *zoomOutAction = new QAction(tr("缩小(&M)"), this);
    QAction *zoomResetAction = new QAction(tr("重置(&R)"), this);

    viewMenu->addAction(zoomInAction);
    viewMenu->addAction(zoomOutAction);
    viewMenu->addAction(zoomResetAction);

    menuBar->addMenu(viewMenu);

    // 帮助菜单
    helpMenu = new QMenu(tr("帮助"), this);

    menuBar->addMenu(helpMenu);
}

void MainWindow::createStatusBar()
{
    // 创建状态栏标签
    statusLabel = new QLabel(tr("就绪"));
    statusLabel->setObjectName("statusLabel");

    // 设置状态栏
    statusBar()->addWidget(statusLabel);

    // 显示当前时间
    QLabel *timeLabel = new QLabel(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    statusBar()->addPermanentWidget(timeLabel);

    // 每秒更新时间
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]()
            { timeLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")); });
    timer->start(1000);
}

void MainWindow::StudentManageSlot()
{
    if (!studentmanage)
    {
        studentmanage = new StudentManage(this);
    }
    takeCentralWidget();
    setCentralWidget(studentmanage);
    statusLabel->setText(tr("学生管理模块"));
}

void MainWindow::ScoreManageSlot()
{
    takeCentralWidget();
    setCentralWidget(scoremanage);
    statusLabel->setText(tr("成绩管理模块"));
}

void MainWindow::SystemManageSlot()
{
    takeCentralWidget();
    setCentralWidget(systemmanage);
    statusLabel->setText(tr("系统管理模块"));
}

void MainWindow::CourseManageSlot()
{
    takeCentralWidget();
    setCentralWidget(coursemanage);
    statusLabel->setText(tr("课程管理模块"));
}

// 菜单和工具栏槽函数实现
void MainWindow::newFile()
{
    statusLabel->setText(tr("执行了新建操作"));
    QMessageBox::information(this, tr("新建"), tr("新建功能待实现"));
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"), "",
                                                    tr("所有文件 (*)"));
    if (!fileName.isEmpty())
    {
        statusLabel->setText(tr("打开了文件: %1").arg(fileName));
    }
}

void MainWindow::saveFile()
{
    statusLabel->setText(tr("执行了保存操作"));
    QMessageBox::information(this, tr("保存"), tr("保存功能待实现"));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("关于"),
                       tr("<h2>学生成绩管理系统</h2>"
                          "<p>版本: 1.0</p>"
                          "<p>这是一个基于Qt的学生信息管理系统</p>"));
}

MainWindow::~MainWindow()
{
    if (studentmanage)
        delete studentmanage;
    if (coursemanage)
        delete coursemanage;
    if (scoremanage)
        delete scoremanage;
    if (systemmanage)
        delete systemmanage;
    delete ui;
}
