/*
    Property of Shaunak Perni 
    Offered in GNU-L2 Public Limited License 
*/
#include "BCPPTerminalFormating.h"
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <limits>
#include <functional>

#pragma once
namespace BCPP_Package_TerminalUI
{
    namespace formattedOutputs
    {
        void generateTitleBar(std::string text, std::string color)
        {
            terminalFunctions::clearScreen();
            std::cout << color << text << terminalFormatingColorcodes::ANSI_RESET << std::endl;
        }

        void printHeaders(int selection, const std::vector<std::string> &Headers, std::string colorActive, std::string colorInactive)
        {
            for (int nHeader = 0; nHeader < Headers.size(); nHeader++)
            {
                selection == nHeader ? std::cout << colorActive << Headers[nHeader] << terminalFormatingColorcodes::ANSI_RESET
                                                 << "\t"
                                     : std::cout << colorInactive << Headers[nHeader] << terminalFormatingColorcodes::ANSI_RESET << "\t";
            }
            std::cout << std::endl;
        }

        void optionsMenu(bool directionVertical, std::vector<std::string> options)
        {
            std::string endline = "";
            directionVertical ? endline = "\n" : endline = "\t";
            for (std::string option : options)
            {
                std::cout << option << endline;
            }
            std::cout << std::endl;
        }

        void optionsMenu(bool directionVertical, std::vector<std::string> options, std::string color)
        {
            std::string endline = "";
            directionVertical ? endline = "\n" : endline = "\t";
            for (std::string option : options)
            {
                std::cout << color << option << terminalFormatingColorcodes::ANSI_RESET << endline;
            }
            std::cout << std::endl;
        }

        void addMarker(std::string MarkerColor)
        {
            std::cout << MarkerColor;
        }

        void resetMarker()
        {
            std::cout << terminalFormatingColorcodes::ANSI_RESET;
        }

        void highlightText(std::string text, std::string HighlightColor)
        {
            std::cout << HighlightColor << text << terminalFormatingColorcodes::ANSI_RESET;
        }

        struct stringWColor
        {
            std::string optionString;
            std::string color;
        };

        void fancyOptionsMenu(bool directionVertical, std::vector<stringWColor> optionsStruct)
        {
            std::string endline = "";
            directionVertical ? endline = "\n" : endline = "\t";
            for (stringWColor option : optionsStruct)
            {
                std::cout << option.color << option.optionString << terminalFormatingColorcodes::ANSI_RESET << endline;
            }
            std::cout << std::endl;
        }

        template <typename ratedDataType>
        void binaryColorThresholdFormatedValue(ratedDataType data, std::string colorAbove, std::string colorBelow, ratedDataType midPoint)
        {
            data >= midPoint ? std::cout << colorAbove : std::cout << colorBelow;
            std::cout << data << terminalFormatingColorcodes::ANSI_RESET;
        }

        void binaryColorAlternateFormatedValue(std::string printValue, bool *alternator, std::string Color1, std::string Color2)
        {
            *alternator ? addMarker(Color1) : addMarker(Color2);
            std::cout << printValue;
            resetMarker();
            *alternator = !*alternator;
        }
    }
    namespace formattedPrompts
    {
        template <typename returnData>
        returnData customPrompt(std::string promptSymbol)
        {
            returnData tempReturnData;
            std::cout << promptSymbol;
            std::cin >> tempReturnData;
            return tempReturnData;
        }

        template <typename returnData>
        returnData customPrompt(std::string promptSymbol, std::string message)
        {
            returnData tempReturnData;
            std::cout << message << "\t" << promptSymbol;
            std::cin >> tempReturnData;
            return tempReturnData;
        }

        template <typename returnDataMember0, typename returnDataMember1>
        auto customPromptFormDualInputs(std::string formMessage = "", std::string promptSymbol = ">:", std::string input0Message = "", std::string input1Message = "", std::string input0Color = "", std::string input1Color = "")
        {
            std::cout << formMessage << std::endl;
            returnDataMember0 rData0;
            returnDataMember1 rData1;

            std::cout << input0Message << promptSymbol << terminalFormatingColorcodes::ANSI_RESET << input0Color;

            while (!(std::cin >> rData0))
            {
                std::cout << terminalFormatingColorcodes::ANSI_RESET << "Invalid input. Please enter a valid " << typeid(returnDataMember0).name() << ": " << input0Color;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

            std::cout << terminalFormatingColorcodes::ANSI_RESET << input1Message << promptSymbol << terminalFormatingColorcodes::ANSI_RESET << input1Color;

            while (!(std::cin >> rData1))
            {
                std::cout << terminalFormatingColorcodes::ANSI_RESET << "Invalid input. Please enter a valid " << typeid(returnDataMember1).name() << ": " << input1Color;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

            std::cout << terminalFormatingColorcodes::ANSI_RESET << std::endl;

            return std::make_pair(rData0, rData1);
        }

    }
}

namespace BCCP_Package_UIRATPACK
{
    namespace menuSystem
    {
        struct actionOptions
        {
            char option;
            std::string optinDescription;
            std::function<void()> action;
        };
        void menuSystem()
        {
        }
    }
}