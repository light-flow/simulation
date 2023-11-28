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
    int exportxml(QList<Node> list, QVector<QVector<int>> v);
private:
    QVector<int> v1;
    QString traffic;
    QString protocol;
    QString source;
};

#endif // XMLOUTPUT_H
