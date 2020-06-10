/*!
 * \file test_findmethods.cpp
 * \brief файл, содержащий реализацию тестирующего класса функции findMethods
*/
#include "test_findmethods.h"

Test_findMethods::Test_findMethods()
{

}

Test_findMethods::~Test_findMethods()
{

}

void Test_findMethods::ffindMethods()
{
    QFETCH(QStringList, list);
    QFETCH(int, start);
    QFETCH(int, end);
    QFETCH(ListInt, result);
    QVERIFY2(findMethods(list, start, end)==result, "Test was failed");
}

void Test_findMethods::ffindMethods_data()
{
    QTest::addColumn<QStringList>("list");
    QTest::addColumn<int>("start");
    QTest::addColumn<int>("end");
    QTest::addColumn<ListInt>("result");

    QStringList list;

    //1
    list << "class Main"
         << "{"
         << "int sum()"
         << "{"
         << "}"
         << "}";
    QTest::newRow("oneMethod") << list << 2 << 5 << ListInt{2};

    //2
    list.clear();
    list << "class Main"
         << "{"
         << "int sum()"
         << "{"
         << "}"
         << "int sum2()"
         << "{"
         << "}"
         << "}";
    QTest::newRow("moreThanOneMethod") << list << 2 << 8 << ListInt{2, 5};

    //3
    list.clear();
    list << "class Main"
         << "{"
         << "int sum({})"
         << "{"
         << "}"
         << "}";
    QTest::newRow("methodHasParametersWithBraces") << list << 2 << 5 << ListInt{2};

    //4
    list.clear();
    list << "class Main"
         << "{"
         << "Main()"
         << "{"
         << "}"
         << "}";
    QTest::newRow("methodIsConstructorOfClass") << list << 2 << 5 << ListInt{2};

    //5
    list.clear();
    list << "class Main"
         << "{"
         << "int sum()"
         << "{"
         << "{"
         << "}"
         << "}"
         << "}";
    QTest::newRow("methodHasBodyWithBraces") << list << 2 << 7 << ListInt{2};
}
