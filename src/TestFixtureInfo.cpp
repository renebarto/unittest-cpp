#include <unittest-cpp/TestFixtureInfo.h>

#include <cassert>

using namespace std;

namespace UnitTestCpp
{

TestFixtureInfo::TestFixtureInfo(const std::string & fixtureName)
    : next(nullptr)
    , head(nullptr)
    , tail(nullptr)
    , fixtureName(fixtureName)
{
}

void TestFixtureInfo::AddTest(Test * test)
{
    if (!tail)
    {
        assert(!head);
        head = test;
        tail = test;
    }
    else
    {
        tail->_next = test;
        tail = test;
    }
}

Test * TestFixtureInfo::GetHead() const
{
    return head;
}

const std::string & TestFixtureInfo::Name() const
{
    return fixtureName;
}

} // namespace UnitTestCpp
