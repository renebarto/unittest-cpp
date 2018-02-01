#pragma once

namespace UnitTestCpp
{

class UNIT_TEST_CPP_EXPORT TestFixture
{
protected:
    TestFixture() {}
    TestFixture(const TestFixture &) = delete;
    virtual ~TestFixture() noexcept(false) {}

    TestFixture & operator = (const TestFixture &) = delete;

    virtual void SetUp() {}
    virtual void TearDown() {}

private:
    void Setup(); // Guard against incorrect override
    void Teardown(); // Guard against incorrect override
};

} // namespace UnitTestCpp
