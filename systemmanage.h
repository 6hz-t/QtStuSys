#ifndef SYSTEMMANAGE_H
#define SYSTEMMANAGE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QComboBox>
#include <QLineEdit>

class SystemManage : public QWidget
{
    Q_OBJECT

public:
    explicit SystemManage(QWidget *parent = nullptr);
    ~SystemManage();

private slots:
    void addUser();
    void editUser();
    void deleteUser();
    void refreshOperationLogs();

private:
    void setupUI();
    void initConnections();
    
    QLabel *label_;
    QTableWidget *table;
    QComboBox *searchCombo;
    QLineEdit *searchEdit;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *refreshButton;
};

#endif // SYSTEMMANAGE_H
