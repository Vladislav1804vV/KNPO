/*!
 * \file test_parsingclasses.cpp
 * \brief файл, содержащий реализацию тестирующего класса функции parsingClasses
*/
#include "test_parsingclasses.h"

Test_parsingClasses::Test_parsingClasses()
{

}

Test_parsingClasses::~Test_parsingClasses()
{

}

void Test_parsingClasses::pparsingClasses()
{
    QFETCH(QStringList, list);
    QFETCH(QString, namePackage);
    QFETCH(int, startPos);
    QFETCH(dataClass, result);
    dataClass dataClasses;
    parsingClasses(list, namePackage, startPos, dataClasses);
    for(int i = 0; i < dataClasses.size(); i++)
    {
       QVERIFY2(dataClasses[i].fields.size() == result[i].fields.size(), "Test was failed");
       QVERIFY2(dataClasses[i].methods.size() == result[i].methods.size(), "Test was failed");
       QVERIFY2(dataClasses[i].nameClass == result[i].nameClass, "Test was failed");
       QVERIFY2(dataClasses[i].extendsValue == result[i].extendsValue, "Test was failed");
       QVERIFY2(dataClasses[i].implementsValue == result[i].implementsValue, "Test was failed");
       QVERIFY2(dataClasses[i].nestedClasses.size() == result[i].nestedClasses.size(), "Test was failed");
    }
}

void Test_parsingClasses::pparsingClasses_data()
{
    QTest::addColumn<QStringList>("list");
    QTest::addColumn<QString>("namePackage");
    QTest::addColumn<int>("startPos");
    QTest::addColumn<dataClass>("result");

    QStringList list;
    dataClass dataClasses;
    dataWrittenClass *dataClass;

    //1
    list << "class Main"
         << "{"
         << "}";
    dataClass = new dataWrittenClass;
    dataClass->nameClass = "Main";
    dataClasses << *dataClass;
    QTest::newRow("emptyClass") << list << "" << 0 << dataClasses;
    delete dataClass;

    //2
    list.clear();
    dataClasses.clear();
    list << "class Main"
         << "{"
         << "int number;"
         << "}";
    dataClass = new dataWrittenClass;
    dataClass->nameClass = "Main";
    dataClass->fields << dataWrittenField{};
    dataClasses << *dataClass;
    QTest::newRow("classHasOneField") << list << "" << 0 << dataClasses;
    delete dataClass;

    //3
    list.clear();
    dataClasses.clear();
    list << "class Main"
         << "{"
         << "int number;"
         << "int number2;"
         << "}";
    dataClass = new dataWrittenClass;
    dataClass->nameClass = "Main";
    dataClass->fields << dataWrittenField{} << dataWrittenField{};
    dataClasses << *dataClass;
    QTest::newRow("classHasTwoFields") << list << "" << 0 << dataClasses;
    delete dataClass;

    //4
    list.clear();
    dataClasses.clear();
    list << "class Main"
         << "{"
         << "int number, number2;"
         << "}";
    dataClass = new dataWrittenClass;
    dataClass->nameClass = "Main";
    dataClass->fields << dataWrittenField{} << dataWrittenField{};
    dataClasses << *dataClass;
    QTest::newRow("classHasTwoFieldsInSameLine") << list << "" << 0 << dataClasses;
    delete dataClass;

    //5
    list.clear();
    dataClasses.clear();
    list << "class Main"
         << "{"
         << "int sum()"
         << "{"
         << "}"
         << "}";
    dataClass = new dataWrittenClass;
    dataClass->nameClass = "Main";
    dataClass->methods << dataWrittenMethod{};
    dataClasses << *dataClass;
    QTest::newRow("classHasOneMethod") << list << "" << 0 << dataClasses;
    delete dataClass;

    //6
    list.clear();
    dataClasses.clear();
    list << "class Main"
         << "{"
         << "int sum()"
         << "{"
         << "}"
         << "int sum2()"
         << "{"
         << "}"
         << "}";
    dataClass = new dataWrittenClass;
    dataClass->nameClass = "Main";
    dataClass->methods << dataWrittenMethod{} << dataWrittenMethod{};
    dataClasses << *dataClass;
    QTest::newRow("classHasTwoMethods") << list << "" << 0 << dataClasses;
    delete dataClass;

    //7
    list.clear();
    dataClasses.clear();
    list << "class Main"
         << "{"
         << "int number;"
         << "int sum()"
         << "{"
         << "}"
         << "}";
    dataClass = new dataWrittenClass;
    dataClass->nameClass = "Main";
    dataClass->fields << dataWrittenField{};
    dataClass->methods << dataWrittenMethod{};
    dataClasses << *dataClass;
    QTest::newRow("classHasOneFieldAndOneMethod") << list << "" << 0 << dataClasses;
    delete dataClass;

    //8
    list.clear();
    dataClasses.clear();
    list << "package com.main"
         << "class Main"
         << "{"
         << "}";
    dataClass = new dataWrittenClass;
    dataClass->nameClass = "com.main.Main";
    dataClasses << *dataClass;
    QTest::newRow("classHasPackage") << list << "com.main" << 0 << dataClasses;
    delete dataClass;

    //9
    list.clear();
    dataClasses.clear();
    list << "class Main extends Ancestor"
         << "{"
         << "}";
    dataClass = new dataWrittenClass;
    dataClass->nameClass = "Main";
    dataClass->extendsValue = "Ancestor";
    dataClasses << *dataClass;
    QTest::newRow("classHasExtends") << list << "" << 0 << dataClasses;
    delete dataClass;

    //10
    list.clear();
    dataClasses.clear();
    list << "class Main implements Interface"
         << "{"
         << "}";
    dataClass = new dataWrittenClass;
    dataClass->nameClass = "Main";
    dataClass->implementsValue = "Interface";
    dataClasses << *dataClass;
    QTest::newRow("classHasImplements") << list << "" << 0 << dataClasses;
    delete dataClass;

    //11
    list.clear();
    dataClasses.clear();
    list << "class Main"
         << "{"
         << "class nestedMain"
         << "{"
         << "}"
         << "}";
    dataClass = new dataWrittenClass;
    dataClass->nameClass = "Main";
    dataClass->nestedClasses << dataWrittenClass{};
    dataClasses << *dataClass;
    QTest::newRow("classHasNestedClass") << list << "" << 0 << dataClasses;
    delete dataClass;
}
