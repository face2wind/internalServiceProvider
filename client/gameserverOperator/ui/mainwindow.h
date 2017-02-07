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

protected:

private:

private slots:
    void on_execute_btn_clicked();

private:
    Ui::ChatWindow *ui;
};

#endif // CHATWINDOW_H
