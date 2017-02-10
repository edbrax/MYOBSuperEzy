#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QString>
#include <QWidget>
#include <QMessageBox>
#include <QIODevice>

#include "filecontroller.h"
#include "dataformatter.h"

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
    void on_writeContributionFileButton_clicked();
    void on_writeMemberFileButton_clicked();

private:
    Ui::MainWindow *ui;
    QFile *employeeFilePtr;
    QFile *myobSuperFilePtr;
    QFile *contributionFilePtr;
    QFile *memberFilePtr;
    QFile *terminatedMemberFilePtr;
    uint8_t financialYear;
    uint8_t quarter;
    uint8_t filesFlag;

    bool checkInputFilesPresent();
    void outputFile(QFile *filePtr, QString prompt);
};

#endif // MAINWINDOW_H
