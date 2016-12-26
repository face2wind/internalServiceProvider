#include <QApplication>

#include "ui/ui_manager.hpp"
#include "network/networkagent.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    NetworkAgent::GetInstance();
    UIManager::GetInstance().ShowMainView();

    return a.exec();
}
