#include "ui_manager.hpp"

#include <QApplication>
#include <QDesktopWidget>

#include "mainwindow.h"

#include "message_code.hpp"
#include "network/networkagent.h"

#include "cs_protocol_def.hpp"

using namespace Protocol;

UIManager::UIManager()
{
    main_window_ = new MainWindow();

    QDesktopWidget *desktop = QApplication::desktop();
    //desktop_width_= desktop->width();
    //desktop_height_= desktop->height();

    QRect screen_rect = desktop->availableGeometry(desktop->primaryScreen());
    desktop_width_= screen_rect.width();
    desktop_height_ = screen_rect.height();
}

UIManager::~UIManager()
{

}

UIManager & UIManager::GetInstance()
{
    static UIManager instance;
    return instance;
}

void UIManager::ShowLogin()
{

}

void UIManager::ShowRegister()
{
}

void UIManager::ShowMainView()
{
    main_window_->SetData();
    main_window_->show();
}

void UIManager::OnLoginResult(int result)
{
}

void UIManager::ShowChatWindow(unsigned int user_id)
{
}

void UIManager::MoveCenter(QMainWindow *window)
{
    if (nullptr != window)
        window->move((desktop_width_ - window->width())/2,(desktop_height_ - window->height())/2);
}

void UIManager::MoveLeft(QMainWindow *window)
{
    if (nullptr != window)
        window->move(100,(desktop_height_ - window->height())/2);
}
