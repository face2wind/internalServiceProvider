#-------------------------------------------------
#
# Project created by QtCreator 2016-09-05T19:48:41
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gameserverOperator
TEMPLATE = app

INCLUDEPATH += ../../common

SOURCES += main.cpp\
    ui/ui_manager.cpp \
    network/msg_handler.cpp \
    network/networkagent.cpp \
    ui/mainwindow.cpp \
    ../../common/protocol/cs_protocol_def.cpp \
    elegance/common/debug_message.cpp \
    elegance/common/random.cpp \
    elegance/common/tools.cpp \
    elegance/memory/byte_array/queue_byte_array.cpp \
    elegance/memory/serialize/serialize_base.cpp \
    elegance/memory/serialize/serialize_manager.cpp \
    elegance/network/network_packer/network_packer_fixhead.cpp \
    elegance/network/network_manager.cpp \
    elegance/network/network_packer_factory.cpp \
    elegance/platform/network/socket_accept.cpp \
    elegance/platform/network/socket_connect.cpp

HEADERS  += \
    ../../common/message_code.hpp \
    ../../common/protocol/cs_protocol_def.hpp \
    ui/ui_manager.hpp \
    network/msg_handler.h \
    network/networkagent.h \
    ../../common/commondef.hpp \
    ui/mainwindow.h \
    client_def.hpp \
    elegance/common/debug_message.hpp \
    elegance/common/random.hpp \
    elegance/common/tools.hpp \
    elegance/memory/byte_array/byte_array.hpp \
    elegance/memory/byte_array/queue_byte_array.hpp \
    elegance/memory/serialize/serialize_base.hpp \
    elegance/memory/serialize/serialize_manager.hpp \
    elegance/memory/flexible_buffer.hpp \
    elegance/network/network_packer/network_packer_fixhead.hpp \
    elegance/network/network_manager.hpp \
    elegance/network/network_packer_factory.hpp \
    elegance/platform/network/socket_accept.hpp \
    elegance/platform/network/socket_connect.hpp \
    elegance/platform/network/socket_def.hpp \
    elegance/elegance.hpp

FORMS    += \
    ui/mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11

RESOURCES +=

DISTFILES += \
    common/protocol_def.xml \
    elegance/CanNotCoverFiles.txt
