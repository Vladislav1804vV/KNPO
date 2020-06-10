/*!
 * \file test_findfields.cpp
 * \brief файл, содержащий реализацию тестирующего класса функции findFields
*/
#include "test_findfields.h"

Test_findFields::Test_findFields()
{

}

Test_findFields::~Test_findFields()
{

}

void Test_findFields::ffindFields()
{
    QFETCH(QStringList, list);
    QFETCH(int, start);
    QFETCH(int, end);
    QFETCH(ListInt, result);
    QVERIFY2(findFields(list, start, end)==result, "Test was failed");
}

void Test_findFields::ffindFields_data()
{
    QTest::addColumn<QStringList>("list");
    QTest::addColumn<int>("start");
    QTest::addColumn<int>("end");
    QTest::addColumn<ListInt>("result");

    QStringList list;

    //1
    list << "class Main"
         << "{"
         << "int number;"
         << "}";
    QTest::newRow("oneField") << list << 2 << 3 << ListInt{2};

    //2
    list.clear();
    list << "class Main"
         << "{"
         << "int number;"
         << "int number2;"
         << "}";
    QTest::newRow("moreThanOneField") << list << 2 << 4 << ListInt{2, 3};

    //3
    list.clear();
    list << "class Main"
         << "{"
         << "int number = 5;"
         << "}";
    QTest::newRow("fieldHasValue") << list << 2 << 3 << ListInt{2};

    //4
    list.clear();
    list << "class Main"
         << "{"
         << "int number, number2"
         << "}";
    QTest::newRow("moreThanOneFieldWithoutValues") << list << 2 << 3 << ListInt{2, 2};

    //5
    list.clear();
    list << "class Main"
         << "{"
         << "int number = 5, number2"
         << "}";
    QTest::newRow("moreThanOneFieldAndFirstHasValue") << list << 2 << 3 << ListInt{2, 2};

    //6
    list.clear();
    list << "class Main"
         << "{"
         << "int number, number2 = 7"
         << "}";
    QTest::newRow("moreThanOneFieldAndLastHasValue") << list << 2 << 3 << ListInt{2, 2};

    //7
    list.clear();
    list << "class Main"
         << "{"
         << "int number = (2+3);"
         << "}";
    QTest::newRow("fieldHasValueWithRoundBrackets") << list << 2 << 3 << ListInt{2};

    //8
    list.clear();
    list << "class Main"
         << "{"
         << "int number[1] = {5};"
         << "}";
    QTest::newRow("fieldHasValueWithBraces") << list << 2 << 3 << ListInt{2};

    //9
    list.clear();
    list << "class Main"
         << "{"
         << "int number[3] = {1,2,3};"
         << "}";
    QTest::newRow("fieldHasValueWithCommas") << list << 2 << 3 << ListInt{2};

    //10
    list.clear();
    list << "class Main"
         << "{"
         << "Map<int,String> number;"
         << "}";
    QTest::newRow("fieldHasTypeWithCommas") << list << 2 << 3 << ListInt{2};
}
