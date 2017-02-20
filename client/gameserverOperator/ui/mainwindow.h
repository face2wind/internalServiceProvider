#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <deque>
#include "cs_protocol_def.hpp"

namespace Ui {
class ChatWindow;
}

struct ChatMessageItem
{
    unsigned int sender_id;
    unsigned int receiver_id;
    std::string message;
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

private slots:
    void on_execute_btn_clicked();

    void on_project_name_currentIndexChanged(int index);

    void on_operate_type_currentIndexChanged(int index);

private:
    Ui::ChatWindow *ui;

    std::vector<Protocol::SCGOCommandProjectItem> project_list_;
    std::vector<Protocol::SCGOCommandOperateItem> operate_list_;
};

#endif // CHATWINDOW_H
