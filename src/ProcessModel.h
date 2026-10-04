#pragma once

#include <QAbstractTableModel>
#include <QTimer>
#include <QVector>

struct ProcessInfo
{
int pid = 0;
QString name;
QString user;
double cpu = 0.0;
double memory = 0.0;
QString status;
};

class ProcessModel : public QAbstractTableModel
{
Q_OBJECT

public:
explicit ProcessModel(QObject *parent = nullptr);

int rowCount(const QModelIndex &parent = QModelIndex()) const override;
int columnCount(const QModelIndex &parent = QModelIndex()) const override;

QVariant data(
    const QModelIndex &index,
    int role = Qt::DisplayRole) const override;

QVariant headerData(
    int section,
    Qt::Orientation orientation,
    int role = Qt::DisplayRole) const override;

void refresh();

bool killProcess(int row);


private:
QVector<ProcessInfo> m_processes;
};
