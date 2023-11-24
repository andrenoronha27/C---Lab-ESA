//#include "bcppTableObjectsLib.h"
#include "BCPPPackageVectorTable.h"
#include "bcppTimeFunctionsLib.h"
#include "BCPPPackageTerminalUI.h"
#include "BCPPTerminalFormating.h"
#include <iostream>
#include <ctime>
#include <vector>
#include <iomanip>
#include <sstream>
#include <windows.h>

namespace tableObjects
{
    struct auditorumDBrow
    {
        int id;
        char blockname;
        int blockSize;
    };
    
    std::string ROWHEADERS = "\tBlock\tCapacity\n";
    
    std::string ROWHEADERSBOOK = "\tBlock\tBooked Date\n";

    struct bookingsDBRow
    {
        int id;
        int fkeyAuditorium;
        std::tm assignedDate;
    };
}

bool isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool isValidDate(int day, int month, int year)
{
    if (month < 1 || month > 12)
    {
        return false;
    }

    const int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (day < 1 || day > daysInMonth[month])
    {
        if (month == 2 && day == 29 && isLeapYear(year))
        {
            return true;
        }
        return false;
    }

    return true;
}

BCPP_Package_VectorTable::VectorTable<tableObjects::auditorumDBrow> auditoriumDB; // Database::
BCPP_Package_VectorTable::VectorTable<tableObjects::bookingsDBRow> bookingsDB;    // Database ::

void displayMenu(std::vector<std::string> menuOptions)
{
    std::cout<< "\n";
    for (std::string option : menuOptions)
    {
        std::cout << option << "\n";
    }
}

template <typename translationType>
translationType boolTranslate(bool inputbool, translationType ifTrueVal, translationType ifFalseVal)
{
    if (inputbool)
    {
        return ifTrueVal;
    }
    else
    {
        return ifFalseVal;
    }
}

void printAudDB()
{
    std::cout << tableObjects::ROWHEADERS;
    for (auto row : auditoriumDB.returnAllRows())
    {
        std::cout << "\t" << row.blockname << "\t" << row.blockSize << "\n";
    }
}

char returnAudChar(int id)
{
    return auditoriumDB.returnRow(id).blockname;
}

void createNewAuditorium()
{
    int last = auditoriumDB.returnSize();
    BCPP_Package_TerminalUI::formattedOutputs::generateTitleBar("\n\t\t\t\t\tAuditorium Management System", terminalFormatingColorcodes::ANSI_BG_BLUE);

    std::cout << "\n";
    std::cout<<"-------------------------------------------------------------------------------------------------------------";
    char newBlockName = BCPP_Package_TerminalUI::formattedPrompts::customPrompt<char>("\n\n\tEnter Block Name &: ", terminalFormatingColorcodes::ANSI_BG_GREEN);
    std::cout <<"\n";
    int blockSize = BCPP_Package_TerminalUI::formattedPrompts::customPrompt<int>("Enter Block Size $: ", terminalFormatingColorcodes::ANSI_BG_GREEN);
    std::cout<<"-------------------------------------------------------------------------------------------------------------";
    std::cout<< "\n";
    auditoriumDB.pushRow({last, newBlockName, blockSize});
}

int returnAudIDOnName(char AudName)
{
    for (auto &row : auditoriumDB.returnAllRows())
    {
        if (AudName == row.blockname)
        {
            return row.id;
        }
    }
    return -1;
}

bool checkBookingOnDate(char AuditorimName, std::tm date) //!!!CHECK THIS FUNCTION
{
    int Aid = -1;

    for (auto &row : auditoriumDB.returnAllRows())
    {
        if (AuditorimName == row.blockname)
        {
            Aid = row.id;
            break;
        }
    }

    for (auto &rowB : bookingsDB.returnAllRows())
    {
        if (Aid == rowB.fkeyAuditorium)
        {
            if (date.tm_mday == rowB.assignedDate.tm_mday &&
                date.tm_mon == rowB.assignedDate.tm_mon &&
                date.tm_year == rowB.assignedDate.tm_year)
            {
                return false;
            }
        }
    }

    return true;
}

