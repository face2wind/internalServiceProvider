#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

#include <map>

class QMainWindow;
class MainWindow;

class UIManager
{
public:
    ~UIManager();
    static UIManager & GetInstance();

    void ShowLogin();
    void ShowRegister();
    void ShowMainView();
    void OnLoginResult(int result);
    void ShowChatWindow(unsigned int user_id);

protected:
    void MoveCenter(QMainWindow *window);
    void MoveLeft(QMainWindow *window);

protected:
    UIManager();

    MainWindow *main_window_;

    int desktop_width_;
    int desktop_height_;
};

#endif // UI_MANAGER_HPP
