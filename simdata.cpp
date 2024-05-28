#include "simdata.h"

simdata::simdata()
{

}

int simdata::simres(QString projname,QString scenname){
    QStringList argument;
    QString filePath = QApplication::applicationDirPath() + "\\openetdata.txt";
    QString psName = projname + '-' + scenname;
    argument << "/c" <<"op_cvov" << "-output_file_path" << filePath << "-verbose" << "TRUE" << "-vector_data" << "-vector_data_format" << "H" << "-m" << psName;
    QProcess process(0);
    process.setProgram("cmd");
    process.setArguments(argument);
    process.start();
    process.waitForStarted(); //等待程序启动
    process.waitForFinished();//等待程序关闭

    QVector<QString> time,delay,retrap,setrap,retravc,setravc,retrae,setrae,retrav,setrav;
    // 读取文件位置
    QFile fileread(filePath);
    QFile filewrite(QApplication::applicationDirPath() + "/simulation_result.txt");

    if(!fileread.open(QIODevice::ReadOnly))
    {
        return 1;
    }
    if(!filewrite.open(QFile::WriteOnly|QFile::Truncate))
    {
        return 1;
    }
    // 文件流
    QTextStream streamread(&fileread);
    QTextStream streamwrite(&filewrite);
    // 一行一行的读
    while(!streamread.atEnd())
    {
        QString line = streamread.readLine();
//        int splitindex = line.indexOf("\t");
//        qDebug()<<line.left(splitindex)<<line.right(line.size()-splitindex-1);
        if(line.contains("Delay"))
        {
//            double predelay = 0;
            while(!line.contains("0"))
            {
                line = streamread.readLine();
            }
            while(!line.contains("end"))
            {
                int splitindex = line.indexOf("\t");
                QString value = line.right(line.size()-splitindex-1);
                time.push_back(line.left(splitindex));
                if(value=="Undefined")
                {
//                    double random=QRandomGenerator::global()->bounded(predelay/10);
                    delay.push_back("0");
                }else
                {
                    delay.push_back(value);
//                    predelay = value.toDouble();
                }
                line = streamread.readLine();
            }
        }else if(line.contains("Print.Traffic Received"))
        {
            while(!line.contains("0"))
            {
                line = streamread.readLine();
            }
            while(!line.contains("end"))
            {
                int splitindex = line.indexOf("\t");
                QString value = line.right(line.size()-splitindex-1);
                retrap.push_back(value);
                line = streamread.readLine();
            }
        }else if(line.contains("Print.Traffic Sent"))
        {
            while(!line.contains("0"))
            {
                line = streamread.readLine();
            }
            while(!line.contains("end"))
            {
                int splitindex = line.indexOf("\t");
                QString value = line.right(line.size()-splitindex-1);
                setrap.push_back(value);
                line = streamread.readLine();
            }
        }else if(line.contains("Video Conferencing.Traffic Received"))
        {
            while(!line.contains("0"))
            {
                line = streamread.readLine();
            }
            while(!line.contains("end"))
            {
                int splitindex = line.indexOf("\t");
                QString value = line.right(line.size()-splitindex-1);
                retravc.push_back(value);
                line = streamread.readLine();
            }
        }else if(line.contains("Video Conferencing.Traffic Sent"))
        {
            while(!line.contains("0"))
            {
                line = streamread.readLine();
            }
            while(!line.contains("end"))
            {
                int splitindex = line.indexOf("\t");
                QString value = line.right(line.size()-splitindex-1);
                setravc.push_back(value);
                line = streamread.readLine();
            }
        }else if(line.contains("Email.Traffic Received"))
        {
            while(!line.contains("0"))
            {
                line = streamread.readLine();
            }
            while(!line.contains("end"))
            {
                int splitindex = line.indexOf("\t");
                QString value = line.right(line.size()-splitindex-1);
                retrae.push_back(value);
                line = streamread.readLine();
            }
        }else if(line.contains("Email.Traffic Sent"))
        {
            while(!line.contains("0"))
            {
                line = streamread.readLine();
            }
            while(!line.contains("end"))
            {
                int splitindex = line.indexOf("\t");
                QString value = line.right(line.size()-splitindex-1);
                setrae.push_back(value);
                line = streamread.readLine();
            }
        }else if(line.contains("Voice.Traffic Received"))
        {
            while(!line.contains("0"))
            {
                line = streamread.readLine();
            }
            while(!line.contains("end"))
            {
                int splitindex = line.indexOf("\t");
                QString value = line.right(line.size()-splitindex-1);
                retrav.push_back(value);
                line = streamread.readLine();
            }
        }else if(line.contains("Voice.Traffic Sent"))
        {
            while(!line.contains("0"))
            {
                line = streamread.readLine();
            }
            while(!line.contains("end"))
            {
                int splitindex = line.indexOf("\t");
                QString value = line.right(line.size()-splitindex-1);
                setrav.push_back(value);
                line = streamread.readLine();
            }
        }
    }
    streamwrite<<"time"<<"\t"<<"delay"<<"\t"<<"throughoutput"<<"\t"<<"message.throughoutput"<<"\t"<<"voice.throughoutput"<<"\t"<<"video.throughoutput"<<"\t"<<"print.throughoutput"<<"\t"<<"\n";

    double delaytotal = 0, throutotal = 0, delaymax = INT_MIN, delaymin = INT_MAX;

    for (int i = 0; i < time.size(); ++i) {
        delaymax = delaymax < delay.at(i).toFloat()?delay.at(i).toFloat():delaymax;
        if(delay.at(i).toFloat()>0)
        {
            delaymin = delaymin > delay.at(i).toFloat()?delay.at(i).toFloat():delaymin;
        }
        delaytotal += delay.at(i).toFloat();
        double messagere = retrae.at(i).toFloat();
        double messagese = setrae.at(i).toFloat();
        double message = messagere + messagese;
        double voicere = retrav.at(i).toFloat();
        double voicese = setrav.at(i).toFloat();
        double voice = voicere + voicese;
        double videore = retravc.at(i).toFloat();
        double videose = setravc.at(i).toFloat();
        double video = videore + videose;
        double printre = retrap.at(i).toFloat();
        double printse = setrap.at(i).toFloat();
        double print = printre + printse;
        double throughoutput = message + voice + video + print;
        throutotal += throughoutput;
        streamwrite<<time.at(i)<<"\t"<<delay.at(i)<<"\t"<<throughoutput<<"\t"<<message<<"\t"<<voice<<"\t"<<video<<"\t"<<print<<"\n";
    }

    QXlsx::Document xlsx;//操作Excel

    QXlsx::Format boldFormat;
    boldFormat.setFontBold(true);//加粗

    //写Excel
    xlsx.write("A1", "序号",boldFormat);
    xlsx.write("B1", "评价项目",boldFormat);
    xlsx.write("C1", "值(2位有效小数)",boldFormat);
    xlsx.write("D1", "单位",boldFormat);
    xlsx.write("E1", "参数id",boldFormat);

    xlsx.write("A2", "1");
    xlsx.write("B2", "网络收敛时间");
    xlsx.write("C2", QString::number((delay.at(0).toFloat())*1000, 'f', 2));
    xlsx.write("D2", "ms");
    xlsx.write("E2", "1");

    xlsx.write("A3", "2");
    xlsx.write("B3", "网络延时");
    xlsx.write("C3", QString::number((delaytotal/time.size())*1000, 'f', 2));
    xlsx.write("D3", "ms");
    xlsx.write("E3", "2");

    xlsx.write("A4", "3");
    xlsx.write("B4", "网络抖动");
    xlsx.write("C4", QString::number((delaymax - delaymin)*1000, 'f', 2));
    xlsx.write("D4", "ms");
    xlsx.write("E4", "3");

    xlsx.write("A5", "4");
    xlsx.write("B5", "网络吞吐量");
    xlsx.write("C5", QString::number(throutotal/time.size(), 'f', 2));
    xlsx.write("D5", "bps");
    xlsx.write("E5", "4");

    xlsx.saveAs(QApplication::applicationDirPath()+"/export.xlsx");//保存
    qDebug()<<"export.xlsx生成完毕";


    fileread.close();
    filewrite.close();
    qDebug()<<"simulation_result.txt生成完毕";

    return 0;
}
