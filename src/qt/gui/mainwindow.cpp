#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "keyboard.h"

#include <QtMultimedia/QSound>

#include <QDebug>

#include <windows.h>

#include <stdlib.h>
#include <iostream>

HHOOK hKeyboardHook;
Keyboard kb = Keyboard();

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;

    if (nCode == HC_ACTION)
    {
        switch (wParam)
        {
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
                qDebug() << "Keydown | scanCode: " << p->scanCode;
                kb.play(p->scanCode, Keyboard::PRESSED);
                break;
            case WM_KEYUP:
            case WM_SYSKEYUP:
                qDebug() << "Keyup | scanCode: " << p->scanCode;
                kb.play(p->scanCode, Keyboard::RELEASED);
                break;
        }
    }

    CallNextHookEx(NULL, nCode, wParam, lParam);
}




void PrintFullPath( char * partialPath )
{
   char full[_MAX_PATH];
   if( _fullpath( full, partialPath, _MAX_PATH ) != NULL )
      qDebug() <<  full;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, nullptr, 0);
    /*
    std::string asd = ".";
    const char * c = asd.c_str();
    PrintFullPath(".");*/
}

MainWindow::~MainWindow()
{
    delete ui;
    UnhookWindowsHookEx(hKeyboardHook);
}
