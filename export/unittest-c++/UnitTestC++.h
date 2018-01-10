// Unit testing framework for C++
//
// Based on google test and UnitTest++
//
// UnitTest-c++ is based on google test (for printing values and test definitions), and UnitTest++.
//
// MIT License
//
// Copyright (c) 2017 Rene Barto
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
// to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of
// the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Copyright
//
// For Google test:
//
// Copyright 2008, Google Inc.
// All rights reserved.
//
// For UnitTest++:
//
// Copyright (c) 2006 Noel Llopis and Charles Nicholson

#pragma once

#include <unittest-c++/AssertionFailedException.h>
#include <unittest-c++/AssertMacros.h>
#include <unittest-c++/ConsoleTestReporter.h>
#include <unittest-c++/CurrentTest.h>
#include <unittest-c++/ITestReporter.h>
#include <unittest-c++/TestFixture.h>
#include <unittest-c++/TestMacros.h>
#include <unittest-c++/TestRunner.h>
#include <unittest-c++/TimeConstraint.h>
#include <unittest-c++/XMLTestReporter.h>

namespace UnitTestCpp
{

UNIT_TEST_CPP_EXPORT int RunAllTests();
UNIT_TEST_CPP_EXPORT int RunAllTests(ITestReporter & reporter);

template<class Predicate>
int RunSelectedTests(const Predicate & predicate)
{
    ConsoleTestReporter reporter;
    return RunSelectedTests(reporter, predicate);
}

template<class Predicate>
int RunSelectedTests(ITestReporter & reporter, const Predicate & predicate)
{
    TestRunner runner(reporter);
    return runner.RunTestsIf(Test::GetTestRegistry(), predicate, 0);
}

UNIT_TEST_CPP_EXPORT void ListAllTests();
UNIT_TEST_CPP_EXPORT void ListAllTests(ITestReporter & reporter);

template<class Predicate>
void ListSelectedTests(const Predicate & predicate)
{
    ConsoleTestReporter reporter;
    ListSelectedTests(reporter, predicate);
}

template<class Predicate>
void ListSelectedTests(ITestReporter & reporter, const Predicate & predicate)
{
    TestRunner runner(reporter);
    runner.ListTestsIf(Test::GetTestRegistry(), predicate);
}

} // namespace UnitTestCpp
