#ifndef XMLINPUT_H
#define XMLINPUT_H

#include <QList>
#include <QDebug>
#include <QFile>
#include <QtXml>
#include "node.h"

extern QString path;

class xmlinput
{
public:
    xmlinput();
    int xmlserial(QString inputpath, QList<Node>* list, QVector<QVector<int>>* v, QVector<QVector<int>>* va,QVector<link>* links);

    double caltotaldistance(QList<Node> list,QVector<QVector<int>> v,QVector<link> links);//计算总长度

private:
    link alink;
};

#endif // XMLINPUT_H
