#ifndef TEST_PARSINGFIELDS_H
#define TEST_PARSINGFIELDS_H

#include <QObject>
#include <QtTest/QtTest>
#include "main.h"

class Test_parsingFields : public QObject
{
    Q_OBJECT
public:
    Test_parsingFields();
    ~Test_parsingFields();

private slots:
    void pparsingFields();
    void pparsingFields_data();
};

#endif // TEST_PARSINGFIELDS_H
