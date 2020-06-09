#include "test_parsingmethods.h"

Test_parsingMethods::Test_parsingMethods()
{

}

Test_parsingMethods::~Test_parsingMethods()
{

}

void Test_parsingMethods::pparsingMethods()
{
    QFETCH(QStringList, list);
    QFETCH(ListInt, listMethods);
    QFETCH(dataMethod, result);
    dataMethod dataMethods;
    parsingMethods(list, listMethods, dataMethods);
    for(int i = 0; i < dataMethods.size(); i++)
    {
       QVERIFY2(dataMethods[i].nameMethod == result[i].nameMethod, "Test was failed");
       QVERIFY2(dataMethods[i].parameters == result[i].parameters, "Test was failed");
       QVERIFY2(dataMethods[i].typeMethod == result[i].typeMethod, "Test was failed");
       QVERIFY2(dataMethods[i].throwsValue == result[i].throwsValue, "Test was failed");
       QVERIFY2(dataMethods[i].valueMethod == result[i].valueMethod, "Test was failed");
       QVERIFY2(dataMethods[i].otherModifier == result[i].otherModifier, "Test was failed");
       QVERIFY2(dataMethods[i].accessModifier == result[i].accessModifier, "Test was failed");
       QVERIFY2(dataMethods[i].typeInitializationBlock == result[i].typeInitializationBlock, "Test was failed");
       QVERIFY2(dataMethods[i].valueInitializationBlock == result[i].valueInitializationBlock, "Test was failed");
    }
}

