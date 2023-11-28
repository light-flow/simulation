#ifndef NODE_H
#define NODE_H
#include <QString>

enum Protocol{
    Tcp,
    Udp
};

enum DataType{
    Message,
    Voice,
    Image,
    Video
};

class Node
{
public:
    Node(QString name, QString model, Protocol protocol, DataType dataType, QString dataAmount, QString xPosition, QString yPosition);
    QString name;
    QString model;
    Protocol protocol;
    DataType dataType;
    QString dataAmount;//单位bytes或seconds(voice时)
    QString xPosition;
    QString yPosition;
};

#endif // NODE_H

