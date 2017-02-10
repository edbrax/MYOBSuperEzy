#include "dataformatter.h"

DataFormatter::DataFormatter(QWidget *parent) : QWidget(parent), parentptr(parent)
{

}

// Sets the DataFormatter Class scope pointer to the MYOB file
// with data on Superannuation payments owed to each employee
void DataFormatter::setSuperannuationFile(QFile *filePtr) {
    myobSuperFilePtr = filePtr;
}

// Sets the DataFormatter Class scope pointer to the MYOB file
// with the Employee general data
void DataFormatter::setEmployeesFile(QFile * filePtr){
    employeeFilePtr = filePtr;
}

// Sets the values for relevant FY and Quarter for the calculation
void DataFormatter::setDateDetails(int financYear, int quartr){
    financialYear = financYear;
    quarter = quartr;
}

// Writes a CSV file with superannuation payment data to send to
// MLC. This file gives SupererEzy details on Member Contributions
// to be paid.
void DataFormatter::writeContributionsFile(QFile *filePtr){

}


// Writes a CSV file with member data for sending to MLC.
// Use this file to add new members to SuperEzy or to amend
// members details.
void DataFormatter::writeMembersFile(QFile *filePtr){
    QTextStream membersText(filePtr);
    QTextStream employeeDetailsStream(employeeFilePtr);
    QStringList employeeDetailsLine = employeeDetailsStream.readLine().split("\t", QString::SkipEmptyParts);
    float hours;

    // Output file headings (first line of CSV file).
    membersText << "Fund Code,MemberNo,Title,First,Last,Street1,"
            << "Street2,Suburb,State,Postcode,Country,"
            << "Mobile,Email,Gender,CollarType,EmploymentStatus,"
            << "Paygroup,DOB,DateJoinedEmp,EmpCeaseDate,"
            << "EmpCeaseReason,PayrollNo,TFN,Salary" << endl << endl;

    // Loop through MYOB employees .txt file to get each employee's data.
    while(!employeeDetailsStream.atEnd()) {
        employeeDetailsLine = employeeDetailsStream.readLine().split("\t");

        // Disregard employees with no CardID, inactive cards or Under18s
        if(employeeDetailsLine.at(2).trimmed() == "*None" ||
                employeeDetailsLine.at(3).trimmed() == "Y" ||
                isUnderEighteen(employeeDetailsLine)) continue;

        // Output details
        membersText << employeeDetailsLine.at(118).trimmed() << ","
                    << employeeDetailsLine.at(119).trimmed() << ","
                    << (employeeDetailsLine.at(110).trimmed() == "M" ? "Mr," : "Ms,")
                    << employeeDetailsLine.at(1).trimmed() << ","
                    << getSurname(employeeDetailsLine.at(1).trimmed(), employeeDetailsLine.at(0).trimmed()) << ",";

        // Best street address
        if(employeeDetailsLine.at(5).contains("PO Box", Qt::CaseInsensitive)
                && employeeDetailsLine.at(20).length() > 0)
            membersText << employeeDetailsLine.at(20).trimmed() << ","
                        << employeeDetailsLine.at(21).trimmed() << ","
                        << employeeDetailsLine.at(24).trimmed() << ","
                        << employeeDetailsLine.at(25).trimmed() << ","
                        << employeeDetailsLine.at(26).trimmed() << ","
                        << (employeeDetailsLine.at(27).trimmed().length() > 0 ?
                            employeeDetailsLine.at(27).trimmed() : "Australia")
                        << ",";
        else
            membersText << employeeDetailsLine.at(5).trimmed() << ","
                        << employeeDetailsLine.at(6).trimmed() << ","
                        << employeeDetailsLine.at(9).trimmed() << ","
                        << employeeDetailsLine.at(10).trimmed() << ","
                        << employeeDetailsLine.at(11).trimmed() << ","
                        << (employeeDetailsLine.at(12).trimmed().length() > 0 ?
                            employeeDetailsLine.at(12).trimmed() : "Australia")
                        << ",";


        // Mobile Number
        membersText << (employeeDetailsLine.at(13).startsWith("04") ?
                            QString("%1,").arg(employeeDetailsLine.at(13).trimmed()) :
                       (employeeDetailsLine.at(14).startsWith("04") ?
                            QString("%1,").arg(employeeDetailsLine.at(14).trimmed()) : ","));
        // email
        membersText << (employeeDetailsLine.at(133).trimmed().length() > 0 ?
                            QString("%1,").arg(employeeDetailsLine.at(133).trimmed()) :
                       (employeeDetailsLine.at(17).trimmed().length() > 0 ?
                           QString("%1,").arg(employeeDetailsLine.at(17).trimmed())  : ","));

        // Gender
        membersText << employeeDetailsLine.at(110).trimmed()  << ",";

        // Collartype
        if(employeeDetailsLine.at(108).contains("Warehouse") ||
               employeeDetailsLine.at(108).contains("Cleaner"))
            membersText << "Light Blue,";
        else
            membersText << "White,";

        // Calculate employment status
        hours = employeeDetailsLine.at(128).toFloat();
        if(hours < 0.1) hours = 37.5;

        if(employeeDetailsLine.at(128).trimmed() == "C")
            membersText << "Casual,";
        else if(hours < 15)
            membersText << "Permanent Part Time < 15hrs per week,";
        else if(hours < 32)
            membersText << "Permanent Part Time >= 15hrs per week,";
        else
            membersText << "Permanent Full Time,";

        // Paygroup, DOB, Date Joined, Date Ceased
        membersText << "1,"
                    << employeeDetailsLine.at(109).trimmed() << ","
                    << employeeDetailsLine.at(111).trimmed() << ","
                    << employeeDetailsLine.at(112).trimmed() << ",";

        // Reason if terminated
        if(employeeDetailsLine.at(112).trimmed().length() > 0)
            membersText << "Left Employment,";
        else
            membersText << ",";

        // Payroll Number, TFN
        membersText << employeeDetailsLine.at(2).trimmed() << ","
                    << employeeDetailsLine.at(120).trimmed() << ",";

        // Calculate salary
        if(employeeDetailsLine.at(113).trimmed() == "H")
            membersText << qRound(employeeDetailsLine.at(114).toFloat() *
                            employeeDetailsLine.at(116).toFloat() * 52) << endl;
        else
            membersText << employeeDetailsLine.at(114).toInt() << endl;


    } // !employeeDetailsStream.atEnd()
    QMessageBox::warning(this, QString("MLC SuperEzy Converter"),
        QString("Members file completed."));
}

