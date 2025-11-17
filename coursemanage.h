#ifndef COURSEMANAGE_H
#define COURSEMANAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>

class CourseManage : public QWidget
{
    Q_OBJECT

public:
    explicit CourseManage(QWidget *parent = nullptr);
    ~CourseManage();

private slots:
    void addCourse();
    void editCourse();
    void deleteCourse();
    void importCourse();
    void exportCourse();
    void refreshCourseTable();

private:
    void setupUI();
    void initConnections();
    
    QTableWidget *table;
    QLineEdit *searchEdit;
    QComboBox *searchCombo;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *importButton;
    QPushButton *exportButton;
    QPushButton *refreshButton;
};

#endif // COURSEMANAGE_H
