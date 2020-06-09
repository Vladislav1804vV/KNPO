#ifndef TEST_PARSINGCLASSES_H
#define TEST_PARSINGCLASSES_H

#include <QObject>
#include <QtTest/QtTest>
#include "main.h"

class Test_parsingClasses : public QObject
{
    Q_OBJECT
public:
    Test_parsingClasses();
    ~Test_parsingClasses();

private slots:
    void pparsingClasses();
    void pparsingClasses_data();
};

#endif // TEST_PARSINGCLASSES_H
