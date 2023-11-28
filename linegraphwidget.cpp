#include "linegraphwidget.h"
#include <QLineSeries>
#include <QValueAxis>
#include <QtMath>
#include <QGridLayout>
#include <QPushButton>
#include <QSplineSeries>
#include <QApplication>
#include <climits>
#include "resultparser.h"

lineGraphWidget::lineGraphWidget(QWidget* parent): QWidget(parent) {
    // 设置窗口标题，大小，显示状态，非阻塞模态框
    this->setWindowTitle("仿真结果");
    this->setWindowModality(Qt::NonModal);
    this->resize(800, 500);
    // 初始化布局，四个切换按钮
    QGridLayout *layout = new QGridLayout(this);
    QPushButton *latencyBtn = new QPushButton("时延");
    QPushButton *throughputBtn = new QPushButton("总吞吐");
    QPushButton *ditheringBtn = new QPushButton("抖动");
    QPushButton *convergenceBtn = new QPushButton("收敛时间");
    QPushButton *meaageThrBtn = new QPushButton("战术消息吞吐");
    QPushButton *voiceThrBtn = new QPushButton("语音吞吐");
    QPushButton *videoThrBtn = new QPushButton("视频吞吐");
    QPushButton *imageThrBth = new QPushButton("图像吞吐");

    // 初始化图表数据
    timeSeries = new QVector<int>;
    latencySeries = new QVector<double>;
    totalThr = new QVector<double>;
    dithering = new QVector<double>;
    convergence = new QVector<double>;
    messageThr = new QVector<double>;
    voiceThr = new QVector<double>;
    videoThr = new QVector<double>;
    printThr = new QVector<double>;

    resultparser parser;
    int parseSuccess = parser.parserTxtRes(QApplication::applicationDirPath() + "/simulation_result.txt", timeSeries,
                        latencySeries, totalThr, dithering, convergence, messageThr, voiceThr,videoThr, printThr);
    if (parseSuccess != 0) {
        qDebug() << "解析失败";
    }

    // 绑定事件
    connect(latencyBtn, &QPushButton::clicked, this, &lineGraphWidget::onLatencyButtonClick);
    connect(throughputBtn, &QPushButton::clicked, this, &lineGraphWidget::onThroughputButtonClick);
    connect(ditheringBtn, &QPushButton::clicked, this, &lineGraphWidget::onDitheringButtonClick);
    connect(convergenceBtn, &QPushButton::clicked, this, &lineGraphWidget::onConvergenceButtonClick);
    connect(meaageThrBtn, &QPushButton::clicked, this, &lineGraphWidget::onMessageBtnClick);
    connect(voiceThrBtn, &QPushButton::clicked, this, &lineGraphWidget::onVoiceBthClick);
    connect(videoThrBtn, &QPushButton::clicked, this, &lineGraphWidget::onVideoBthClick);
    connect(imageThrBth, &QPushButton::clicked, this, &lineGraphWidget::onImageBthClick);
    //创建图表框架
    this->cview = new QChartView();
    cview->setChart(getLatencyChart());
    // 将控件放置到布局之中
    layout->addWidget(cview, 0, 0, 4, 4);
    layout->addWidget(latencyBtn, 4, 0);
    layout->addWidget(throughputBtn, 4, 1);
    layout->addWidget(ditheringBtn, 4, 2);
    layout->addWidget(convergenceBtn, 4, 3);
    layout->addWidget(meaageThrBtn, 5, 0);
    layout->addWidget(voiceThrBtn, 5, 1);
    layout->addWidget(videoThrBtn, 5, 2);
    layout->addWidget(imageThrBth, 5, 3);
}

void lineGraphWidget::onLatencyButtonClick(){
    this->cview->setChart(getLatencyChart());
}

void lineGraphWidget::onThroughputButtonClick() {
    cview->setChart(getThroughputChart());
}

void lineGraphWidget::onDitheringButtonClick() {
    cview->setChart(getDitheringChart());
}

void lineGraphWidget::onConvergenceButtonClick() {
    cview->setChart(getConvergenceChart());
}

void lineGraphWidget::onMessageBtnClick() {
    cview->setChart(getMessageThrChart());
}

void lineGraphWidget::onVoiceBthClick() {
    cview->setChart(getVoiceThrChart());
}

void lineGraphWidget::onVideoBthClick() {
    cview->setChart(getVideoThrChart());
}

