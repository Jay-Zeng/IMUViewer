#ifndef TESTS_H
#define TESTS_H

#include "csv.h"
#include "signal.h"
#include <time.h>
#include <iostream>
#include "tableaudebord.h"
//#include <QtGui>
//#include <QtTest/QTest>

using namespace std;

class Tests
{

public:
    Tests();

    static bool testQuaternion();
    static void TdSReechantilonnage();
    static void TdSPasseBas();
    static bool tout();
    static bool testItegration();

};

#endif // TESTS_H
