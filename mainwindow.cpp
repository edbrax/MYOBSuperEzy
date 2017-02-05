#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->spinFY->setRange(16, 50);
    ui->spinQuarter->setRange(1,4);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_getEmployeeFileButton_clicked()
{
    employeeFilePtr = GetFileController::getAFile(QString("Open the EMPLOYEE file for this quarter"));
}



/*
ofstream *contributionFilePtr;
ofstream *memberFilePtr;
ofstream *terminatedMemberFilePtr;
*/

void MainWindow::on_getSuperFileButton_clicked()
{
    myobSuperFilePtr = GetFileController::getAFile(QString("Open the SUPER file for this quarter"));
}


void MainWindow::on_spinFY_editingFinished()
{
    quarter = static_cast<uint8_t>(ui->spinQuarter->value());
}

void MainWindow::on_spinQuarter_editingFinished()
{
    quarter = static_cast<uint8_t>(ui->spinQuarter->value());
}