// Returns true if the employee is under 18, false if over 18
// or if an error occurs in the calculation.
bool DataFormatter::isUnderEighteen(QStringList employeeDetails){

    QDate eoqDate;
    QString dobString = employeeDetails.at(109).trimmed();
    QDate dobDate(dobString.split("/").at(2).toInt(),
                  dobString.split("/").at(1).toInt(),
                  dobString.split("/").at(0).toInt());

    if(!dobDate.isValid()) {
        QMessageBox::warning(this, QString("MLC SuperEzy Converter"),
            QString("Error in detecting age for %1 %2.")
            .arg(employeeDetails.at(1)).arg(employeeDetails.at(0)));
        return false;
    }

    switch(quarter) {
        case 1 :
            eoqDate.setDate((financialYear + 1999),9,30);
            break;
        case 2 :
            eoqDate.setDate((financialYear + 1999),12,31);
            break;
        case 3 :
            eoqDate.setDate((financialYear + 2000),3,31);
            break;
        case 4 :
            eoqDate.setDate((financialYear + 2000),6,30);
            break;
        default :
            QMessageBox::warning(this, QString("MLC SuperEzy Converter"),
                QString("Error in detecting quarter date for %1 %2.")
                .arg(employeeDetails.at(1)).arg(employeeDetails.at(0)));
            return false;
    }

    // Is the employee under 18 at the end of this quarter?
    return (dobDate.addYears(18) >= eoqDate);
}

// Very creaky way to adjust for married names
QString DataFormatter::getSurname(QString givenName, QString familyName) {

    if(givenName == "Stephanie" && familyName == "Roughton")
        return "McKillop";
    else if(givenName == "Rose" && familyName == "Stone")
        return "Hannigan";
    else if(givenName == "Jacqueline" && familyName == "Jones")
       return "Vega";
    else
       return familyName;
}
