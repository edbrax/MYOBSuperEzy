#ifndef GETFILECONTROLLER_H
#define GETFILECONTROLLER_H

#include <QObject>
#include <QFileDialog>
#include <QWidget>
#include <QMessageBox>
#include <QFile>
#include <QString>

class FileController : public QWidget
{
    Q_OBJECT

public:
    explicit FileController(QWidget *parent = nullptr);

    QFile *openFile(QString prompt, QIODevice::OpenMode openMode);

private:
    QWidget *parentptr;

};



#endif // GETFILECONTROLLER_H
