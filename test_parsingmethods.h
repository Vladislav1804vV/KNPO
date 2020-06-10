/*!
 * \file test_parsingmethods.h
 * \brief файл, содержащий тестирующий класс для функции parsingMethods
*/

#ifndef TEST_PARSINGMETHODS_H
#define TEST_PARSINGMETHODS_H

#include <QObject>
#include <QtTest/QtTest>
#include "main.h"

/*!
 * \class Test_parsingMethods
 * \brief Тестирующий класс функции parsingMethods
 */
class Test_parsingMethods : public QObject
{
    Q_OBJECT
public:
    Test_parsingMethods();          ///< Конструктор тестирующего класса
    ~Test_parsingMethods();         ///< Дестуруктор тестирующего класса

private slots:
    void pparsingMethods();         ///< Тестовая функция
    void pparsingMethods_data();    ///< Функция обеспечивающая данные для тестовой функции
};

#endif // TEST_PARSINGMETHODS_H
