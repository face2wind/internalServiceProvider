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
    this->setFixedWidth(210);
    this->setFixedHeight(330);
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

    if (ack->operate_list.size() > 0)
        ui->describe_txt->setText(ack->operate_list[0].operate_describe.c_str());

    project_list_ = ack->project_list;
    operate_list_ = ack->operate_list;
}

void MainWindow::on_execute_btn_clicked()
{
    int project_index = ui->project_name->currentIndex();
    int operate_index = ui->operate_type->currentIndex();
    if (project_index >= (int)project_list_.size() || operate_index >= (int)operate_list_.size())
        return;

    Protocol::CSGORequestCommand cmd;
    cmd.project_type = project_list_[project_index].project_type;
    cmd.operate_type = operate_list_[operate_index].operate_type;
    NetworkAgent::GetInstance().SendToServer(cmd);

    this->setFixedWidth(760);
}

void MainWindow::on_project_name_currentIndexChanged(int index)
{
    if (index >= (int)project_list_.size())
        return;
}

void MainWindow::on_operate_type_currentIndexChanged(int index)
{
    if (index >= (int)operate_list_.size())
        return;

    ui->describe_txt->setText(operate_list_[index].operate_describe.c_str());
}
