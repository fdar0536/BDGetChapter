/*
Copyright (C) 2019 fdar0536
This file is part of BDGetChapter.
BDGetChapter is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
BDGetChapter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public License
along with BDGetChapter.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//qt
#include "QMainWindow"
#include "QCloseEvent"

namespace Ui {
class MainWindow;
}

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_bd_add_btn_clicked();
    
    void on_bd_remove_btn_clicked();
    
    void on_bd_clean_btn_clicked();
    
    void on_bd_broswe_btn_clicked();
    
    void on_bd_start_btn_clicked();
    
    void on_save_log_btn_clicked();
    
    void on_clean_log_btn_clicked();
    
    //gui built in action
    void on_actionAbout_QT_triggered();
    
private:
    Ui::MainWindow *ui;
    
    void closeEvent(QCloseEvent *e);
};

#endif // MAINWINDOW_H
