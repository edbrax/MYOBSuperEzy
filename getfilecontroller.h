#ifndef GETFILECONTROLLER_H
#define GETFILECONTROLLER_H

#include <QObject>
#include <QFileDialog>
#include <QWidget>
#include <QMessageBox>
#include <iostream>
#include <fstream>
#include <QString>

class GetFileController : public QObject
{

    Q_OBJECT

public:
    explicit GetFileController(QObject *parent = 0);

    static std::ifstream *getAFile(QString prompt);

private:
    QWidget *parentptr;

};



#endif // GETFILECONTROLLER_H
