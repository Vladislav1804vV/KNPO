/*!
 * \file test_findclasses.h
 * \brief файл, содержащий тестирующий класс для функции findClasses
*/

#ifndef TEST_FINDCLASSES_H
#define TEST_FINDCLASSES_H

#include <QObject>
#include <QtTest/QtTest>
#include "main.h"

/*!
 * \class Test_findClasses
 * \brief Тестирующий класс функции findClasses
 */
class Test_findClasses : public QObject
{
    Q_OBJECT
public:
    Test_findClasses();         ///< Конструктор тестирующего класса
    ~Test_findClasses();        ///< Дестуруктор тестирующего класса

private slots:
    void ffindClasses();        ///< Тестовая функция
    void ffindClasses_data();   ///< Функция обеспечивающая данные для тестовой функции
};

#endif // TEST_FINDCLASSES_H
