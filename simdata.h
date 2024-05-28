#ifndef SIMDATA_H
#define SIMDATA_H

#include <QList>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QProcess>
#include <QRandomGenerator>
#include "xlsxdocument.h"

class simdata
{
public:
    simdata();
    int simres(QString projname,QString scenname);
};

#endif // SIMDATA_H
