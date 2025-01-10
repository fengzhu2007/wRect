#include "w_qss.h"

wQSS* wQSS::instance = nullptr;



wQSS::wQSS(Theme theme):m_theme(theme){

}


void wQSS::init(Theme theme){
    if(instance==nullptr){
        instance = new wQSS(theme);
    }
}

void wQSS::destory(){
    if(instance){
        delete instance;
        instance = nullptr;
    }
}

int wQSS::theme(){
    if(instance){
        return instance->m_theme;
    }else{
        return Light;
    }
}

QString wQSS::global()
{
    if(instance==nullptr || instance->m_theme==Light){
        //vs2019 light
        //menu bakcground #f6f6f6 border:#cccedb color:#1e1e1e
        //menu item hover #c9def5
        QString qss = ".wMainWindow{background-color:white;}"
                      ".QFrame#background{background-color:white}"
                      ".QToolBar{background:#EEEEF2;}"
                      ".QMenuBar{padding:5px 4px 0px 4px;border:0;background:#EEEEF2;}"
                      ".QMenuBar#menu_placeholder{height:32px;}"
                      ".QMenuBar::item:selected{background:#c9def5}"
                      ".QMenuBar::item:pressed {background:#f6f6f6;border:1px solid #cccedb;border-bottom:0;}"

                      ".QToolBar:top{background-color:#EEEEF2;border:0;}"
                      ".QToolBar::icon{padding:5px}"
                      ".QToolBar::handle{height:16px;width:20px;image:url(':/images/toolbar_handle.png')}"
                      ".QStatusBar{background-color:#007acc;}"
                      ".wSystemButton{background-color: transparent; border: none;width:46px;height:32px;}"
                      ".wSystemButton:hover{background-color:#fff;}"
                      ".wSystemButton:pressed{background-color:#007acc;}"
                      ".wWindowNClient{background-color:#EEEEF2;}"
                      ".wWindowNClient>QLabel#system_logo{width:36px;height:32px;}"
                      ".wWindowNClient>QLabel#system_title{height:32px;padding:0 4px 0 4px}"
                      ".wProgressBar>QFrame#bar{background-color:#007acc}";
        return qss;
    }else{
        //vs2019 dark
        QString qss =
            ".QFrame#background{background-color:#2d2d30}"
            ".QToolBar{background:#2d2d30;}"
            ".QMenuBar{padding:5px 4px 0px 4px;border:0;background:#2d2d30}"
            ".QMenuBar#menu_placeholder{height:32px;}"

            ".QMenuBar::item{background-color:#2d2d30;color:white}"
            ".QMenuBar::item:selected{background:#494949}"
            ".QMenuBar::item:pressed {background:#494949;border:1px solid #494949;border-bottom:0;}"
            ".QMenu{background-color:#2d2d30;color:white;border:1px solid #494949;padding:2px}"
            ".QMenu::item:selected{background-color:#494949;color:white;}"

            ".QToolBar:top{background-color:#2d2d30;border:0;}"
            ".QToolBar::icon{padding:5px}"
            ".QToolBar::handle{height:16px;width:20px;image:url(':/images/toolbar_handle.png')}"
            ".QStatusBar{background-color:#007acc;}"
            ".wSystemButton{background-color: transparent; border: none;width:46px;height:32px;padding:0;margin:0;min-width:0;}"
            ".wSystemButton:hover{background-color:#494949;}"
            ".wSystemButton:pressed{background-color:#007acc;}"
            ".wWindowNClient{background-color:#2d2d30;}"
            ".wWindowNClient#dialog_nclient{border-bottom:1px solid #494949}"
            ".wWindowNClient>QLabel#system_logo{width:36px;height:32px;}"
            ".wWindowNClient>QLabel#system_title{height:32px;padding:0 4px 0 4px;color:white;}"
            ".wProgressBar>QFrame#bar{background-color:#007acc}";
        return qss;
    }

}




