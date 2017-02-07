#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "network/networkagent.h"
#include <QKeyEvent>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetOperateData(Protocol::SCGORequestCommandListACK *ack)
{
    QStringList name_str_list;
    for (Protocol::SCGOCommandProjectItem item : ack->project_list)
        name_str_list<<item.project_name.c_str();
    ui->project_name->addItems(name_str_list);

    QStringList opera_str_list;
    for (Protocol::SCGOCommandOperateItem item : ack->operate_list)
        opera_str_list<<item.operate_name.c_str();
    ui->operate_type->addItems(opera_str_list);
}

void MainWindow::on_execute_btn_clicked()
{

}
