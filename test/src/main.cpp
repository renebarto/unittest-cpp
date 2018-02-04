#include <unittest-cpp/UnitTestC++.h>

#include <fstream>
#include "unittest-cpp/OSAL.h"
#include "unittest-cpp/Console.h"
#include "unittest-cpp/ConsoleGoogleTestReporter.h"

static const std::string moduleName = "unittest-cpp";

int Usage(UnitTestCpp::Console & console)
{
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "TODO: explain command line" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Default);
    return EXIT_FAILURE;
}

int main(int argc, const char * argv[])
{
    UnitTestCpp::Console console;

    console << fgcolor(UnitTestCpp::ConsoleColor::Magenta | UnitTestCpp::ConsoleColor::Intensity);
    console << "Running tests for: " << moduleName << std::endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Default);
    std::string applicationName = argv[0];
    std::string xmlOutput;
    bool gtestEmulation{};
    bool gtestList{};
    const std::string optionXML = "--xml";
    const std::string optionGtestFilter = "--gtest_filter=";
    const std::string optionGtestColor = "--gtest_color=";
    const std::string optionGtestList = "--gtest_list_tests";
    console << fgcolor(UnitTestCpp::ConsoleColor::Yellow) << "Command line arguments:" << endl;
    if (argc > 1)
    {
        if (UnitTestCpp::IsEqualIgnoreCase(argv[1], optionXML))
            xmlOutput = argv[2];
        else
        {
            for (int i = 1; i < argc; ++i)
            {
                if (UnitTestCpp::IsEqualIgnoreCase(string(argv[i]).substr(0, optionGtestFilter.length()), optionGtestFilter) ||
                    UnitTestCpp::IsEqualIgnoreCase(string(argv[i]).substr(0, optionGtestColor.length()), optionGtestColor))
                    gtestEmulation = true;
                else if (UnitTestCpp::IsEqualIgnoreCase(string(argv[i]).substr(0, optionGtestList.length()), optionGtestList))
                {
                    gtestEmulation = true;
                    gtestList = true;
                }
                else
                    return Usage(console);
            }
        }
    }
    console << fgcolor(UnitTestCpp::ConsoleColor::Default);

    console << "Application: " << applicationName << std::endl;
    console << "XML output : " << xmlOutput << std::endl;

    int result = 0;

    if (!xmlOutput.empty())
    {
        std::ofstream outputFile;

        outputFile.open(xmlOutput);
        UnitTestCpp::XMLTestReporter reporter(outputFile);
        result = RunAllTests(reporter);
    } else if (gtestEmulation)
    {
        if (gtestList)
        {
            UnitTestCpp::ConsoleGoogleTestReporter reporter;
            ListAllTests(reporter);
        }
        else
        {
            UnitTestCpp::ConsoleGoogleTestReporter reporter;
            result = RunAllTests(reporter);
        }
    } else
    {
        UnitTestCpp::ConsoleTestReporter reporter;
        result = RunAllTests(reporter);
    }

    return result;
}
