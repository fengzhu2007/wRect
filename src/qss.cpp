#include "qss.h"


QString QSS::global()
{
    //vs2019 light
    //menu bakcground #f6f6f6 border:#cccedb color:#1e1e1e
    //menu item hover #c9def5
    QString qss = ".wMainWindow{background-color:white;}"
                  "QFrame#background{background-color:white}"
                  ".QMenuBar{padding:4px;border:0;background:#EEEEF2;}"
                  ".QMenuBar#menu_placeholder{height:32px;}"
                  ".QMenuBar::item:selected{background:#c9def5}"
                  ".QMenuBar::item:pressed {background:#f6f6f6;border:1px solid #cccedb;border-bottom:0;}"
                  ".QMenuBar QMenu{background:#f6f6f6;border:1px solid #cccedb;color:#1e1e1e;}"
                  ".QMenuBar QMenu::item:selected{background-color:#c9def5}"
                  ".QToolBar:top{background-color:#EEEEF2;border:0;}"
                  ".QToolBar::icon{padding:5px}"
                  ".QToolBar::handle{height:16px;width:20px;image:url(':/images/toolbar_handle.png')}"
                  ".QStatusBar{background-color:#007acc}"
                  ".wSystemButton{background-color: transparent; border: none;width:46px;height:32px;}"
                  ".wSystemButton:hover{background-color:#fff;}"
                  ".wSystemButton:pressed{background-color:#007acc;}"
                  ".wWindowNClient{background-color:#EEEEF2;}"
                  ".wWindowNClient>QLabel#logo{width:36px;height:32px;}";
    return qss;
}



