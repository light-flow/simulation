#ifndef XMLOUTPUT_H
#define XMLOUTPUT_H

#include <QList>
#include <QDebug>
#include <QFile>
#include <QtXml>
#include <QApplication>
#include "node.h"

class xmloutput
{
public:
    xmloutput();
    int exportxml(QList<Node> list, QVector<QVector<int>> v, QVector<QVector<int>> va, int routenum);
    QVector<QVector<int>> deleteroute(QVector<QVector<int>> v, QVector<QVector<int>> va, int routenum);//删除链路

private:
    QVector<int> v1;
    QString traffic;
    QString protocol;
    QString source;
};

#endif // XMLOUTPUT_H
