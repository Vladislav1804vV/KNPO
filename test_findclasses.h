#ifndef TEST_FINDCLASSES_H
#define TEST_FINDCLASSES_H

#include <QObject>
#include <QtTest/QtTest>
#include "main.h"

class Test_findClasses : public QObject
{
    Q_OBJECT
public:
    Test_findClasses();
    ~Test_findClasses();

private slots:
    void ffindClasses();
    void ffindClasses_data();
};

#endif // TEST_FINDCLASSES_H
