#ifndef MAIN_H
#define MAIN_H

#include <QCoreApplication>
#include <fileapi.h>
#include <windows.h>
#include <QStringList>
#include <QDebug>
#include <iostream>
#include <QDir>
#include <QList>
#include <conio.h>
#include <QXmlStreamWriter>
#include <QtTest/QtTest>

#include "test_findclasses.h"
#include "test_findfields.h"
#include "test_findmethods.h"
#include "test_parsingclasses.h"
#include "test_parsingfields.h"
#include "test_parsingmethods.h"

struct dataWrittenClass;
struct dataWrittenField;
struct dataWrittenMethod;

typedef QList<int> ListInt;
typedef QList <dataWrittenClass> dataClass;
typedef QList <dataWrittenField> dataField;
typedef QList <dataWrittenMethod> dataMethod;
Q_DECLARE_METATYPE(ListInt)
Q_DECLARE_METATYPE(dataClass)
Q_DECLARE_METATYPE(dataField)
Q_DECLARE_METATYPE(dataMethod)

using namespace std;

struct dataWrittenField
{
    QString nameField;
    QString typeField;
    QString accessModifier;
    QStringList otherModifier;
    QString valueField;
};

struct dataWrittenMethod
{
    QString nameMethod;
    QString typeMethod;
    QString accessModifier;
    QStringList otherModifier;
    QStringList parameters;
    QString throwsValue;
    QString valueMethod;
    bool isInitializationBlock = false;
    QString typeInitializationBlock;
    QString valueInitializationBlock;
};

struct dataWrittenClass
{
    QString nameClass;
    QString implementsValue;
    QString extendsValue;
    QList <dataWrittenClass> nestedClasses;
    QList <dataWrittenField> fields;
    QList <dataWrittenMethod> methods;
};

/* Разбить программу на языке Java на элементы (классы, методы и т.д.)
 * param [in] filename имя файла, содержащего список путей к файлам программы на языке Java
*/
void JavaParssing(const char *fileName);

/* Открыть заданный файл
 * param [in] fileName имя заданного файла
 * return дескриптор открытого файла
*/
HANDLE openFile(QString fileName);

/* Открыть файлы, содержащиеся в списке путей к файлам программы на языке Java
 * param [in] fileName имя файла, содержащего список путей к файлам программы на языке Java
 * return список дескрипторов открытых файлов
*/
QList <HANDLE> openAllFiles(QString fileName);

/* Получить абсолютный путь файла
 * param [in] fileName имя заданного файла
 * return абсолютный путь заданного файла
*/
QString absolutePath(QString fileName);

/* Обработка текстовых ошибок с последующим завершением работы программы
 * param [in] message текст заданной ошибки
*/
void errorMessage(QString message);

/* Проверка принадлежности файла к расширению языка Java
 * param [in] path путь к заданному файлу (включая его)
*/
void fileExtensionJava(QString path);

/* Проверка принадлежности всех файлов к одному проекту на языке Java
 * param [in] listFiles список путей к файлам программы на языке Java
*/
void sameProject(QStringList listFiles);

/* Открыть файл на языке Java
 * param [in] handle дескриптор открытого файла
 * return массив, содержащий строки кода заданного файла на языке Java
*/
QStringList fileToStringList(HANDLE handle);

/* Открыть файлы на языке Java
 * param [in] listHandle список дескрипторов открытых файлов
 * return список массивов, содержащих строки кода каждого файла на языке Java
*/
QList <QStringList> getFilesArray(QList <HANDLE> listHandle);

/* Проверка на включение в строковую константу от заданного места
 * param [in] list массив, содержащий строки кода заданного файла на языке Java
 * param [in] index индекс строки кода, от которой производить проверку
 * param [in] position позиция символа в заданной строке кода, от которой производить проверку
 * return наличие/отсутствие включения в строковую константу
*/
bool quotedThing(QStringList list, int index, int position);

/* Удалить комментарии в файлах программы на языке Java
 * param [in,out] list список массивов, содержащих строки кода каждого файла на языке Java
*/
void deleteComments(QList <QStringList> &list);

/* Проверка на заключение круглыми скобками от заданного места
 * param [in] list массив, содержащий строки кода заданного файла на языке Java
 * param [in] index индекс строки кода, от которой производить проверку
 * param [in] position позиция символа в заданной строке кода, от которой производить проверку
 * return наличие/отсутствие заключения круглыми скобками
*/
bool thingInRoundBrackets(QStringList list, int index, int position);

/* Удалить аннотации в файлах программы на языке Java
 * param [in,out] list список массивов, содержащих строки кода каждого файла на языке Java
*/
void deleteAnnotations(QList <QStringList> &list);

