#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <QString>
#include <QFileDialog>
#include <QWidget>
#include <QMessageBox>

#include "inputfilecontroller.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_getEmployeeFileButton_clicked();
    void on_getSuperFileButton_clicked();
    void on_spinQuarter_editingFinished();
    void on_spinFY_editingFinished();

private:
    Ui::MainWindow *ui;
    std::ifstream *employeeFilePtr;
    std::ifstream *myobSuperFilePtr;
    std::ofstream *contributionFilePtr;
    std::ofstream *memberFilePtr;
    std::ofstream *terminatedMemberFilePtr;
    uint8_t financialYear;
    uint8_t quarter;
};

#endif // MAINWINDOW_H
