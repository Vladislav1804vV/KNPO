#ifndef TEST_FINDFIELDS_H
#define TEST_FINDFIELDS_H

#include <QObject>
#include <QtTest/QtTest>
#include "main.h"

class Test_findFields : public QObject
{
    Q_OBJECT
public:
    Test_findFields();
    ~Test_findFields();

private slots:
    void ffindFields();
    void ffindFields_data();
};

#endif // TEST_FINDFIELDS_H