void lineGraphWidget::onImageBthClick() {
    cview->setChart(getImageThrChart());
}

QChart* lineGraphWidget::getLatencyChart() {
    auto series = new QLineSeries;
    int minTime = INT_MAX, maxTime = INT_MIN;
    double minLatency = INT_MAX, maxLat = INT_MIN;
    series->setName(tr("时延"));
    for (int i = 0; i < timeSeries->size(); i++) {
        series->append(timeSeries->at(i), latencySeries->at(i));
        if (minTime > timeSeries->at(i)) minTime = timeSeries->at(i);
        if (maxTime < timeSeries->at(i)) maxTime = timeSeries->at(i);
        if (minLatency > latencySeries->at(i)) minLatency = latencySeries->at(i);
        if (maxLat < latencySeries->at(i)) maxLat = latencySeries->at(i);
    }
    auto chart = new QChart;
    //定义坐标轴
    QValueAxis* xAxis = new QValueAxis();
    QValueAxis* yAxis = new QValueAxis();
    xAxis->setTickCount(11);
    xAxis->setRange(minTime, maxTime);
    xAxis->setLabelFormat("%d");
    xAxis->setTitleText("时间 s");
    xAxis->setTitleFont(QFont("宋体"));
    yAxis->setRange(minLatency, maxLat);
    yAxis->setTitleText("延迟 s");
    yAxis->setTitleFont(QFont("宋体"));
    chart->addAxis(xAxis, Qt::AlignBottom);
    chart->addAxis(yAxis, Qt::AlignLeft);
    chart->addSeries(series);
    series->attachAxis(xAxis);
    series->attachAxis(yAxis);
    return chart;
}

QChart* lineGraphWidget::getThroughputChart() {
    auto series = new QLineSeries;
    series->setName(tr("总吞吐量"));
    int minTime = INT_MAX, maxTime = INT_MIN;
    double minTot = INT_MAX, maxTot = INT_MIN;
    for (int i = 0; i < timeSeries->size(); i++) {
        series->append(timeSeries->at(i), totalThr->at(i));
        if (minTime > timeSeries->at(i)) minTime = timeSeries->at(i);
        if (maxTime < timeSeries->at(i)) maxTime = timeSeries->at(i);
        if (minTot > totalThr->at(i)) minTot = totalThr->at(i);
        if (maxTot < totalThr->at(i)) maxTot = totalThr->at(i);
    }
    auto chart = new QChart;
    // 添加折线
    chart->addSeries(series);
    // 定义坐标轴
    QValueAxis* xAxis = new QValueAxis();
    QValueAxis* yAxis = new QValueAxis();
    xAxis->setTickCount(11);
    xAxis->setRange(minTime, maxTime);
    xAxis->setLabelFormat("%d");
    xAxis->setTitleText("时间 s");
    xAxis->setTitleFont(QFont("宋体"));
    yAxis->setRange(minTot, maxTot);
    yAxis->setLabelFormat("%d");
    yAxis->setTitleText("吞吐量 bytes/s");
    yAxis->setTitleFont(QFont("宋体"));
    chart->addAxis(xAxis, Qt::AlignBottom);
    chart->addAxis(yAxis, Qt::AlignLeft);
    series->attachAxis(xAxis);
    series->attachAxis(yAxis);
    return chart;
}

QChart* lineGraphWidget::getDitheringChart() {
    auto series = new QSplineSeries;
    series->setName(tr("抖动"));
    int minTime = INT_MAX, maxTime = INT_MIN;
    double minLatency = INT_MAX, maxLat = INT_MIN;
    for (int i = 0; i < timeSeries->size(); i++) {
        series->append(timeSeries->at(i), dithering->at(i));
        if (minTime > timeSeries->at(i)) minTime = timeSeries->at(i);
        if (maxTime < timeSeries->at(i)) maxTime = timeSeries->at(i);
        if (minLatency > latencySeries->at(i)) minLatency = latencySeries->at(i);
        if (maxLat < latencySeries->at(i)) maxLat = latencySeries->at(i);
    }
    auto chart = new QChart;
    chart->addSeries(series);
    //定义坐标轴
    QValueAxis* xAxis = new QValueAxis();
    QValueAxis* yAxis = new QValueAxis();
    xAxis->setTickCount(11);
    xAxis->setRange(minTime, maxTime);
    xAxis->setLabelFormat("%d");
    xAxis->setTitleText("时间 s");
    xAxis->setTitleFont(QFont("宋体"));
    yAxis->setRange(minLatency, maxLat);
    yAxis->setLabelFormat("%.4f");
    yAxis->setTitleText("抖动 s");
    yAxis->setTitleFont(QFont("宋体"));
    chart->addAxis(xAxis, Qt::AlignBottom);
    chart->addAxis(yAxis, Qt::AlignLeft);
    series->attachAxis(xAxis);
    series->attachAxis(yAxis);
    return chart;
}

