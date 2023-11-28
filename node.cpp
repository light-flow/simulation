#include "node.h"

Node::Node(QString name, QString model, Protocol protocol, DataType dataType, QString dataAmount, QString xPosition, QString yPosition)
{
    this->name = name;
    this->model = model;
    this->protocol = protocol;
    this->dataType = dataType;
    this->dataAmount = dataAmount;
    this->xPosition = xPosition;
    this->yPosition = yPosition;
}