void addNewBooking()
{
    int last = bookingsDB.returnSize();

    std::cout<< "\n";

    char AudName = BCPP_Package_TerminalUI::formattedPrompts::customPrompt<char>("Enter the Auditorium that you wish to book:", terminalFormatingColorcodes::ANSI_BG_GREEN);

    int fkeyID = returnAudIDOnName(AudName);
    if (fkeyID < 0)
    {
        BCPP_Package_TerminalUI::formattedOutputs::addMarker(terminalFormatingColorcodes::ANSI_BG_RED);
        std::cout << "\tInvalid Auditorium Name";
        BCPP_Package_TerminalUI::formattedOutputs::resetMarker();
        return;
    }
    std::tm parsedDate = {};

    std::cout<< "\n";

    parsedDate.tm_mday = BCPP_Package_TerminalUI::formattedPrompts::customPrompt<int>("\t\tDD%: ", terminalFormatingColorcodes::ANSI_BG_GREEN);
    parsedDate.tm_mon = BCPP_Package_TerminalUI::formattedPrompts::customPrompt<int>("\t\tMM%: ", terminalFormatingColorcodes::ANSI_BG_GREEN) - 1;
    parsedDate.tm_year = BCPP_Package_TerminalUI::formattedPrompts::customPrompt<int>("\t\tYY%: ", terminalFormatingColorcodes::ANSI_BG_GREEN) - 1900;
    std::cout<< "\n";
    bool valid = checkBookingOnDate(AudName, parsedDate);
    bool isDateValid = isValidDate(parsedDate.tm_mday, parsedDate.tm_mon + 1, parsedDate.tm_year + 1900);
    if (valid && isDateValid)
    {
        bookingsDB.pushRow({last, fkeyID, parsedDate});
    }
    else
    {
        BCPP_Package_TerminalUI::formattedOutputs::addMarker(terminalFormatingColorcodes::ANSI_BG_RED);
        std::cout << "\tKINDLY NOTE: \n\tBooking UNSUCCESSFUL! \n\tAuditorium is already Booked on that Date!";
        BCPP_Package_TerminalUI::formattedOutputs::resetMarker();
        BCPP_Package_TerminalUI::formattedPrompts::customPrompt<char>("\n\n\tPRESS ANY KEY TO CONTINUE! ");
        return;//
    };
}

void retunBookingTable()
{
    std::cout<< "\n\n\n";
    std::cout << tableObjects::ROWHEADERSBOOK;
    for (auto row : bookingsDB.returnAllRows())
    {
        std::cout << "\t" << returnAudChar(row.fkeyAuditorium) << "\t" << BudgetCppTimeDataStringFormatting::returnFormatedDate(row.assignedDate) << "\n";
    }
}

class Auditorium {
public:
    Auditorium(int size, int seatsCount) : size(size), seatsCount(seatsCount) {
        seats = new char**[size];
        for (int i = 0; i < size; ++i) {
            seats[i] = new char*[size];
            for (int j = 0; j < size; ++j) {
                seats[i][j] = new char[2];
                seats[i][j][0] = (i * size + j < seatsCount) ? 'O' : 'O'; // 'O' for seats, always marked as 'O'
                seats[i][j][1] = '\0'; // Null-terminate the string
            }
        }
    }

    ~Auditorium() {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                delete[] seats[i][j];
            }
            delete[] seats[i];
        }
        delete[] seats;
    }

    void display() const {
        for (int i = 0; i < size; ++i) {
            // Set background color to gray
            std::cout << "\x1B[47m";

            for (int j = 0; j < size; ++j) {
                // Check if a seat is present
                if (seats[i][j][0] == 'O') {
                    std::cout << "\x1B[37m" << seats[i][j] << " "; // White text for seats
                } else {
                    // If no seat, print a space with gray background
                    std::cout << "\x1B[37m \x1B[0m";
                }
            }

            std::cout << "\x1B[0m" << std::endl; // Reset to default colors after each row
        }
    }

public:
    char*** seats;
    int size;
    int seatsCount;
};


