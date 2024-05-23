#ifndef LINEGRAPHWIDGET_H
#define LINEGRAPHWIDGET_H

#include <QWidget>
#include <QChart>
#include <QChartView>
class lineGraphWidget: public QWidget
{
public:
    lineGraphWidget(QWidget* parent = nullptr);
    QVector<int>* timeSeries;
    QVector<double>* latencySeries;
    QVector<double>* totalThr;
    QVector<double>* dithering;
    QVector<double>* convergence;
    QVector<double>* messageThr;
    QVector<double>* voiceThr;
    QVector<double>* videoThr;
    QVector<double>* printThr;
    QChart* getLatencyChart();
    QChart* getThroughputChart();
    QChart* getDitheringChart();
    QChart* getConvergenceChart();
    QChart* getMessageThrChart();
    QChart* getVoiceThrChart();
    QChart* getVideoThrChart();
    QChart* getImageThrChart();
    QChart* getThrBarChart();
    QChart* getLatencyScatterChart();
    QChartView* cview;

public slots:
    void onLatencyButtonClick();
    void onThroughputButtonClick();
    void onDitheringButtonClick();
    void onConvergenceButtonClick();
    void onMessageBtnClick();
    void onVoiceBthClick();
    void onVideoBthClick();
    void onImageBthClick();
    void onLatencyScatterBtnClick();
    void onThrBarBthClick();
};

#endif // LINEGRAPHWIDGET_H
