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
    QTextStream contributionText(filePtr);
    QTextStream contributionsStream(myobSuperFilePtr);
    QStringList contributionsLine;
    QString strFund, strName, strSGC;
    QString strSalSacrificeAmount, strEmpAddAmount, strMemberVolAmount;
    QMap<QString, QString> *employeeMap = getEmployeeMap();
    int i;

    // Output file headings (first line of CSV file).
    contributionText << "Employee Name,Fund Code,Member Number,Payroll Number,"
                     << "SGC Amount,Salary Sacrifice Amount,"
            << "Employer Additional Amount,Member Voluntary Amount"
            << endl << endl;

    // Loop through MYOB file header lines
    for(i = 0; i < 11; i++) {
       contributionsStream.readLine();
    }

    // Loop through MYOB contributions .txt file. Note this file is NOT
    // a neatly formatted flat data file.
    contributionsLine = contributionsStream.readLine().split("\t");
    while(!contributionsStream.atEnd()) {

 //       QMessageBox::warning(this, QString("MLC SuperEzy Converter"),
 //           QString("Got here!"));

        if(contributionsLine.at(0).trimmed() == QString("Superannuation Fund:")) {

            // First, get the fund name (second item at "Superannuation Fund:")
            strFund = contributionsLine.at(1).trimmed();

            // Next, go to next line
            contributionsLine = contributionsStream.readLine().split("\t");

            // Loop through MYOB contributions file
            while(!contributionsStream.atEnd()) {

                // If the current line is the next fund, leave this loop
                if(contributionsLine.at(0).trimmed() ==
                        QString("Superannuation Fund:")) break;

                // If the current line is an employee name, process it.
                // Only employee names have a comma in the first item.
                if(contributionsLine.at(0).contains(",")) {

                    // Some surnames are different, getSurname() is the fix
                    strName = QString("%1, %2")
                        .arg(getSurname(contributionsLine.at(0).split(",").at(1).trimmed(),
                                        contributionsLine.at(0).split(",").at(0).trimmed()))
                        .arg(contributionsLine.at(0).split(",").at(1).trimmed());

                    // Initialize the amount strings to zero
                    strSGC = strSalSacrificeAmount =
                     strEmpAddAmount = strMemberVolAmount = QString("$0.00");

                    // Get the next line
                    contributionsLine = contributionsStream.readLine().split("\t");

                    // loop until all amounts are captured
                    if(contributionsLine.count() > 2) {
                        while(contributionsLine.at(2).contains("$")) {

                            // Record each applicable payment amount
                            if(contributionsLine.at(1).trimmed() == QString("Salary Sacrifice"))
                                strSalSacrificeAmount = QString("\"%1\"").arg(contributionsLine.at(2).trimmed());
                            else if(contributionsLine.at(1).trimmed() == QString("Superannuation Guarantee"))
                                strSGC = QString("\"%1\"").arg(contributionsLine.at(2).trimmed());
                            else if(contributionsLine.at(1).trimmed() == QString("Employer Additional"))
                                strEmpAddAmount = QString("\"%1\"").arg(contributionsLine.at(2).trimmed());
                            else if(contributionsLine.at(1).trimmed() == QString("Employee Additional"))
                                strMemberVolAmount = QString("\"%1\"").arg(contributionsLine.at(2).trimmed());

                            // Get the next line
                            contributionsLine = contributionsStream.readLine().split("\t");

                            // end of contributions amount list if count < 3
                            if(contributionsLine.count() < 3) break;

                        } // end loop through amounts for this employee
                    }

                    // Write out the details
                    contributionText << "\"" << strName << "\"" << "," << strFund << ","
                            << employeeMap->value(strName).split(",").at(1).trimmed() << ","
                            << employeeMap->value(strName).split(",").at(0).trimmed() << ","
                            << strSGC << "," << strSalSacrificeAmount << ","
                            << strEmpAddAmount << "," << strMemberVolAmount << endl;

                } // end if the line found is an employee name

                // After each employee details written out, go to next line
                contributionsLine = contributionsStream.readLine().split("\t");

            } //end loop through this Fund batch
        } else { // end if line is "Superannuation Fund:"

            // If in outer loop and line is NOT "Superannuation Fund:", get next line
            contributionsLine = contributionsStream.readLine().split("\t");
        }

    } // end loop through entire file

    delete employeeMap;

    // The output file is flushed and closed by the calling function in MainWindow

    QMessageBox::warning(this, QString("MLC SuperEzy Converter"),
        QString("Contributions file completed."));
}


// Writes a CSV file with member data for sending to MLC.
// Use this file to add new members to SuperEzy or to amend
// members details.
void DataFormatter::writeMembersFile(QFile *filePtr){
    QTextStream membersText(filePtr);
    QTextStream employeeDetailsStream(employeeFilePtr);
    QStringList employeeDetailsLine = employeeDetailsStream.readLine().split("\t");
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

    // The output file is flushed and closed by the calling function in MainWindow

    QMessageBox::warning(this, QString("MLC SuperEzy Converter"),
        QString("Members file completed."));
}

// Returns true if the employee is under 18, false if over 18
// or if an error occurs in the calculation.
bool DataFormatter::isUnderEighteen(QStringList employeeDetails){

    QDate eoqDate;

    // Get the employee's date of birth
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

// Maps employee Payroll Number and Member Number using
// Employee Name as key
QMap<QString, QString> *DataFormatter::getEmployeeMap(){
    QMap<QString, QString> *map = new QMap<QString, QString>();
    QTextStream employeeDetailsStream(employeeFilePtr);
    QStringList employeeDetailsLine = employeeDetailsStream.readLine().split("\t");

    // Loop through MYOB employees .txt file to get each employee's data.
    // Key is "MLC surname, firstname". Data is "PayrollNumber, MemberNumber".
    while(!employeeDetailsStream.atEnd()) {
        employeeDetailsLine = employeeDetailsStream.readLine().split("\t");

        map->insert(QString("%1, %2")
          .arg(getSurname(employeeDetailsLine.at(1).trimmed(),
                          employeeDetailsLine.at(0).trimmed()))
          .arg(employeeDetailsLine.at(1).trimmed()),
                    QString("%1,%2")
                    .arg(employeeDetailsLine.at(2).trimmed())
                    .arg(employeeDetailsLine.at(119).trimmed()));
    }
    return map;
}
