#ifndef SIMDATA_H
#define SIMDATA_H

#include <QList>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QProcess>
#include <QRandomGenerator>
#include "node.h"

class simdata
{
public:
    simdata();
    int simres(QString projname,QString scenname,QString fibretype,QList<Node> list,QVector<QVector<int>> v, QVector<QVector<int>> va, int routenum);
};

#endif // SIMDATA_H
