#include "mainwindow.h"
#include "ui_chatwindow.h"
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

void MainWindow::SetData()
{
    QStringList str_list;
    str_list<<"天天爱仙侠"<<"灵域修真"<<"mg22";
    ui->project_name->addItems(str_list);
}

void MainWindow::on_execute_btn_clicked()
{

}
