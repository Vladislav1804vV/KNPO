/*!
 * \file test_findfields.h
 * \brief файл, содержащий тестирующий класс для функции findFields
*/

#ifndef TEST_FINDFIELDS_H
#define TEST_FINDFIELDS_H

#include <QObject>
#include <QtTest/QtTest>
#include "main.h"

/*!
 * \class Test_findFields
 * \brief Тестирующий класс функции findFields
 */
class Test_findFields : public QObject
{
    Q_OBJECT
public:
    Test_findFields();          ///< Конструктор тестирующего класса
    ~Test_findFields();         ///< Дестуруктор тестирующего класса

private slots:
    void ffindFields();         ///< Тестовая функция
    void ffindFields_data();    ///< Функция обеспечивающая данные для тестовой функции
};

#endif // TEST_FINDFIELDS_H
