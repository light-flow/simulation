#ifndef RESULTPARSER_H
#define RESULTPARSER_H
#include <QString>
#include <QVector>

class resultparser
{
public:
    resultparser();
    int parserTxtRes(QString filePath, QVector<int>* timeSeries, QVector<double>* latencySeries, QVector<double>* totalThr, QVector<double>* dithering, QVector<double>* convergence, QVector<double>* messageThr, QVector<double>* voiceThr, QVector<double>* videoThr, QVector<double>* printThr);

};

#endif // RESULTPARSER_H
