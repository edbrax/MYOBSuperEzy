#include "filecontroller.h"


FileController::FileController(QWidget *parent) : QWidget(parent), parentptr(parent)
{

}


QFile *FileController::openFile(QString prompt, QIODevice::OpenMode openMode) {

    QFile *filePtr = new QFile(parentptr);

    if(openMode == QIODevice::ReadOnly)
        filePtr->setFileName(QFileDialog::getOpenFileName(parentptr,
                              QString("Select %1 file").arg(prompt),
                               "/", "Text (*.txt)"));
    else
        filePtr->setFileName(QFileDialog::getSaveFileName(parentptr,
                              QString("Select %1 file").arg(prompt),
                               "/", "CSV (*.csv)"));

     if(filePtr->fileName().isNull()) return nullptr;

     filePtr->open(openMode);

     if(!filePtr->isOpen()){
         QMessageBox::warning(parentptr, QString("MLC SuperEzy Converter"), QString("%1 file not open.").arg(prompt));
         return nullptr;
     }

     return filePtr;
}