void Test_parsingMethods::pparsingMethods_data()
{
    QTest::addColumn<QStringList>("list");
    QTest::addColumn<ListInt>("listMethods");
    QTest::addColumn<dataMethod>("result");

    QStringList list;
    dataMethod dataMethods;
    dataWrittenMethod *dataMethod;

    //1
    list << "class Main"
         << "{"
         << "int sum()"
         << "{"
         << "}"
         << "}";
    dataMethod = new dataWrittenMethod;
    dataMethod->nameMethod = "sum";
    dataMethod->typeMethod = "int";
    dataMethod->parameters << "";
    dataMethods << *dataMethod;
    QTest::newRow("oneMethod") << list  << ListInt{2} << dataMethods;
    delete dataMethod;

    //2
    list.clear();
    dataMethods.clear();
    list << "class Main"
         << "{"
         << "int sum()"
         << "{"
         << "}"
         << "int sum2()"
         << "{"
         << "}"
         << "int sum3()"
         << "{"
         << "}"
         << "}";
    dataMethod = new dataWrittenMethod[3];
    dataMethod[0].nameMethod = "sum";
    dataMethod[0].typeMethod = "int";
    dataMethod[0].parameters << "";
    dataMethod[1].nameMethod = "sum2";
    dataMethod[1].typeMethod = "int";
    dataMethod[1].parameters << "";
    dataMethod[2].nameMethod = "sum3";
    dataMethod[2].typeMethod = "int";
    dataMethod[2].parameters << "";
    dataMethods << dataMethod[0] << dataMethod[1] << dataMethod[2];
    QTest::newRow("moreThanOneMethod") << list  << ListInt{2,5,8} << dataMethods;
    delete[] dataMethod;

    //3
    list.clear();
    dataMethods.clear();
    list << "class Main"
         << "{"
         << "int sum()"
         << "{"
         << "int number;"
         << "}"
         << "}";
    dataMethod = new dataWrittenMethod;
    dataMethod->nameMethod = "sum";
    dataMethod->typeMethod = "int";
    dataMethod->valueMethod = "int number;";
    dataMethod->parameters << "";
    dataMethods << *dataMethod;
    QTest::newRow("methodHasBody") << list  << ListInt{2} << dataMethods;
    delete dataMethod;

    //4
    list.clear();
    dataMethods.clear();
    list << "class Main"
         << "{"
         << "public int sum()"
         << "{"
         << "}"
         << "}";
    dataMethod = new dataWrittenMethod;
    dataMethod->nameMethod = "sum";
    dataMethod->typeMethod = "int";
    dataMethod->accessModifier = "public";
    dataMethod->parameters << "";
    dataMethods << *dataMethod;
    QTest::newRow("methodHasAccessModifier") << list  << ListInt{2} << dataMethods;
    delete dataMethod;

    //5
    list.clear();
    dataMethods.clear();
    list << "class Main"
         << "{"
         << "static int sum()"
         << "{"
         << "}"
         << "}";
    dataMethod = new dataWrittenMethod;
    dataMethod->nameMethod = "sum";
    dataMethod->typeMethod = "int";
    dataMethod->otherModifier << "static";
    dataMethod->parameters << "";
    dataMethods << *dataMethod;
    QTest::newRow("methodHasOtherModifier") << list  << ListInt{2} << dataMethods;
    delete dataMethod;

    //6
    list.clear();
    dataMethods.clear();
    list << "class Main"
         << "{"
         << "static final int sum()"
         << "{"
         << "}"
         << "}";
    dataMethod = new dataWrittenMethod;
    dataMethod->nameMethod = "sum";
    dataMethod->typeMethod = "int";
    dataMethod->otherModifier << "static" << "final";
    dataMethod->parameters << "";
    dataMethods << *dataMethod;
    QTest::newRow("methodHasMoreThanOneOtherModifier") << list  << ListInt{2} << dataMethods;
    delete dataMethod;

    //7
    list.clear();
    dataMethods.clear();
    list << "class Main"
         << "{"
         << "public static int sum()"
         << "{"
         << "}"
         << "}";
    dataMethod = new dataWrittenMethod;
    dataMethod->nameMethod = "sum";
    dataMethod->typeMethod = "int";
    dataMethod->accessModifier = "public";
    dataMethod->otherModifier << "static";
    dataMethod->parameters << "";
    dataMethods << *dataMethod;
    QTest::newRow("methodHasAccessModifierAndOtheModifier") << list  << ListInt{2} << dataMethods;
    delete dataMethod;

    //8
    list.clear();
    dataMethods.clear();
    list << "class Main"
         << "{"
         << "List<int> sum()"
         << "{"
         << "}"
         << "}";
    dataMethod = new dataWrittenMethod;
    dataMethod->nameMethod = "sum";
    dataMethod->typeMethod = "List<int>";
    dataMethod->parameters << "";
    dataMethods << *dataMethod;
    QTest::newRow("methodHasTypeOfSerialContainers") << list  << ListInt{2} << dataMethods;
    delete dataMethod;

    //9
    list.clear();
    dataMethods.clear();
    list << "class Main"
         << "{"
         << "Map<int,String> sum()"
         << "{"
         << "}"
         << "}";
    dataMethod = new dataWrittenMethod;
    dataMethod->nameMethod = "sum";
    dataMethod->typeMethod = "Map<int,String>";
    dataMethod->parameters << "";
    dataMethods << *dataMethod;
    QTest::newRow("methodHasTypeOfAssociativeContainers") << list  << ListInt{2} << dataMethods;
    delete dataMethod;

    //10
    list.clear();
    dataMethods.clear();
    list << "class Main"
         << "{"
         << "int sum(int param)"
         << "{"
         << "}"
         << "}";
    dataMethod = new dataWrittenMethod;
    dataMethod->nameMethod = "sum";
    dataMethod->typeMethod = "int";
    dataMethod->parameters << "int param";
    dataMethods << *dataMethod;
    QTest::newRow("methodHasOneParameter") << list  << ListInt{2} << dataMethods;
    delete dataMethod;

    //11
    list.clear();
    dataMethods.clear();
    list << "class Main"
         << "{"
         << "int sum(int param, int param2)"
         << "{"
         << "}"
         << "}";
    dataMethod = new dataWrittenMethod;
    dataMethod->nameMethod = "sum";
    dataMethod->typeMethod = "int";
    dataMethod->parameters << "int param" << "int param2";
    dataMethods << *dataMethod;
    QTest::newRow("methodHasMoreThanOneParameter") << list  << ListInt{2} << dataMethods;
    delete dataMethod;

    //12
    list.clear();
    dataMethods.clear();
    list << "class Main"
         << "{"
         << "int sum(int[] param = {1,2,3})"
         << "{"
         << "}"
         << "}";
    dataMethod = new dataWrittenMethod;
    dataMethod->nameMethod = "sum";
    dataMethod->typeMethod = "int";
    dataMethod->parameters << "int[] param = {1,2,3}";
    dataMethods << *dataMethod;
    QTest::newRow("parameterOfMethodHasValueWithCommasIntoBraces") << list  << ListInt{2} << dataMethods;
    delete dataMethod;

    //13
    list.clear();
    dataMethods.clear();
    list << "class Main"
         << "{"
         << "int sum(String str = \"(,,,)\")"
         << "{"
         << "}"
         << "}";
    dataMethod = new dataWrittenMethod;
    dataMethod->nameMethod = "sum";
    dataMethod->typeMethod = "int";
    dataMethod->parameters << "String str = \"(,,,)\"";
    dataMethods << *dataMethod;
    QTest::newRow("parameterOfMethodHasValueWithCommasInRoundBrackesIntoStringConstant") << list  << ListInt{2} << dataMethods;
    delete dataMethod;

    //14
    list.clear();
    dataMethods.clear();
    list << "class Main"
         << "{"
         << "int sum(String str = \"{,,,}\")"
         << "{"
         << "}"
         << "}";
    dataMethod = new dataWrittenMethod;
    dataMethod->nameMethod = "sum";
    dataMethod->typeMethod = "int";
    dataMethod->parameters << "String str = \"{,,,}\"";
    dataMethods << *dataMethod;
    QTest::newRow("parameterOfMethodHasValueWithCommasInBracesIntoStringConstant") << list  << ListInt{2} << dataMethods;
    delete dataMethod;

    //15
    list.clear();
    dataMethods.clear();
    list << "class Main"
         << "{"
         << "{"
         << "}"
         << "}";
    dataMethod = new dataWrittenMethod;
    dataMethod->typeInitializationBlock = "non-static";
    dataMethods << *dataMethod;
    QTest::newRow("initializationBlockAnnouncedWithoutType") << list  << ListInt{2} << dataMethods;
    delete dataMethod;

    //16
    list.clear();
    dataMethods.clear();
    list << "class Main"
         << "{"
         << "static"
         << "{"
         << "}"
         << "}";
    dataMethod = new dataWrittenMethod;
    dataMethod->typeInitializationBlock = "static";
    dataMethods << *dataMethod;
    QTest::newRow("initializationBlockAnnouncedWithType") << list  << ListInt{3} << dataMethods;
    delete dataMethod;

    //17
    list.clear();
    dataMethods.clear();
    list << "class Main"
         << "{"
         << "{"
         << "int number;"
         << "}"
         << "}";
    dataMethod = new dataWrittenMethod;
    dataMethod->typeInitializationBlock = "non-static";
    dataMethod->valueInitializationBlock = "int number;";
    dataMethods << *dataMethod;
    QTest::newRow("initializationBlockHasBody") << list  << ListInt{2} << dataMethods;
    delete dataMethod;

    //18
    list.clear();
    dataMethods.clear();
    list << "class Main"
         << "{"
         << "Main()"
         << "{"
         << "}"
         << "}";
    dataMethod = new dataWrittenMethod;
    dataMethod->nameMethod = "Main";
    dataMethod->typeMethod = "";
    dataMethod->parameters << "";
    dataMethods << *dataMethod;
    QTest::newRow("constructorAnnounced") << list  << ListInt{2} << dataMethods;
    delete dataMethod;
}
