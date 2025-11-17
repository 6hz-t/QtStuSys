#ifndef STUDENTMANAGE_H
#define STUDENTMANAGE_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

class StudentManage : public QWidget
{
    Q_OBJECT

public:
    explicit StudentManage(QWidget *parent = nullptr);
    ~StudentManage();

private slots:
    void addStudent();
    void editStudent();
    void deleteStudent();
    void importStudent();
    void exportStudent();
    void refreshStudentTable();

private:
    void setupUI();
    void initConnections();

    QComboBox *searchCombo;
    QLineEdit *searchEdit;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *importButton;
    QPushButton *exportButton;
    QPushButton *refreshButton;
    QTableWidget *table;
};

#endif // STUDENTMANAGE_H
