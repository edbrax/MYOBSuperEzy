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

private:
    Ui::MainWindow *ui;
    ifstream *employeeFilePtr;
    ifstream *myobSuperFilePtr;
    ofstream *contributionFilePtr;
    ofstream *memberFilePtr;
    ofstream *terminatedMemberFilePtr;
};

#endif // MAINWINDOW_H
