#include <unittest-cpp/UnitTestC++.h>

#include <fstream>
#include "unittest-cpp/OSAL.h"
#include "unittest-cpp/Console.h"
#include "unittest-cpp/ConsoleGoogleTestReporter.h"

static const std::string moduleName = "unittest-cpp";

int Usage(UnitTestCpp::Console & console)
{
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "Usage: " << moduleName << ".test.exe [options]" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "Options:" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "  --xml <path>                 " << "Output to XML file" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "  --gtest_emulation            " << "Output in Google Test emulation mode (Google Test emulation)" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "  --filter <pattern> or        " << "Output in Google Test emulation mode" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "  --gtest_filter <pattern>     " << "Filter tests (* for all, <suite>.* for all in suite, *<string>* for all containing string etc. (Google Test emulation)" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "  --color [1|Y|YES|ON] or      " << "Filter tests (* for all, <suite>.* for all in suite, *<string>* for all containing string etc." << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "  --gtest_color [1|Y|YES|ON]   " << "Output in color if one of the options is used otherwise no color (default = color output)" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "  --list or --gtest_list_tests " << "Only list tests according to filter, then exit" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "Pattern can be:                " << "Treated as:" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "suite.fixture.test             " << "suite.fixture.test" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "fixture.test                   " << "*.fixture.test" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "suite.fixture.*                " << "suite.fixture.*" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "suite.*                        " << "suite.*.*" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "*.fixture.test                 " << "*.fixture.test" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "*.test                         " << "*.*.test" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "*                              " << "*.*.*" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "*test                          " << "*.*.*test" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "*fixture.test                  " << "*.*fixture.test" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "*suite.fixture.test            " << "*suite.fixture.test" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "suite*                         " << "suite*.*.*" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "suite.fixture*                 " << "suite.fixture*.*" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "suite.fixture.test*            " << "suite.fixture.test*" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Default);
    return EXIT_FAILURE;
}

bool GetBoolean(const std::string & option)
{
    return UnitTestCpp::IsEqualIgnoreCase(option, "1") ||
           UnitTestCpp::IsEqualIgnoreCase(option, "ON") ||
           UnitTestCpp::IsEqualIgnoreCase(option, "Y") ||
           UnitTestCpp::IsEqualIgnoreCase(option, "YES");
}

int main(int argc, const char * argv[])
{
    UnitTestCpp::Console console;

    console << fgcolor(UnitTestCpp::ConsoleColor::Magenta | UnitTestCpp::ConsoleColor::Intensity);
    console << "Running tests for: " << moduleName << std::endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Default);
    std::string applicationName = argv[0];
    std::string testFilter;
    bool useColor = true;
    std::string xmlOutput;
    bool gtestEmulation{};
    bool listTests{};
    const std::string optionXML = "--xml";
    const std::string optionGTestEmulation = "--gtest_emulation";
    const std::string optionGTestFilter = "--gtest_filter=";
    const std::string optionFilter = "--filter=";
    const std::string optionGTestColor = "--gtest_color=";
    const std::string optionColor = "--color=";
    const std::string optionGTestList = "--gtest_list_tests";
    const std::string optionList = "--list";
    console << fgcolor(UnitTestCpp::ConsoleColor::Yellow) << "Command line arguments:" << endl;
    if (argc > 1)
    {
        if (UnitTestCpp::IsEqualIgnoreCase(argv[1], optionXML))
            xmlOutput = argv[2];
        else
        {
            for (int i = 1; i < argc; ++i)
            {
                std::string argument(argv[i]);
                if (UnitTestCpp::IsEqualIgnoreCase(argument.substr(0, optionGTestEmulation.length()), optionGTestEmulation))
                {
                    gtestEmulation = true;
                }
                else if (UnitTestCpp::IsEqualIgnoreCase(argument.substr(0, optionGTestFilter.length()), optionGTestFilter))
                {
                    testFilter = argument.substr(optionGTestFilter.length());
                    gtestEmulation = true;
                }
                else if (UnitTestCpp::IsEqualIgnoreCase(argument.substr(0, optionFilter.length()), optionFilter))
                {
                    testFilter = argument.substr(optionFilter.length());
                }
                else if (UnitTestCpp::IsEqualIgnoreCase(argument.substr(0, optionGTestColor.length()), optionGTestColor))
                {
                    useColor = GetBoolean(argument.substr(optionGTestColor.length()));
                    gtestEmulation = true;
                }
                else if (UnitTestCpp::IsEqualIgnoreCase(argument.substr(0, optionColor.length()), optionColor))
                {
                    useColor = GetBoolean(argument.substr(optionColor.length()));
                }
                else if (UnitTestCpp::IsEqualIgnoreCase(argument.substr(0, optionGTestList.length()), optionGTestList))
                {
                    gtestEmulation = true;
                    listTests = true;
                }
                else if (UnitTestCpp::IsEqualIgnoreCase(argument.substr(0, optionList.length()), optionList))
                {
                    listTests = true;
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

    UnitTestCpp::True NonFilteringSelector;
    UnitTestCpp::InSelectionFilter FilteringSelector(testFilter);
    UnitTestCpp::Selector * filter = &NonFilteringSelector;
    if (!testFilter.empty())
        filter = &FilteringSelector;

    if (!xmlOutput.empty())
    {
        std::ofstream outputFile;

        outputFile.open(xmlOutput);
        UnitTestCpp::XMLTestReporter reporter(outputFile);
        result = RunSelectedTests(reporter, *filter);
    } else
    {
        UnitTestCpp::ConsoleTestReporter reporterStandard(useColor);
        UnitTestCpp::ConsoleGoogleTestReporter reporterGTest(useColor);
        UnitTestCpp::ITestReporter *reporter = &reporterStandard;
        if (gtestEmulation)
        {
            reporter = &reporterGTest;
        }
        if (listTests)
        {
            ListSelectedTests(*reporter, *filter);
        } else
        {
            result = RunSelectedTests(*reporter, *filter);
        }
    }
    return result;
}
