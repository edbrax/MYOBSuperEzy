#include "dataformatter.h"

DataFormatter::DataFormatter(QWidget *parent) : QWidget(parent), parentptr(parent)
{

}


void DataFormatter::setSuperannuationFile(QFile *filePtr) {
    myobSuperFilePtr = filePtr;
}

void DataFormatter::setEmployeesFile(QFile * filePtr){
    employeeFilePtr = filePtr;
}


void DataFormatter::setDateDetails(int financYear, int quartr){
    financialYear = financYear;
    quarter = quartr;
}
    /*
void DataFormatter::writeContributionsFile(QFile *filePtr){

}

void DataFormatter::writeMembersFile(QFile *filePtr){

}

*/