/* Найти конечный символ от заданного места
 * param [in] list массив, содержащий строки кода заданного файла на языке Java
 * param [in] startPosition индекс строки кода, от которой начинать поиск
 * param [in] indexBrace позиция символа в заданной строке кода, от которой начинать поиск
 * param [in] closeSymbol конечный символ
 * return массив, содержащий индекс строки и позиции найденного конечного символа
*/
QList <int> findClosingSymbol(const QStringList &list, int startPosition, int indexBrace, QChar closeSymbol);

/* Привести заданный код к структурному виду
 * param [in,out] list список массивов, содержащих строки кода каждого файла на языке Java
*/
void structuralViewOfCode(QList <QStringList> &list);

/* Выделить пакеты и импорты для всех заданных файлов программы
 * param [in] list список массивов, содержащих строки кода каждого файла на языке Java
 * param [out] retList список массивов, содержащих пакеты и импорты
*/
void takePackageAndImports(const QList <QStringList> &list, QList <QStringList>& retList);

/* Распарсить (разложить на поля, методы) классы заданного файла программы на языке Java
 * param [in] list массив, содержащий строки кода заданного файла на языке Java
 * param [in] namePackage название пакета заданного файла программы на языке Java
 * param [in] startPos позиция, от которой необходимо начать парсинг классов
 * param [out] dataClasses список синтаксически разложенных классов
*/
void parsingClasses(const QStringList &list, QString namePackage, int startPos, QList <dataWrittenClass> &dataClasses);

/* Найти позиции вхождений классов в заданном диапазоне в строках кода заданного файла программы на языке Java
 * param [in] list массив, содержащий строки кода заданного файла на языке Java
 * param [in] start позиция, от которой необходимо начать поиск
 * param [in] end позиция, до которой необходимо закончить поиск (включительно)
 * return список позиций вхождений классов
*/
QList <int> findClasses(const QStringList &list, int start, int end);

/* Найти позиции вхождений полей в заданном диапазоне в строках кода заданного файла программы на языке Java
 * param [in] list массив, содержащий строки кода заданного файла на языке Java
 * param [in] start позиция, от которой необходимо начать поиск
 * param [in] end позиция, до которой необходимо закончить поиск (включительно)
 * return список позиций вхождений полей
*/
QList <int> findFields(const QStringList &list, int start, int end);

/* Найти позиции вхождений методов в заданном диапазоне в строках кода заданного файла программы на языке Java
 * param [in] list массив, содержащий строки кода заданного файла на языке Java
 * param [in] start позиция, от которой необходимо начать поиск
 * param [in] end позиция, до которой необходимо закончить поиск (включительно)
 * return список позиций вхождений методов
*/
QList <int> findMethods(const QStringList &list, int start, int end);

/* Распарсить (разложить каждое поле на его составляющие) поля заданного файла программы на языке Java
 * param [in] list массив, содержащий строки кода заданного файла на языке Java
 * param [in] listFields список позиций вхождений полей
 * param [in,out] xmlWriter класс для записи файла в формате XML
 * param [out] dataFields список синтаксически разложенных полей
*/
void parsingFields(const QStringList &list, QList <int> &listFields, QList <dataWrittenField> &dataFields);

/* Распарсить (разложить каждый метод на его составляющие) методы заданного файла программы на языке Java
 * param [in] list массив, содержащий строки кода заданного файла на языке Java
 * param [in] listMethods список позиций вхождений методов
 * param [in,out] xmlWriter класс для записи файла в формате XML
 * param [out] dataMethods список синтаксически разложенных методов
*/
void parsingMethods(const QStringList &list, const QList <int> &listMethods, QList <dataWrittenMethod> &dataMethods);

/* Разделить заданную строку по запятой, учитывая отсутствие запятой в строковой константе или заключения в следующих символов: {,}; <,>
 * param [in] value заданная строка
 * param [out] listVal список строк, разделенные по данному принципу
*/
void correctlyBrokenByCommas(QString value, QStringList &listVal);

/* Записать данные в XML формате
 * param [in] dataXML список синтаксически разложенных классов
 * param [in] imports список выделенных "импортов"
*/
void writeToXML(const QList <dataWrittenClass> &dataXML, const QStringList &imports);

/* Записать данные классов в XML формате
 * param [in] dataClass синтаксически разложенный класс
 * param [in,out] xmlWriter класс для записи файла в формате XML
*/
void writeClassToXML(const dataWrittenClass &dataClass, QXmlStreamWriter &xmlWriter);

#endif // MAIN_H
