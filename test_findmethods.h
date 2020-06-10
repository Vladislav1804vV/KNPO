/*!
 * \file test_findmethods.h
 * \brief файл, содержащий тестирующий класс для функции findMethods
*/

#ifndef TEST_FINDMETHODS_H
#define TEST_FINDMETHODS_H

#include <QObject>
#include <QtTest/QtTest>
#include "main.h"

/*!
 * \class Test_findMethods
 * \brief Тестирующий класс функции findMethods
 */
class Test_findMethods : public QObject
{
    Q_OBJECT
public:
    Test_findMethods();         ///< Конструктор тестирующего класса
    ~Test_findMethods();        ///< Дестуруктор тестирующего класса

private slots:
    void ffindMethods();        ///< Тестовая функция
    void ffindMethods_data();   ///< Функция обеспечивающая данные для тестовой функции
};

#endif // TEST_FINDMETHODS_H
