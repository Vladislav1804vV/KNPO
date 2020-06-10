/*!
 * \file test_parsingclasses.h
 * \brief файл, содержащий тестирующий класс для функции parsingClasses
*/

#ifndef TEST_PARSINGCLASSES_H
#define TEST_PARSINGCLASSES_H

#include <QObject>
#include <QtTest/QtTest>
#include "main.h"

/*!
 * \class Test_parsingClasses
 * \brief Тестирующий класс функции parsingClasses
 */
class Test_parsingClasses : public QObject
{
    Q_OBJECT
public:
    Test_parsingClasses();          ///< Конструктор тестирующего класса
    ~Test_parsingClasses();         ///< Дестуруктор тестирующего класса

private slots:
    void pparsingClasses();         ///< Тестовая функция
    void pparsingClasses_data();    ///< Функция обеспечивающая данные для тестовой функции
};

#endif // TEST_PARSINGCLASSES_H
