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

private:
    struct link{
        QString src;
        QString dest;
    };
    link alink;
    QVector<link> links;
};

#endif // XMLINPUT_H
