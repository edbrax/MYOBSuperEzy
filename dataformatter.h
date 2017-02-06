#ifndef DATAFORMATTER_H
#define DATAFORMATTER_H

#include <QObject>
#include <QWidget>
#include <QFile>


class DataFormatter : public QWidget
{
    Q_OBJECT

public:
    explicit DataFormatter(QWidget *parent = nullptr);

    void setSuperannuationFile(QFile *filePtr);
    void setEmployeesFile(QFile * filePtr);
    void setDateDetails(int financialYear, int quarter);
    /*
    void writeContributionsFile(QFile *filePtr);
    void writeMembersFile(QFile *filePtr);
*/

private:
    QWidget *parentptr;
    QFile *employeeFilePtr;
    QFile *myobSuperFilePtr;
    int financialYear;
    int quarter;
};

#endif // DATAFORMATTER_H
