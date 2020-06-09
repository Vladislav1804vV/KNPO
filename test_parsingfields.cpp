#include "test_parsingfields.h"

Test_parsingFields::Test_parsingFields()
{

}

Test_parsingFields::~Test_parsingFields()
{

}

void Test_parsingFields::pparsingFields()
{
    QFETCH(QStringList, list);
    QFETCH(ListInt, listFields);
    QFETCH(dataField, result);
    dataField dataFields;
    parsingFields(list, listFields, dataFields);
    for(int i = 0; i < dataFields.size(); i++)
    {
       QVERIFY2(dataFields[i].nameField == result[i].nameField, "Test was failed");
       QVERIFY2(dataFields[i].typeField == result[i].typeField, "Test was failed");
       QVERIFY2(dataFields[i].valueField == result[i].valueField, "Test was failed");
       QVERIFY2(dataFields[i].otherModifier == result[i].otherModifier, "Test was failed");
       QVERIFY2(dataFields[i].accessModifier == result[i].accessModifier, "Test was failed");
    }
}

void Test_parsingFields::pparsingFields_data()
{
    QTest::addColumn<QStringList>("list");
    QTest::addColumn<ListInt>("listFields");
    QTest::addColumn<dataField>("result");

    QStringList list;
    dataField dataFields;
    dataWrittenField *dataField;

    //1
    list << "class Main"
         << "{"
         << "int number;"
         << "}";
    dataField = new dataWrittenField;
    dataField->nameField = "number";
    dataField->typeField = "int";
    dataFields << *dataField;
    QTest::newRow("oneField") << list  << ListInt{2} << dataFields;
    delete dataField;

    //2
    list.clear();
    dataFields.clear();
    list << "class Main"
         << "{"
         << "int number;"
         << "int number2;"
         << "int number3;"
         << "}";
    dataField = new dataWrittenField[3];
    dataField[0].nameField = "number";
    dataField[0].typeField = "int";
    dataField[1].nameField = "number2";
    dataField[1].typeField = "int";
    dataField[2].nameField = "number3";
    dataField[2].typeField = "int";
    dataFields << dataField[0] << dataField[1] << dataField[2];
    QTest::newRow("moreThanOneFieldInOtherLine") << list  << ListInt{2,3,4} << dataFields;
    delete[] dataField;

    //3
    list.clear();
    dataFields.clear();
    list << "class Main"
         << "{"
         << "int number, number2, number3;"
         << "}";
    dataField = new dataWrittenField[3];
    dataField[0].nameField = "number";
    dataField[0].typeField = "int";
    dataField[1].nameField = "number2";
    dataField[1].typeField = "int";
    dataField[2].nameField = "number3";
    dataField[2].typeField = "int";
    dataFields << dataField[0] << dataField[1] << dataField[2];
    QTest::newRow("moreThanOneFieldInSameLine") << list  << ListInt{2,2,2} << dataFields;
    delete[] dataField;

    //4
    list.clear();
    dataFields.clear();
    list << "class Main"
         << "{"
         << "int number = 5;"
         << "}";
    dataField = new dataWrittenField;
    dataField->nameField = "number";
    dataField->typeField = "int";
    dataField->valueField = "5";
    dataFields << *dataField;
    QTest::newRow("moreThanOneFieldInSameLine") << list  << ListInt{2} << dataFields;
    delete dataField;

    //5
    list.clear();
    dataFields.clear();
    list << "class Main"
         << "{"
         << "public int number;"
         << "}";
    dataField = new dataWrittenField;
    dataField->nameField = "number";
    dataField->typeField = "int";
    dataField->accessModifier = "public";
    dataFields << *dataField;
    QTest::newRow("fieldHasAccessModifier") << list  << ListInt{2} << dataFields;
    delete dataField;

    //6
    list.clear();
    dataFields.clear();
    list << "class Main"
         << "{"
         << "static int number;"
         << "}";
    dataField = new dataWrittenField;
    dataField->nameField = "number";
    dataField->typeField = "int";
    dataField->otherModifier << "static";
    dataFields << *dataField;
    QTest::newRow("fieldHasOtherModifier") << list  << ListInt{2} << dataFields;
    delete dataField;

    //7
    list.clear();
    dataFields.clear();
    list << "class Main"
         << "{"
         << "static final int number;"
         << "}";
    dataField = new dataWrittenField;
    dataField->nameField = "number";
    dataField->typeField = "int";
    dataField->otherModifier << "static" << "final";
    dataFields << *dataField;
    QTest::newRow("fieldHasMoreThanOneOtherModifier") << list  << ListInt{2} << dataFields;
    delete dataField;

    //8
    list.clear();
    dataFields.clear();
    list << "class Main"
         << "{"
         << "public static int number;"
         << "}";
    dataField = new dataWrittenField;
    dataField->nameField = "number";
    dataField->typeField = "int";
    dataField->accessModifier = "public";
    dataField->otherModifier << "static";
    dataFields << *dataField;
    QTest::newRow("fieldHasAccessModifierAndOtherModifier") << list  << ListInt{2} << dataFields;
    delete dataField;

    //9
    list.clear();
    dataFields.clear();
    list << "class Main"
         << "{"
         << "List<int> number;"
         << "}";
    dataField = new dataWrittenField;
    dataField->nameField = "number";
    dataField->typeField = "List<int>";
    dataFields << *dataField;
    QTest::newRow("fieldHasTypeOfSerialContainers") << list  << ListInt{2} << dataFields;
    delete dataField;

    //10
    list.clear();
    dataFields.clear();
    list << "class Main"
         << "{"
         << "Map<int,String> number;"
         << "}";
    dataField = new dataWrittenField;
    dataField->nameField = "number";
    dataField->typeField = "Map<int,String>";
    dataFields << *dataField;
    QTest::newRow("fieldHasTypeOfAssociativeContainers") << list  << ListInt{2} << dataFields;
    delete dataField;

    //11
    list.clear();
    dataFields.clear();
    list << "class Main"
         << "{"
         << "int[] number;"
         << "}";
    dataField = new dataWrittenField;
    dataField->nameField = "number";
    dataField->typeField = "int[]";
    dataFields << *dataField;
    QTest::newRow("fieldHasTypeOfArrayWhatAnnouncedBeforeItsName") << list  << ListInt{2} << dataFields;
    delete dataField;

    //12
    list.clear();
    dataFields.clear();
    list << "class Main"
         << "{"
         << "int number[];"
         << "}";
    dataField = new dataWrittenField;
    dataField->nameField = "number";
    dataField->typeField = "int[]";
    dataFields << *dataField;
    QTest::newRow("fieldHasTypeOfArrayWhatAnnouncedAfterItsName") << list  << ListInt{2} << dataFields;
    delete dataField;

    //13
    list.clear();
    dataFields.clear();
    list << "class Main"
         << "{"
         << "int[] number[];"
         << "}";
    dataField = new dataWrittenField;
    dataField->nameField = "number";
    dataField->typeField = "int[][]";
    dataFields << *dataField;
    QTest::newRow("fieldHasTypeOfArrayWhatAnnouncedBeforeAndAfterItsName") << list  << ListInt{2} << dataFields;
    delete dataField;

    //14
    list.clear();
    dataFields.clear();
    list << "class Main"
         << "{"
         << "int[] number = {1,2,3};"
         << "}";
    dataField = new dataWrittenField;
    dataField->nameField = "number";
    dataField->typeField = "int[]";
    dataField->valueField = "{1,2,3}";
    dataFields << *dataField;
    QTest::newRow("fieldHasValueWithCommasInBraces") << list  << ListInt{2} << dataFields;
    delete dataField;

    //15
    list.clear();
    dataFields.clear();
    list << "class Main"
         << "{"
         << "int number=5, number2=6, number3=7;"
         << "}";
    dataField = new dataWrittenField[3];
    dataField[0].nameField = "number";
    dataField[0].typeField = "int";
    dataField[0].valueField = "5";
    dataField[1].nameField = "number2";
    dataField[1].typeField = "int";
    dataField[1].valueField = "6";
    dataField[2].nameField = "number3";
    dataField[2].typeField = "int";
    dataField[2].valueField = "7";
    dataFields << dataField[0] << dataField[1] << dataField[2];
    QTest::newRow("moreThanOneFieldInSameLineWithValues") << list  << ListInt{2,2,2} << dataFields;
    delete[] dataField;

    //16
    list.clear();
    dataFields.clear();
    list << "class Main"
         << "{"
         << "int number, number2=6, number3=7;"
         << "}";
    dataField = new dataWrittenField[3];
    dataField[0].nameField = "number";
    dataField[0].typeField = "int";
    dataField[1].nameField = "number2";
    dataField[1].typeField = "int";
    dataField[1].valueField = "6";
    dataField[2].nameField = "number3";
    dataField[2].typeField = "int";
    dataField[2].valueField = "7";
    dataFields << dataField[0] << dataField[1] << dataField[2];
    QTest::newRow("moreThanOneFieldInSameLineWithoutFirstValue") << list  << ListInt{2,2,2} << dataFields;
    delete[] dataField;

    //17
    list.clear();
    dataFields.clear();
    list << "class Main"
         << "{"
         << "String str = \"{,,,}\""
         << "}";
    dataField = new dataWrittenField;
    dataField->nameField = "str";
    dataField->typeField = "String";
    dataField->valueField = "\"{,,,}\"";
    dataFields << *dataField;
    QTest::newRow("fieldHasValueWithCommasInBracesIntoStringConstant") << list  << ListInt{2} << dataFields;
    delete dataField;

    //18
    list.clear();
    dataFields.clear();
    list << "class Main"
         << "{"
         << "String str = \"(,,,)\""
         << "}";
    dataField = new dataWrittenField;
    dataField->nameField = "str";
    dataField->typeField = "String";
    dataField->valueField = "\"(,,,)\"";
    dataFields << *dataField;
    QTest::newRow("fieldHasValueWithCommasInRoundBrackesIntoStringConstant") << list  << ListInt{2} << dataFields;
    delete dataField;
}
