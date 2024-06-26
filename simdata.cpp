#include "simdata.h"
#include "xmloutput.h"
#include "xmlinput.h"

simdata::simdata()
{

}

int simdata::simres(QString projname,QString scenname,QString fibretype,QList<Node> list,QVector<QVector<int>> v, QVector<QVector<int>> va, int routenum,QVector<link> links){//fibretype光纤类型：G.652（+0.714μs/km）、G.653（5μs/km）、G.655（+0.3125μs/km）
    //totaldistance链路总长度
    xmloutput xo;
    xmlinput xi;
    v = xo.deleteroute(v,va,routenum);//删除非备用路线
    double totaldistance = xi.caltotaldistance(list,v,links);//计算总距离

    //保护倒换
    double trans = 0;
    if(routenum)
    {
        trans = 0.05;
    }

//    qDebug()<<trans;

    QStringList argument;
    QString filePath = QApplication::applicationDirPath() + "\\openetdata.txt";//仿真结果文件路径
    QString psName = projname + '-' + scenname;
    argument << "/c" <<"op_cvov" << "-output_file_path" << filePath << "-verbose" << "TRUE" << "-vector_data" << "-vector_data_format" << "H" << "-m" << psName;//cmd导出仿真结果
    QProcess process(0);
    process.setProgram("cmd");
    process.setArguments(argument);
    process.start();
    process.waitForStarted(); //等待程序启动
    process.waitForFinished();//等待程序关闭

    QVector<QString> time,delay,retrap,setrap,retravc,setravc,retrae,setrae,retrav,setrav;//仿真结果的每一项单独处理
    // 读取文件位置
    QFile fileread(filePath);
    QFile filewrite(QApplication::applicationDirPath() + "/simulation_result.txt");//处理后的仿真结果存储路径

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
        if(line.contains("Delay"))//延时数据
        {
            double difference = totaldistance*(5-3.33)*1e-9;
            if(fibretype == "G.652") {//传播延时换算（根据介质速率换算）
                difference = difference+totaldistance*0.714*1e-9;
            }else if(fibretype == "G.655"){
                difference = difference+totaldistance*0.3125*1e-9;
            }else{
                difference = difference;
            }
            double amplifier = (totaldistance/80000)*0.25*1e-6;//放大器延时，每80公里一个
//            qDebug()<<difference;
            double compensation = difference+amplifier+trans+100*1e-6;//每个设备OTU的100微秒延时
            while(!line.contains("0"))//从0时开始
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
                    delay.push_back(QString::number(compensation));
                }else
                {
                    compensation = value.toDouble() + compensation;
                    delay.push_back(QString::number(compensation));
//                    predelay = value.toDouble();
                }
                line = streamread.readLine();
            }
        }else if(line.contains("Print.Traffic Received"))//print类型的收traffic
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
    //做成表格的形式
    streamwrite<<"time"<<"\t"<<"delay"<<"\t"<<"throughoutput"<<"\t"<<"message.throughoutput"<<"\t"<<"voice.throughoutput"<<"\t"<<"video.throughoutput"<<"\t"<<"print.throughoutput"<<"\t"<<"\n";
    for (int i = 0; i < time.size(); ++i) {
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
        streamwrite<<time.at(i)<<"\t"<<delay.at(i)<<"\t"<<throughoutput<<"\t"<<message<<"\t"<<voice<<"\t"<<video<<"\t"<<print<<"\n";
    }
    fileread.close();
    filewrite.close();
    qDebug()<<"simulation_result.txt生成完毕";

    return 0;
}
