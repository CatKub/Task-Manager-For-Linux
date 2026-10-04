#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
QApplication app(argc, argv);

app.setApplicationName("Task Manager");
app.setApplicationDisplayName("Task Manager");
app.setOrganizationName("TaskMgr Linux");

MainWindow window;
window.show();

return app.exec();


}
