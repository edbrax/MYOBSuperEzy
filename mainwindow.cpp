#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->spinFY->setRange(16, 50);
    ui->spinQuarter->setRange(1,4);
    employeeFilePtr = nullptr;
    myobSuperFilePtr = nullptr;
    contributionFilePtr = nullptr;
    memberFilePtr = nullptr;
    terminatedMemberFilePtr = nullptr;
    financialYear = 0;
    quarter = 0;
}

MainWindow::~MainWindow()
{
    if(employeeFilePtr) {
        if(employeeFilePtr->isOpen())
            employeeFilePtr->close();
    }

    if(myobSuperFilePtr) {
        if(myobSuperFilePtr->isOpen())
            myobSuperFilePtr->close();
    }

    if(contributionFilePtr) {
        if(contributionFilePtr->isOpen()) {
            contributionFilePtr->flush();
            contributionFilePtr->close();
        }
    }

    if(memberFilePtr) {
        if(memberFilePtr->isOpen()) {
            memberFilePtr->flush();
            memberFilePtr->close();
        }
    }

    if(terminatedMemberFilePtr) {
        if(terminatedMemberFilePtr->isOpen()) {
            terminatedMemberFilePtr->flush();
            terminatedMemberFilePtr->close();
        }
    }

    delete ui;
}

void MainWindow::on_getEmployeeFileButton_clicked()
{
    FileController dataFile(this);
    employeeFilePtr = dataFile.openFile(QString("Open the EMPLOYEE file for this quarter"), QIODevice::ReadOnly);
    if(!employeeFilePtr->isOpen()) return;
    filesFlag |= 1;
    ui->writeMemberFileButton->setEnabled(true);
    ui->writeMemberFileButton->setEnabled(true);
}

void MainWindow::on_getSuperFileButton_clicked()
{
    FileController dataFile(this);
    myobSuperFilePtr = dataFile.openFile(QString("Open the SUPER file for this quarter"), QIODevice::ReadOnly);
    if(!myobSuperFilePtr->isOpen()) return;
    filesFlag |= 2;
    if(filesFlag & 1)
        ui->writeContributionFileButton->setEnabled(true);
}


void MainWindow::on_writeContributionFileButton_clicked()
{
    outputFile(contributionFilePtr, QString("Write the CONTRIBUTIONS file for this quarter"));
}

void MainWindow::on_writeMemberFileButton_clicked()
{
    outputFile(memberFilePtr, QString("Write the MEMBERS file for this quarter"));
}

void MainWindow::on_spinFY_editingFinished()
{
    financialYear = static_cast<uint8_t>(ui->spinFY->value());
}

void MainWindow::on_spinQuarter_editingFinished()
{
    quarter = static_cast<uint8_t>(ui->spinQuarter->value());
}

bool MainWindow::checkInputFilesPresent() {

    if(!employeeFilePtr) {
        QMessageBox::warning(this, QString("MLC SuperEzy Converter"),
            QString("Employee File has not been opened."));
        return false;
    }

    if(!myobSuperFilePtr) {
        QMessageBox::warning(this, QString("MLC SuperEzy Converter"),
            QString("Super Contributions File has not been opened."));
        return false;
    }

    if(financialYear == 0 || quarter == 0) {
        QMessageBox::warning(this, QString("MLC SuperEzy Converter"),
            QString("Please set Financial Year and Quarter."));
        return false;
    }
    return true;
}

void MainWindow::outputFile(QFile *filePtr, QString prompt) {

    if(!checkInputFilesPresent()) {
        return;
    }

    FileController writeFile(this);
    filePtr = writeFile.openFile(
                prompt, QIODevice::WriteOnly);

    if(!filePtr) return;
/*
    DataFormatter dataFormat(this);
    dataFormat.setEmployeesFile(employeeFilePtr);
    dataFormat.setDateDetails(financialYear, quarter);

    // Write out the data file
    if(prompt.contains("CONTRIBUTIONS")) {
        dataFormat.setSuperannuationFile(myobSuperFilePtr);
        dataFormat.writeContributionsFile(filePtr);
    } else {
        dataFormat.writeMembersFile(filePtr);
    }
*/
    filePtr->flush();
    filePtr->close();
}