QChart* lineGraphWidget::getConvergenceChart() {
    auto series = new QSplineSeries;
    series->setName(tr("收敛时间"));
    int minTime = INT_MAX, maxTime = INT_MIN;
    double minLatency = INT_MAX, maxLat = INT_MIN;
    for (int i = 0; i < timeSeries->size(); i++) {
        series->append(timeSeries->at(i), convergence->at(i));
        if (minTime > timeSeries->at(i)) minTime = timeSeries->at(i);
        if (maxTime < timeSeries->at(i)) maxTime = timeSeries->at(i);
        if (minLatency > latencySeries->at(i)) minLatency = latencySeries->at(i);
        if (maxLat < latencySeries->at(i)) maxLat = latencySeries->at(i);
    }
    auto chart = new QChart;
    chart->addSeries(series);
    //定义坐标轴
    QValueAxis* xAxis = new QValueAxis();
    QValueAxis* yAxis = new QValueAxis();
    xAxis->setTickCount(11);
    xAxis->setRange(minTime, maxTime);
    xAxis->setLabelFormat("%d");
    xAxis->setTitleText("时间 s");
    xAxis->setTitleFont(QFont("宋体"));
    yAxis->setRange(minLatency, maxLat);
    yAxis->setLabelFormat("%.4f");
    yAxis->setTitleText("收敛时间 s");
    yAxis->setTitleFont(QFont("宋体"));
    chart->addAxis(xAxis, Qt::AlignBottom);
    chart->addAxis(yAxis, Qt::AlignLeft);
    // 关联折线与坐标轴
    series->attachAxis(xAxis);
    series->attachAxis(yAxis);
    return chart;
}

QChart* lineGraphWidget::getMessageThrChart() {
    auto messageSer = new QLineSeries;
    messageSer->setName(tr("战术消息"));
    int minTime = INT_MAX, maxTime = INT_MIN;
    double minMes = INT_MAX, maxMes = INT_MIN;
    for (int i = 0; i < timeSeries->size(); i++) {
        messageSer->append(timeSeries->at(i), messageThr->at(i));
        if (minTime > timeSeries->at(i)) minTime = timeSeries->at(i);
        if (maxTime < timeSeries->at(i)) maxTime = timeSeries->at(i);
        if (minMes > messageThr->at(i)) minMes = messageThr->at(i);
        if (maxMes < messageThr->at(i)) maxMes = messageThr->at(i);
    }
    auto chart = new QChart;
    // 添加折线
    chart->addSeries(messageSer);
    // 定义坐标轴
    QValueAxis* xAxis = new QValueAxis();
    QValueAxis* yAxis = new QValueAxis();
    xAxis->setTickCount(11);
    xAxis->setRange(minTime, maxTime);
    xAxis->setLabelFormat("%d");
    xAxis->setTitleText("时间 s");
    xAxis->setTitleFont(QFont("宋体"));
    yAxis->setRange(minMes, maxMes);
    yAxis->setLabelFormat("%d");
    yAxis->setTitleText("吞吐量 bytes/s");
    yAxis->setTitleFont(QFont("宋体"));
    chart->addAxis(xAxis, Qt::AlignBottom);
    chart->addAxis(yAxis, Qt::AlignLeft);
    messageSer->attachAxis(xAxis);
    messageSer->attachAxis(yAxis);
    return chart;
}

