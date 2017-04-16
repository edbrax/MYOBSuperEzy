#ifndef DATAFORMATTER_H
#define DATAFORMATTER_H

#include <QObject>
#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QStringList>
#include <QDate>
#include <QMessageBox>


class DataFormatter : public QWidget
{
    Q_OBJECT

public:
    explicit DataFormatter(QWidget *parent = nullptr);

    void setSuperannuationFile(QFile *filePtr);
    void setEmployeesFile(QFile * filePtr);
    void setDateDetails(int financialYear, int quarter);
    void writeContributionsFile(QFile *filePtr);
    void writeMembersFile(QFile *filePtr);


private:
    QWidget *parentptr;
    QFile *employeeFilePtr;
    QFile *myobSuperFilePtr;
    int financialYear;
    int quarter;

    bool isUnderEighteen(QStringList employeeDetails);
    QString getSurname(QString givenName, QString familyName);
    QMap<QString, QString> *getEmployeeMap();
};

#endif // DATAFORMATTER_H
