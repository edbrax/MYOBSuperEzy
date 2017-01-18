#include "mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::on_getEmployeeFileButton_clicked()
{

    // QString dialogName = "Open Employee file for this quarter";
    QString fileName = QFileDialog::getOpenFileName(this);

     if(fileName.isNull()){
         return nullptr;
     }

     ifstream *fileptr = new ifstream(fileName.toLocal8Bit().constData());

     if(fileptr->fail()){
         cout << "Could not open the required EMPDUPE file" << endl;
         exit(EXIT_FAILURE);
     }
}