QChart* lineGraphWidget::getVoiceThrChart() {
    auto voiceSer = new QLineSeries;
    voiceSer->setName(tr("语音"));
    int minTime = INT_MAX, maxTime = INT_MIN;
    double minVoi = INT_MAX, maxVoi = INT_MIN;
    for (int i = 0; i < timeSeries->size(); i++) {
        voiceSer->append(timeSeries->at(i), voiceThr->at(i));
        if (minTime > timeSeries->at(i)) minTime = timeSeries->at(i);
        if (maxTime < timeSeries->at(i)) maxTime = timeSeries->at(i);
        if (minVoi > voiceThr->at(i)) minVoi = voiceThr->at(i);
        if (maxVoi < voiceThr->at(i)) maxVoi = voiceThr->at(i);
    }
    auto chart = new QChart;
    // 添加折线
    chart->addSeries(voiceSer);
    // 定义坐标轴
    QValueAxis* xAxis = new QValueAxis();
    QValueAxis* yAxis = new QValueAxis();
    xAxis->setTickCount(11);
    xAxis->setRange(minTime, maxTime);
    xAxis->setLabelFormat("%d");
    xAxis->setTitleText("时间 s");
    xAxis->setTitleFont(QFont("宋体"));
    yAxis->setRange(minVoi, maxVoi);
    yAxis->setLabelFormat("%d");
    yAxis->setTitleText("吞吐量 bytes/s");
    yAxis->setTitleFont(QFont("宋体"));
    chart->addAxis(xAxis, Qt::AlignBottom);
    chart->addAxis(yAxis, Qt::AlignLeft);
    voiceSer->attachAxis(xAxis);
    voiceSer->attachAxis(yAxis);
    return chart;
}

QChart* lineGraphWidget::getVideoThrChart() {
    auto videoSer = new QLineSeries;
    videoSer->setName(tr("视频"));
    int minTime = INT_MAX, maxTime = INT_MIN;
    double minVid = INT_MAX, maxVid = INT_MIN;
    for (int i = 0; i < timeSeries->size(); i++) {
        videoSer->append(timeSeries->at(i), videoThr->at(i));
        if (minTime > timeSeries->at(i)) minTime = timeSeries->at(i);
        if (maxTime < timeSeries->at(i)) maxTime = timeSeries->at(i);
        if (minVid > videoThr->at(i)) minVid = videoThr->at(i);
        if (maxVid < videoThr->at(i)) maxVid = videoThr->at(i);
    }
    auto chart = new QChart;
    // 添加折线
    chart->addSeries(videoSer);
    // 定义坐标轴
    QValueAxis* xAxis = new QValueAxis();
    QValueAxis* yAxis = new QValueAxis();
    xAxis->setTickCount(11);
    xAxis->setRange(minTime, maxTime);
    xAxis->setLabelFormat("%d");
    xAxis->setTitleText("时间 s");
    xAxis->setTitleFont(QFont("宋体"));
    yAxis->setRange(minVid, maxVid);
    yAxis->setLabelFormat("%d");
    yAxis->setTitleText("吞吐量 bytes/s");
    yAxis->setTitleFont(QFont("宋体"));
    chart->addAxis(xAxis, Qt::AlignBottom);
    chart->addAxis(yAxis, Qt::AlignLeft);
    videoSer->attachAxis(xAxis);
    videoSer->attachAxis(yAxis);
    return chart;
}

QChart* lineGraphWidget::getImageThrChart() {
    auto imageSer = new QLineSeries;
    imageSer->setName(tr("图像"));
    int minTime = INT_MAX, maxTime = INT_MIN;
    double minIma = INT_MAX, maxIma = INT_MIN;
    for (int i = 0; i < timeSeries->size(); i++) {
        imageSer->append(timeSeries->at(i), printThr->at(i));
        if (minTime > timeSeries->at(i)) minTime = timeSeries->at(i);
        if (maxTime < timeSeries->at(i)) maxTime = timeSeries->at(i);
        if (minIma > printThr->at(i)) minIma = printThr->at(i);
        if (maxIma < printThr->at(i)) maxIma = printThr->at(i);
    }
    auto chart = new QChart;
    // 添加折线
    chart->addSeries(imageSer);
    // 定义坐标轴
    QValueAxis* xAxis = new QValueAxis();
    QValueAxis* yAxis = new QValueAxis();
    xAxis->setTickCount(11);
    xAxis->setRange(minTime, maxTime);
    xAxis->setLabelFormat("%d");
    xAxis->setTitleText("时间 s");
    xAxis->setTitleFont(QFont("宋体"));
    yAxis->setRange(minIma, maxIma);
    yAxis->setLabelFormat("%d");
    yAxis->setTitleText("吞吐量 bytes/s");
    yAxis->setTitleFont(QFont("宋体"));
    chart->addAxis(xAxis, Qt::AlignBottom);
    chart->addAxis(yAxis, Qt::AlignLeft);
    imageSer->attachAxis(xAxis);
    imageSer->attachAxis(yAxis);
    return chart;
}