int main()
{
    system("Color 8F");
    char menuOption;
    bool sessionSwitch = true;
    bool editorSessionSwitch = false; // set to true when a option is selected
    char submenuOption;
    std::cout<< "\n\n";
    auditoriumDB.pushRow({0, 'A', 100});
    auditoriumDB.pushRow({1, 'B', 200});
    auditoriumDB.pushRow({2, 'C', 50});
    auditoriumDB.pushRow({3, 'D', 100});
    auditoriumDB.pushRow({4, 'E', 300});
    auditoriumDB.pushRow({5, 'F', 200});
    auditoriumDB.pushRow({6, 'G', 1000});
    while (sessionSwitch)
    {
        system("Color 8F");
        BCPP_Package_TerminalUI::formattedOutputs::generateTitleBar("\n\t\t\t\t\tAuditorium Management System", terminalFormatingColorcodes::ANSI_BG_BLUE);

        std::cout << "\n";
        std::cout<<"-------------------------------------------------------------------------------------------------------------";
        BCPP_Package_TerminalUI::formattedOutputs::optionsMenu(
            true,
            {"\n\n\t(e)dit Auditorium",
             "\t(b)ook",
             "\t(p)rint all the Auditoriums"
             "\n\t(q)uit"},
            terminalFormatingColorcodes::ANSI_BG_MAGENTA);
        std::cout<<"-------------------------------------------------------------------------------------------------------------";
        std::cout<< "\n";
        menuOption = BCPP_Package_TerminalUI::formattedPrompts::customPrompt<char>("\n\tEnter any of the given optionss $:   ");
        switch (menuOption)
        {
        
        case 'p':
{
    char menuOptionP; // Declare menuOption inside the while loop
    editorSessionSwitch = true;
    while (editorSessionSwitch)
    {
        system("Color 8F");
        BCPP_Package_TerminalUI::formattedOutputs::generateTitleBar("\n\t\t\t\t\tAuditorium Management System", terminalFormatingColorcodes::ANSI_BG_BLUE);

        std::cout << "\n";
        std::cout << "-------------------------------------------------------------------------------------------------------------";
        BCPP_Package_TerminalUI::formattedOutputs::optionsMenu(
            true,
            {"\n\n\t(i)nsert new Auditorium ",
             "\t(q)uit"},
            terminalFormatingColorcodes::ANSI_BG_MAGENTA);
        std::cout << "-------------------------------------------------------------------------------------------------------------";
        std::cout << "\n";
        
        // Block A with 100 seats in a 10x10 auditorium
        Auditorium blockA(10, 100);

        // Block B with 200 seats in a 14x14 auditorium
        Auditorium blockB(14, 200);

        // Block C with 50 seats in a 8x8 auditorium
        Auditorium blockC(8, 50);

        // Block D with 100 seats in a 10x10 auditorium
        Auditorium blockD(10, 100);

        // Block E with 300 seats in a 18x18 auditorium
        Auditorium blockE(18, 300);

        // Block F with 200 seats in a 14x14 auditorium
        Auditorium blockF(14, 200);

        // Block G with 1000 seats in a 32x32 auditorium
        Auditorium blockG(32, 1000);

        // Displaying the auditoriums
        std::cout<< "\n";
        std::cout << "Block A Auditorium:" << std::endl;
        blockA.display();
        std::cout << std::endl;

        std::cout << "Block B Auditorium:" << std::endl;
        blockB.display();
        std::cout << std::endl;

        std::cout << "Block C Auditorium:" << std::endl;
        blockC.display();
        std::cout << std::endl;

        std::cout << "Block D Auditorium:" << std::endl;
        blockD.display();
        std::cout << std::endl;

        std::cout << "Block E Auditorium:" << std::endl;
        blockE.display();
        std::cout << std::endl;

        std::cout << "Block F Auditorium:" << std::endl;
        blockF.display();
        std::cout << std::endl;

        std::cout << "Block G Auditorium:" << std::endl;
        blockG.display();

        menuOptionP = BCPP_Package_TerminalUI::formattedPrompts::customPrompt<char>("\n\n\tEnter an $: ");
        switch (menuOptionP)
        {
        case 'q':
            system("Color 8F");
            editorSessionSwitch = false;
            break;
        case 'i':
            system("Color 8F");
            createNewAuditorium();
            break;
        }
    }
    break; // Add this break statement to exit the 'p' case
}



        case 'q':
            sessionSwitch = false;
            break;

        case 'e':
            editorSessionSwitch = true;
            while (editorSessionSwitch)
            {
                system("Color 8F");
                BCPP_Package_TerminalUI::formattedOutputs::generateTitleBar("\n\t\t\t\t\tAuditorium Management System", terminalFormatingColorcodes::ANSI_BG_BLUE);

                std::cout << "\n";
                std::cout<<"-------------------------------------------------------------------------------------------------------------";
                BCPP_Package_TerminalUI::formattedOutputs::optionsMenu(
                    true,
                    {"\n\n\t(i)nsert new Auditorium ",
                     "\t(q)uit"},
                    terminalFormatingColorcodes::ANSI_BG_MAGENTA);
                std::cout<<"-------------------------------------------------------------------------------------------------------------";
                std::cout << "\n";
                printAudDB();
                submenuOption = BCPP_Package_TerminalUI::formattedPrompts::customPrompt<char>("\n\n\tEnter an $: ");
                switch (submenuOption)
                {
                case 'q':
                    system("Color 8F");
                    editorSessionSwitch = false;
                    break;
                case 'i':
                    system("Color 8F");
                    createNewAuditorium();
                    break;
                }
            }
            break;

        case 'b':
            system("Color 8F");
            editorSessionSwitch = true;
            while (editorSessionSwitch)
            {
                system("Color 8F");
                BCPP_Package_TerminalUI::formattedOutputs::generateTitleBar("\n\t\t\t\t\tAuditorium Management System", terminalFormatingColorcodes::ANSI_BG_BLUE);

                std::cout << "\n";
                std::cout<<"-------------------------------------------------------------------------------------------------------------";
                BCPP_Package_TerminalUI::formattedOutputs::optionsMenu(
                    true,
                    {"\n\n\t(b)ook an Auditorium",
                     "\t(q)uit "},
                    terminalFormatingColorcodes::ANSI_BG_MAGENTA);
                std::cout<<"-------------------------------------------------------------------------------------------------------------";
                std::cout << "\n";
                retunBookingTable();
                submenuOption = BCPP_Package_TerminalUI::formattedPrompts::customPrompt<char>("\n\n\tEnter an $: ");
                std::cout << "\n";
                switch (submenuOption)
                {
                case 'q':
                    editorSessionSwitch = false;
                    break;
                case 'b':
                    system("Color 8F");
                    printAudDB();
                    addNewBooking();
                }
            }
            break;
        default:
            break;
        }
        system("Color 8F");
    }
    system("Color 8F");
}
