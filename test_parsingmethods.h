#ifndef TEST_PARSINGMETHODS_H
#define TEST_PARSINGMETHODS_H

#include <QObject>
#include <QtTest/QtTest>
#include "main.h"

class Test_parsingMethods : public QObject
{
    Q_OBJECT
public:
    Test_parsingMethods();
    ~Test_parsingMethods();

private slots:
    void pparsingMethods();
    void pparsingMethods_data();
};

#endif // TEST_PARSINGMETHODS_H
