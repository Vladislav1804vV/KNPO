/*!
 * \file test_findclasses.cpp
 * \brief файл, содержащий реализацию тестирующего класса функции findClasses
*/
#include "test_findclasses.h"

Test_findClasses::Test_findClasses()
{

}

Test_findClasses::~Test_findClasses()
{

}

void Test_findClasses::ffindClasses()
{
    QFETCH(QStringList, list);
    QFETCH(int, start);
    QFETCH(int, end);
    QFETCH(ListInt, result);
    QVERIFY2(findClasses(list, start, end)==result, "Test was failed");
}

void Test_findClasses::ffindClasses_data()
{
    QTest::addColumn<QStringList>("list");
    QTest::addColumn<int>("start");
    QTest::addColumn<int>("end");
    QTest::addColumn<ListInt>("result");

    QStringList list;

    //1
    list << "class Main"
         << "{"
         << "}";
    QTest::newRow("oneClassWithoutBody") << list << 0 << 2 << ListInt{0};

    //2
    list.clear();
    list << "class Main"
         << "{"
         << "public static void main(String[] args)"
         << "{"
         << "}"
         << "}";
    QTest::newRow("oneClassWithBody") << list << 0 << 5 << ListInt{0};

    //3
    list.clear();
    list << "class Main"
         << "{"
         << "}"
         << "class Second"
         << "{"
         << "}";
    QTest::newRow("moreThanOneClass") << list << 0 << 5 << ListInt{0, 3};

    //4
    list.clear();
    list << "import java.awt.*"
         << "class Main"
         << "{"
         << "}";
    QTest::newRow("ThereIsClassInNotFirstLine") << list << 0 << 3 << ListInt{1};

    //5
    list.clear();
    list << "class my_class"
         << "{"
         << "}";
    QTest::newRow("headOfClassHasClassSubstr") << list << 0 << 2 << ListInt{0};

    //6
    list.clear();
    list << "private class Main"
         << "{"
         << "}";
    QTest::newRow("ClassHasAccessModifier") << list << 0 << 2 << ListInt{0};

    //7
    list.clear();
    list << "class Main"
         << "{"
         << "class NestedMain"
         << "{"
         << "}"
         << "}";
    QTest::newRow("ThereIsNestedClassIntoClass") << list << 0 << 5 << ListInt{0};

    //8
    list.clear();
    list << "class Main"
         << "{"
         << "class NestedMain"
         << "{"
         << "}"
         << "}"
         << "class Second"
         << "{"
         << "}";
    QTest::newRow("oneClassWithoutBody") << list << 1 << 5 << ListInt{2};
}


