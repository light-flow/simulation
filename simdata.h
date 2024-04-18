#ifndef SIMDATA_H
#define SIMDATA_H

#include <QList>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QProcess>
#include <QRandomGenerator>

class simdata
{
public:
    simdata();
    int simres(QString projname,QString scenname,QString fibretype,double totaldistance);
};

#endif // SIMDATA_H
