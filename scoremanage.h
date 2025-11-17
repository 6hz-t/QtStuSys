#ifndef SCOREMANAGE_H
#define SCOREMANAGE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QComboBox>
#include <QLineEdit>

class ScoreManage : public QWidget
{
    Q_OBJECT

public:
    explicit ScoreManage(QWidget *parent = nullptr);
    ~ScoreManage();

private slots:
    void addScore();
    void editScore();
    void deleteScore();
    void importScore();
    void exportScore();
    void refreshScoreTable();

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
    QPushButton *importButton;
    QPushButton *exportButton;
    QPushButton *refreshButton;
};

#endif // SCOREMANAGE_H
