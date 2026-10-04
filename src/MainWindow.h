#pragma once

#include <QMainWindow>

class QTableView;
class QLineEdit;
class QLabel;
class QPushButton;
class ProcessModel;

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
explicit MainWindow(QWidget *parent = nullptr);

private slots:
void refreshProcesses();
void endTask();

private:
QTableView *m_table;
QLineEdit *m_search;
QLabel *m_status;
QPushButton *m_endTask;

ProcessModel *m_model;


};
