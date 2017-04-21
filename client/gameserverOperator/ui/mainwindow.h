#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <deque>
#include <set>
#include "protocol/cs_protocol_def.hpp"

namespace Ui {
class ChatWindow;
}

struct ChatMessageItem
{
    unsigned int sender_id;
    unsigned int receiver_id;
    std::string message;
};

enum class OperateType
{
    DEV_SERVER_NORMAL,
    HOT_UPDATE,
    NEW_VERSION,
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void SetOperateData(Protocol::SCGORequestCommandListACK *ack);
    void AddOutputTxt(Protocol::SCGOCommandOutput*ack);
    void SetTipsTxt(const QString &tips_str);
    void SetUIEnable(bool enable);
    void OperateSucc();

protected:
    void ReadRecord();
    void SaveRecord();

private slots:
    void on_execute_btn_clicked();

    void on_project_name_currentIndexChanged(int index);

    void on_operate_type_currentIndexChanged(int index);

private:
    Ui::ChatWindow *ui;

    std::vector<Protocol::SCGOCommandProjectItem> project_list_;
    std::vector<Protocol::SCGOCommandOperateItem> operate_list_;

    std::set<QString> cur_hot_update_type_set_;
    std::map<QString,QString> hot_update_type_map_;
    std::map<QString,OperateType> operate_type_map_;

    OperateType cur_operate_type_;

    QString cur_version_str_;

    int last_project_index_;
    int last_operate_index_;
};

#endif // CHATWINDOW_H
