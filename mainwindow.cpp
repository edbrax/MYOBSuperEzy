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
    DataFileController dataFile;
    employeeFilePtr = dataFile.getAFile(QString("Open the EMPLOYEE file for this quarter"));
}

void MainWindow::on_getSuperFileButton_clicked()
{
    DataFileController dataFile;
    myobSuperFilePtr = dataFile.getAFile(QString("Open the SUPER file for this quarter"));
}


void MainWindow::on_writeContributionFileButton_clicked()
{
    if(!checkInputFilesPresent()) {
        return;
    }

    OutputFileController writeFile(this);
    contributionFilePtr = writeFile.outputFile(
                QString("Write the CONTRIBUTIONS file for this quarter"));

    if(!contributionFilePtr) return;

    DataFormatController dataFormat(this);
    dataFormat.setEmployeesFile(employeeFilePtr);
    dataFormat.setContributionsFile(myobSuperFilePtr);
    dataFormat.setDateDetails(financialYear, quarter);

    // Write out the data file
    dataFormat.writeContributionsFile(contributionFilePtr);

    contributionFilePtr->flush();
    contributionFilePtr->close();
}

void MainWindow::on_spinFY_editingFinished()
{
    financialYear = static_cast<uint8_t>(ui->spinFY->value());
}

void MainWindow::on_spinQuarter_editingFinished()
{
    quarter = static_cast<uint8_t>(ui->spinQuarter->value());
}


void MainWindow::on_writeMemberFileButton_clicked()
{
    if(!checkInputFilesPresent()) {
        return;
    }
    OutputFileController writeFile(this);
    memberFilePtr = writeFile.outputFile(
                QString("Write the MEMBERS file for this quarter"));
    if(!memberFilePtr) return;

    // Initialize the data formatter
    DataFormatController dataFormat(this);
    dataFormat.setEmployeesFile(employeeFilePtr);
    dataFormat.setDateDetails(financialYear, quarter);

    // Write out the data file
    dataFormat.writeMembersFile(memberFilePtr);

    memberFilePtr->flush();
    memberFilePtr->close();
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
