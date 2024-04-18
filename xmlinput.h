#ifndef XMLINPUT_H
#define XMLINPUT_H

#include <QList>
#include <QDebug>
#include <QFile>
#include <QtXml>
#include "node.h"

class xmlinput
{
public:
    xmlinput();
    int xmlserial(QString inputpath, QList<Node>* list, QVector<QVector<int>>* v);
    int caltotaldistance(QVector<QVector<int>>* v);//计算总长度

private:
    struct link{
        QString src;
        QString dest;
        QString distance;//链路的长度
    };
    link alink;
    QVector<link> links;
};

#endif // XMLINPUT_H
