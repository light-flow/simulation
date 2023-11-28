#include "resultparser.h"
#include <QFile>
#include <QTextStream>
#include <random>
#include <iostream>

resultparser::resultparser() {}
int resultparser::parserTxtRes(QString filePath,
                                QVector<int>* timeSeries,
                                QVector<double>* latencySeries,
                                QVector<double>* totalThr,
                                QVector<double>* dithering,
                                QVector<double>* convergence,
                                QVector<double>* messageThr,
                                QVector<double>* voiceThr,
                                QVector<double>* videoThr,
                                QVector<double>* printThr)
{
    QFile fileread(filePath);
    if (!fileread.open(QIODevice::ReadOnly | QIODevice::Text))
        return 1;//读取失败
    // 使用文本流读取文件
    QTextStream in(&fileread);

    // 跳过第一行
    in.readLine();

    // 逐行读取并以制表符分割
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList numbers = line.split('\t');
        std::cout << "time length" << timeSeries->size();

        // 处理每个数字
        timeSeries->append(numbers[0].toInt());
        latencySeries->append(numbers[1].toDouble());
        totalThr->append(numbers[2].toDouble());
        messageThr->append(numbers[3].toDouble());
        voiceThr->append(numbers[4].toDouble());
        videoThr->append(numbers[5].toDouble());
        printThr->append(numbers[6].toDouble());
    }

    // 关闭文件
    fileread.close();
    std::cout << "time length" << timeSeries->size();

    // 使用随机数引擎和分布器生成随机数
    std::random_device rd;  // 用于获取真正的随机种子
    std::mt19937 gen(rd()); // Mersenne Twister 伪随机数引擎
    std::uniform_real_distribution<double> distribution(-0.0005, 0.0005); // 均匀

    // 计算抖动
    auto seqLen = latencySeries->size();
    double max = INT_MIN, min = INT_MAX, sum = 0;
    for (int i = 0; i < seqLen; i++) {
        max = max < latencySeries->at(i) ? latencySeries->at(i) : max;
        min = min > latencySeries->at(i) ? latencySeries->at(i) : min;
        sum += latencySeries->at(i);
    }
    double bounce = max - min, mean = sum/seqLen;
    for (int i = 0; i < seqLen; i++) {
        dithering->append(bounce);
        convergence->append(mean);
    }
    return 0;
}
