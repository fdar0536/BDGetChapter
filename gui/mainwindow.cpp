/*
Copyright (C) 2019 fdar0536
This file is part of BDGetChapter.
BDGetChapter is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
BDGetChapter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with BDGetChapter.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

//qt
#include "QApplication"
#include "QMessageBox"
#include "QFileDialog"
#include "QCloseEvent"

#include "utils.h"
#include "../common/bdgetchap.h"

#ifdef Q_OS_WIN32 
#include "Windows.h"
#endif

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//bdchap methods
void MainWindow::on_bd_add_btn_clicked()
{
    QString bd(QFileDialog::getExistingDirectory(this, "Open BD",
                                                            QDir::currentPath(),
                                                     QFileDialog::ShowDirsOnly
                                                   | QFileDialog::DontResolveSymlinks));
    
    if (bd == "")
    {
        return;
    }

    if (find_bdmv_list(ui->bdmv_list, bd) == false)
    {
        ui->bdmv_list->addItem(bd);
    }
    else
    {
        QString temp_string = set_date() + "Warning: \"" + bd + "\" is already in list.";
        ui->log_list->addItem(temp_string);
    }
    
    //check output path
    if (ui->bd_output_path->text() != "")
    {
        ui->bd_start_btn->setEnabled(true);
    }
}

void MainWindow::on_bd_remove_btn_clicked()
{
    qDeleteAll(ui->bdmv_list->selectedItems());
}

void MainWindow::on_bd_clean_btn_clicked()
{
    ui->bdmv_list->clear();
    ui->bd_start_btn->setEnabled(false);
}

void MainWindow::on_bd_broswe_btn_clicked()
{
    QString output_path(QFileDialog::getExistingDirectory(this, "Output dir", QDir::currentPath(),
                                                                QFileDialog::ShowDirsOnly
                                                              | QFileDialog::DontResolveSymlinks));
    
    ui->bd_output_path->setText(output_path);
    if ((output_path != "") && (ui->bdmv_list->count() != 0))
    {
        ui->bd_start_btn->setEnabled(true);
    }
}

void MainWindow::on_bd_start_btn_clicked()
{
    int index(0);
    for (index = 0; index < ui->bdmv_list->count(); ++index)
    {
        QStringList input_list = ui->bdmv_list->item(index)->text().split("/");
        
        string input = ui->bdmv_list->item(index)->text().toStdString();
        string output = (ui->bd_output_path->text() + "/" + input_list[input_list.length() - 1]).toStdString();
        try
        {
            bdgetchap_main_process(input, output);
        }
        catch (const char *e)
        {
            QString temp_string = set_date() + QString::fromLocal8Bit(e);
            ui->log_list->addItem(temp_string);
        }
    } //end for
    
    QMessageBox::information(this, "All done!", "All done!\nPlease check log tab for any error output.");
}

//log tab methods
void MainWindow::on_save_log_btn_clicked()
{
    QString save_path = QFileDialog::getSaveFileName(this, "Save log", QDir::currentPath(), "log(*.log)");
    if (save_path == "")
    {
        return;
    }
    
    FILE *log_file;
#ifdef Q_OS_WIN32
    fopen_s(&log_file, save_path.toStdString().c_str(), "w");
#else
    log_file = fopen(save_path.toStdString().c_str(), "w");
#endif
    int index(0);
    for (index = 0; index < ui->log_list->count(); ++index)
    {
        fputs((ui->log_list->item(index)->text().toStdString() + "\n").c_str(), log_file);
    }
    
    fclose(log_file);
    QMessageBox::information(this, "Done.", "All done.");
}

void MainWindow::on_clean_log_btn_clicked()
{
    ui->log_list->clear();
}

//gui built in action
void MainWindow::on_actionAbout_QT_triggered()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    QMessageBox::StandardButton res = QMessageBox::question(this, "Exit", "Are you sure to exit?",
                                                            QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes);
    
    if (res == QMessageBox::Yes)
    {
        QApplication::quit();
    }
    else
    {
        e->ignore();
    }
}