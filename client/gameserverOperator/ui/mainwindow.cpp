#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "network/networkagent.h"
#include <QKeyEvent>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow),
    cur_operate_type_(OperateType::DEV_SERVER_NORMAL),
    last_project_index_(0), last_operate_index_(0)
{
    hot_update_type_map_["/globalconfig/"] = "全局";
    hot_update_type_map_["/skill/"] = "技能";
    hot_update_type_map_["itemmanager.xml|/item/"] = "物品";
    hot_update_type_map_["/drop/"] = "掉落";
    hot_update_type_map_["/monster/"] = "怪物";
    hot_update_type_map_["tasklist.xml"] = "任务";
    hot_update_type_map_["/logicconfig/"] = "逻辑";
    hot_update_type_map_["/shizhuang/"] = "时装";
    hot_update_type_map_["/door/|/map/|/scene/|scenemanager.xml"] = "场景";
    hot_update_type_map_["openserver.xml"] = "共享";

    operate_type_map_["热更新"] = OperateType::HOT_UPDATE;
    operate_type_map_["发版本"] = OperateType::NEW_VERSION;

    ui->setupUi(this);
    ui->tips_txt->setTextInteractionFlags(Qt::TextSelectableByMouse);
    this->setFixedWidth(800);
    this->setFixedHeight(360);
    this->SetUIEnable(false);

    this->ReadRecord();
}

MainWindow::~MainWindow()
{
    this->SaveRecord();
    delete ui;
}

void MainWindow::SetOperateData(Protocol::SCGORequestCommandListACK *ack)
{
    // clear combobox
    {
        int pName_count = ui->project_name->count();
        for (int index = 0; index < pName_count; ++ index)
            ui->project_name->removeItem(0);
        int oList_count = ui->operate_type->count();
        for (int index = 0; index < oList_count; ++ index)
            ui->operate_type->removeItem(0);
    }

    if (int(ack->project_list.size()) <= 0 || int(ack->operate_list.size()) <= 0)
        return;

    if (last_project_index_ < 0 || last_project_index_ >= (int)ack->project_list.size())
        last_project_index_ = 0;
    if (last_operate_index_ < 0 || last_operate_index_ >= (int)ack->operate_list.size())
        last_operate_index_ = 0;

    QStringList name_str_list;
    for (Protocol::SCGOCommandProjectItem item : ack->project_list)
        name_str_list<<item.project_name.c_str();
    ui->project_name->addItems(name_str_list);
    for (int index = 0; index < (int)ack->project_list.size(); ++ index)
        ui->project_name->model()->setData(ui->project_name->model()->index(index,0), QSize(25,25), Qt::SizeHintRole);

    QStringList opera_str_list;
    for (Protocol::SCGOCommandOperateItem item : ack->operate_list)
        opera_str_list<<item.operate_name.c_str();
    ui->operate_type->addItems(opera_str_list);
    for (int index = 0; index < (int)ack->operate_list.size(); ++ index)
        ui->operate_type->model()->setData(ui->operate_type->model()->index(index,0), QSize(25,25), Qt::SizeHintRole);

    ui->project_name->setCurrentIndex(last_project_index_);
    ui->operate_type->setCurrentIndex(last_operate_index_);

    if (ack->operate_list.size() > 0)
        ui->describe_txt->setText(ack->operate_list[last_operate_index_].operate_describe.c_str());

    project_list_ = ack->project_list;
    operate_list_ = ack->operate_list;

}

void MainWindow::AddOutputTxt(Protocol::SCGOCommandOutput*ack)
{
    for (std::string output_txt : ack->output_str_list)
    {
        ui->output_msg_txt->append(output_txt.c_str());
        QString output_qstr(output_txt.c_str());

        for (std::map<QString,QString>::iterator match_it = hot_update_type_map_.begin(); match_it != hot_update_type_map_.end(); ++ match_it)
        {
            QRegExp rx(match_it->first);
            bool match = rx.exactMatch(output_qstr);
            int pos = output_qstr.indexOf(rx);
            if (match || pos >= 0)
            {
                cur_hot_update_type_set_.insert(match_it->second);
            }
        }

        QRegExp version_rx("sh create_version.sh");
        if (output_qstr.indexOf(version_rx) >= 0)
        {
            QRegExp version_num("\\d{10}");
            if (version_num.indexIn(output_qstr) >= 0)
                cur_version_str_ = version_num.cap(0);
        }
    }
}

void MainWindow::SetTipsTxt(const QString &tips_str)
{
    ui->tips_txt->setText(tips_str);
}

void MainWindow::SetUIEnable(bool enable)
{
    ui->execute_btn->setEnabled(enable);
    ui->project_name->setEnabled(enable);
    ui->operate_type->setEnabled(enable);
}

void MainWindow::OperateSucc()
{
    QString tip_str = "操作完毕！";
    if (OperateType::HOT_UPDATE == cur_operate_type_)
    {
        tip_str += "热更";
        for (QString update_type : cur_hot_update_type_set_)
        {
            tip_str += update_type;
            tip_str += ",";
        }
        tip_str += "服务端版本号：";
        tip_str += cur_version_str_;
    }
    else if (OperateType::NEW_VERSION == cur_operate_type_)
    {
        tip_str += "发版，服务端版本号：";
        tip_str += cur_version_str_;
    }
    this->SetTipsTxt(tip_str);
}

void MainWindow::ReadRecord()
{
    QFile file_project_list("last_operate.txt");
    if (file_project_list.open(QFile::ReadOnly))
    {
        QTextStream in(&file_project_list);
        in >> last_project_index_ >> last_operate_index_;
    }
}

void MainWindow::SaveRecord()
{
    QFile file_project_list("last_operate.txt");
    if (file_project_list.open(QFile::WriteOnly | QIODevice::Truncate))
    {
        QTextStream out(&file_project_list);
        last_project_index_ = ui->project_name->currentIndex();
        last_operate_index_ = ui->operate_type->currentIndex();
        out << last_project_index_  << " " << last_operate_index_;
    }
}

void MainWindow::on_execute_btn_clicked()
{
    int project_index = ui->project_name->currentIndex();
    int operate_index = ui->operate_type->currentIndex();
    if (project_index < 0 || project_index >= (int)project_list_.size() || operate_index < 0 || operate_index >= (int)operate_list_.size())
    {
        //this->SetTipsTxt("请先");
        return;
    }

    auto type = operate_type_map_.find(ui->operate_type->currentText());
    if (type != operate_type_map_.end())
        cur_operate_type_ = type->second;
    else
        cur_operate_type_ = OperateType::DEV_SERVER_NORMAL;
    cur_hot_update_type_set_.clear();
    cur_version_str_ = "";

    Protocol::CSGORequestCommand cmd;
    cmd.project_type = project_list_[project_index].project_type;
    cmd.operate_type = operate_list_[operate_index].operate_type;
    NetworkAgent::GetInstance().SendToServer(cmd);

    this->SetTipsTxt("操作请求正在执行中，请耐心等候......");
    ui->output_msg_txt->clear();
    this->SetUIEnable(false);

    this->SaveRecord();
    //this->setFixedWidth(760);
}

void MainWindow::on_project_name_currentIndexChanged(int index)
{
    if (index < 0 || index >= (int)project_list_.size())
        return;
}

void MainWindow::on_operate_type_currentIndexChanged(int index)
{
    if (index < 0 || index >= (int)operate_list_.size())
        return;

    ui->describe_txt->setText(operate_list_[index].operate_describe.c_str());
}
