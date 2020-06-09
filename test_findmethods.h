#ifndef TEST_FINDMETHODS_H
#define TEST_FINDMETHODS_H

#include <QObject>
#include <QtTest/QtTest>
#include "main.h"

class Test_findMethods : public QObject
{
    Q_OBJECT
public:
    Test_findMethods();
    ~Test_findMethods();

private slots:
    void ffindMethods();
    void ffindMethods_data();
};

#endif // TEST_FINDMETHODS_H
