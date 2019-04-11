#include <unittest-cpp/TestSuiteInfo.h>

#include <cassert>

using namespace std;

namespace UnitTestCpp
{

TestSuiteInfo::TestSuiteInfo(const std::string & suiteName)
    : next(nullptr)
    , head(nullptr)
    , tail(nullptr)
    , suiteName(suiteName)
{
}

TestSuiteInfo::~TestSuiteInfo()
{
    TestFixtureInfo * testFixture = GetHead();
    while (testFixture)
    {
        TestFixtureInfo * currentFixture = testFixture;
        testFixture = testFixture->next;
        delete currentFixture;
    }
}

TestFixtureInfo * TestSuiteInfo::GetTestFixture(const std::string & fixtureName)
{
#ifdef TRACE
#if defined(UNICDE) || defined(_UNICODE)
	wcout << L"Find suite " << fixtureName << L" ... ";
#else
	cout << "Find suite " << fixtureName << " ... ";
#endif
#endif
    TestFixtureInfo * testFixture = head;
    while (testFixture && (testFixture->Name() != fixtureName))
        testFixture = testFixture->next;
    if (!testFixture)
    {
#ifdef TRACE
#if defined(UNICDE) || defined(_UNICODE)
		wcout << L"not found, creating new object" << endl;
#else
		cout << "not found, creating new object" << endl;
#endif
#endif
        testFixture = new TestFixtureInfo(fixtureName);
        AddFixture(testFixture);
    }
    else
    {
#ifdef TRACE
#if defined(UNICDE) || defined(_UNICODE)
		wcout << L"found" << endl;
#else
		cout << "found" << endl;
#endif
#endif
    }
    return testFixture;
}

void TestSuiteInfo::AddFixture(TestFixtureInfo * testFixture)
{
    if (!tail)
    {
        assert(!head);
        head = testFixture;
        tail = testFixture;
    }
    else
    {
        tail->next = testFixture;
        tail = testFixture;
    }
}

TestFixtureInfo * TestSuiteInfo::GetHead() const
{
    return head;
}

const std::string & TestSuiteInfo::Name() const
{
    return suiteName;
}

} // namespace UnitTestCpp
