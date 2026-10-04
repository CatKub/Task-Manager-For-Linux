#include "MainWindow.h"
#include "ProcessModel.h"

#include <QTableView>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QTimer>
#include <QMessageBox>
#include <QSortFilterProxyModel>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
{
setWindowTitle("Task Manager");
resize(1100, 700);

m_model = new ProcessModel(this);

auto *proxy = new QSortFilterProxyModel(this);
proxy->setSourceModel(m_model);
proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
proxy->setFilterKeyColumn(0);

auto *central = new QWidget(this);
auto *layout = new QVBoxLayout(central);

auto *top = new QHBoxLayout;

m_search = new QLineEdit;
m_search->setPlaceholderText("Search...");
m_search->setClearButtonEnabled(true);

m_endTask = new QPushButton("End task");
auto *refresh = new QPushButton("Refresh");

top->addWidget(m_search, 1);
top->addWidget(refresh);
top->addWidget(m_endTask);

layout->addLayout(top);

m_table = new QTableView;
m_table->setModel(proxy);

m_table->setAlternatingRowColors(true);
m_table->setSortingEnabled(true);
m_table->sortByColumn(0, Qt::AscendingOrder);

m_table->setSelectionBehavior(
    QAbstractItemView::SelectRows);

m_table->setSelectionMode(
    QAbstractItemView::SingleSelection);

m_table->verticalHeader()->setVisible(false);

m_table->horizontalHeader()->setStretchLastSection(true);
m_table->horizontalHeader()->setSectionResizeMode(
    0, QHeaderView::Stretch);

layout->addWidget(m_table, 1);

m_status = new QLabel("Processes");
layout->addWidget(m_status);

setCentralWidget(central);

connect(
    refresh,
    &QPushButton::clicked,
    this,
    &MainWindow::refreshProcesses);

connect(
    m_endTask,
    &QPushButton::clicked,
    this,
    &MainWindow::endTask);

connect(
    m_search,
    &QLineEdit::textChanged,
    proxy,
    &QSortFilterProxyModel::setFilterFixedString);

auto *timer = new QTimer(this);

connect(
    timer,
    &QTimer::timeout,
    this,
    &MainWindow::refreshProcesses);

timer->start(2000);

setStyleSheet(R"(
    QMainWindow {
        background: #f7f7f7;
    }

    QTableView {
        background: white;
        alternate-background-color: #f5f5f5;
        border: 1px solid #d9d9d9;
        gridline-color: #eeeeee;
        selection-background-color: #dbeafe;
        selection-color: #111111;
    }

    QHeaderView::section {
        background: #fafafa;
        border: none;
        border-bottom: 1px solid #dddddd;
        padding: 8px;
        color: #444444;
    }

    QLineEdit {
        background: white;
        border: 1px solid #cccccc;
        border-radius: 5px;
        padding: 8px;
    }

    QPushButton {
        background: #ffffff;
        border: 1px solid #cccccc;
        border-radius: 5px;
        padding: 8px 16px;
    }

    QPushButton:hover {
        background: #eeeeee;
    }
)");


}

void MainWindow::refreshProcesses()
{
m_model->refresh();

m_status->setText(
    QString("%1 processes")
        .arg(m_model->rowCount()));


}

void MainWindow::endTask()
{
const QModelIndex index =
m_table->currentIndex();

if (!index.isValid())
    return;

auto *proxy =
    qobject_cast<QSortFilterProxyModel *>(
        m_table->model());

if (!proxy)
    return;

const QModelIndex sourceIndex =
    proxy->mapToSource(index);

const int row = sourceIndex.row();

if (m_model->killProcess(row))
    refreshProcesses();
else
    QMessageBox::warning(
        this,
        "Task Manager",
        "Unable to terminate this process.\n"
        "You may need administrator privileges.");


}
