#include "filecontroller.h"


GetFileController::GetFileController(QObject *parent) : QObject(parent)
{

}


std::ifstream *GetFileController::getAFile(QString prompt) {


    QString fileName = QFileDialog::getOpenFileName(nullptr, prompt);

     if(fileName.isNull()){
         return nullptr;
     }

     std::ifstream *filePtr = new std::ifstream(fileName.toLocal8Bit().constData());

     if(filePtr->fail()){
         QMessageBox::warning(nullptr, QString("MLC SuperEzy Converter"), QString("Failed to open the %1 file.").arg(fileName));
         return nullptr;
     }

     return filePtr;
}



