/*!
 * \file test_parsingfields.h
 * \brief файл, содержащий тестирующий класс для функции parsingFields
*/

#ifndef TEST_PARSINGFIELDS_H
#define TEST_PARSINGFIELDS_H

#include <QObject>
#include <QtTest/QtTest>
#include "main.h"

/*!
 * \class Test_parsingFields
 * \brief Тестирующий класс функции parsingFields
 */
class Test_parsingFields : public QObject
{
    Q_OBJECT
public:
    Test_parsingFields();           ///< Конструктор тестирующего класса
    ~Test_parsingFields();          ///< Дестуруктор тестирующего класса

private slots:
    void pparsingFields();          ///< Тестовая функция
    void pparsingFields_data();     ///< Функция обеспечивающая данные для тестовой функции
};

#endif // TEST_PARSINGFIELDS_H
