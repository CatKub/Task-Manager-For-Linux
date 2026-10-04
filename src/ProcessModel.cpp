#include "ProcessModel.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

#include <algorithm>
#include <csignal>

ProcessModel::ProcessModel(QObject *parent)
: QAbstractTableModel(parent)
{
refresh();
}

int ProcessModel::rowCount(const QModelIndex &parent) const
{
if (parent.isValid())
return 0;

return m_processes.size();


}

int ProcessModel::columnCount(const QModelIndex &) const
{
return 6;
}

QVariant ProcessModel::data(const QModelIndex &index, int role) const
{
if (!index.isValid() || index.row() >= m_processes.size())
return {};

const auto &p = m_processes[index.row()];

if (role == Qt::DisplayRole)
{
    switch (index.column())
    {
    case 0: return p.name;
    case 1: return p.status;
    case 2: return QString::number(p.cpu, 'f', 1) + "%";
    case 3: return QString::number(p.memory, 'f', 1) + "%";
    case 4: return p.user;
    case 5: return p.pid;
    }
}

return {};


}

QVariant ProcessModel::headerData(
int section,
Qt::Orientation orientation,
int role) const
{
if (role != Qt::DisplayRole)
return {};

if (orientation == Qt::Horizontal)
{
    switch (section)
    {
    case 0: return "Name";
    case 1: return "Status";
    case 2: return "CPU";
    case 3: return "Memory";
    case 4: return "User";
    case 5: return "PID";
    }
}

return {};


}

void ProcessModel::refresh()
{
beginResetModel();

m_processes.clear();

QDir proc("/proc");

const auto entries = proc.entryList(
    QDir::Dirs | QDir::NoDotAndDotDot);

for (const QString &entry : entries)
{
    bool ok = false;
    int pid = entry.toInt(&ok);

    if (!ok)
        continue;

    QFile statusFile("/proc/" + entry + "/status");

    if (!statusFile.open(QIODevice::ReadOnly))
        continue;

    ProcessInfo info;
    info.pid = pid;

    QTextStream stream(&statusFile);

    while (!stream.atEnd())
    {
        const QString line = stream.readLine();

        if (line.startsWith("Name:"))
            info.name = line.section('\t', 1).trimmed();

        else if (line.startsWith("State:"))
            info.status = line.section('\t', 1).trimmed();

        else if (line.startsWith("Uid:"))
        {
            info.user = line.section('\t', 1)
                           .split(' ')
                           .first();
        }

        else if (line.startsWith("VmRSS:"))
        {
            const QString value =
                line.section('\t', 1)
                    .trimmed()
                    .split(' ')
                    .first();

            bool memoryOk = false;
            const double kb = value.toDouble(&memoryOk);

            if (memoryOk)
                info.memory = kb / 1024.0;
        }
    }

    if (!info.name.isEmpty())
        m_processes.append(info);
}

std::sort(
    m_processes.begin(),
    m_processes.end(),
    [](const ProcessInfo &a, const ProcessInfo &b)
    {
        return a.name.toLower() < b.name.toLower();
    });

endResetModel();


}

bool ProcessModel::killProcess(int row)
{
if (row < 0 || row >= m_processes.size())
return false;

const int pid = m_processes[row].pid;

return ::kill(pid, SIGTERM) == 0;


}
