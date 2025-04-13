QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
RC_ICONS = logo.ico
DESTDIR = ./bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chat_form.cpp \
    config_manager.cpp \
    friend_list_form.cpp \
    global.cpp \
    http_manager.cpp \
    login_dialog.cpp \
    main.cpp \
    main_interface_form.cpp \
    main_window.cpp \
    recent_message_list_form.cpp \
    register_dialog.cpp \
    tcp_manager.cpp \
    user.cpp

HEADERS += \
    chat_form.h \
    config_manager.h \
    friend_list_form.h \
    global.h \
    http_manager.h \
    login_dialog.h \
    main_interface_form.h \
    main_window.h \
    recent_message_list_form.h \
    register_dialog.h \
    singleton.h \
    tcp_manager.h \
    user.h

FORMS += \
    chat_form.ui \
    friend_list_form.ui \
    login_dialog.ui \
    main_interface_form.ui \
    main_window.ui \
    recent_message_list_form.ui \
    register_dialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    app_config.json

# 将 app_config.json 拷贝到构建输出目录
ConfigFile = $${PWD}/app_config.json
CopyConfigCommand = copy "$$ConfigFile" "$${OUT_PWD}/$$DESTDIR"
# windows下必须将/替换为\\，否则会构建失败！！！
CopyConfigCommand = $$replace(CopyConfigCommand, /, \\)
QMAKE_POST_LINK += $$CopyConfigCommand
