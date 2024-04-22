#include "xmloutput.h"

xmloutput::xmloutput()
{

}

int xmloutput::exportxml(QList<Node> list, QVector<QVector<int>> v, QVector<QVector<int>> va, int routenum){
    v = deleteroute(v, va, routenum);//删除路线

    QFile filewrite(QApplication::applicationDirPath()+"/export.xml");
    if(!filewrite.open(QFile::WriteOnly|QFile::Truncate))
        return 1;
    QXmlStreamWriter writer(&filewrite);
    //写xml版本号
    writer.setAutoFormatting(true);
    writer.writeStartDocument("1.0");

    //写network标签属性
    writer.writeStartElement("network");
    writer.writeAttribute("locale","C"); //创建属性  其中键值对的值可以是各种类型
    writer.writeAttribute("version","1.6");
    writer.writeAttribute("attribute_processing","explicit");

    //编写appconfig和proconfig
    int countapp = 0;
    foreach(Node n,list)
    {
        if(n.model.contains("次接驳盒"))
        {
            countapp++;
        }
    }

    writer.writeStartElement("node");
    writer.writeAttribute("name","Applications");
    writer.writeAttribute("model","Application Config");
    writer.writeAttribute("ignore_questions","true");
    writer.writeAttribute("min_match_score","strict matching");

    QFile fileaa(":/model/model/appAttribute.csv");
    if(!fileaa.open(QIODevice::ReadOnly))
    {
        qDebug()<<QStringLiteral("请正确选择csv文件");
    }
    else
    {
        QTextStream * read = new QTextStream(&fileaa);
        QStringList Data = read->readAll().split("\n");
        for(int i = 0 ; i < Data.count(); i++)
        {
            QStringList strLine = Data.at(i).split(",");
            writer.writeStartElement("attr");
            writer.writeAttribute("name",strLine[0]);
            writer.writeAttribute("value",strLine[1]);
            writer.writeEndElement();
        }
        delete read;
    }
    fileaa.close();

    writer.writeStartElement("attr");
    writer.writeAttribute("name","Application Definitions.count");
    writer.writeAttribute("value",QString::number(countapp));
    writer.writeEndElement();

    int counta = 0;
    foreach(Node n,list)
    {
        if(n.model.contains("次接驳盒"))
        {
            switch (n.dataType) {
            case Message:
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Name");
                writer.writeAttribute("value","app"+QString::number(counta));
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description.count");
                writer.writeAttribute("value","1");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Email.count");
                writer.writeAttribute("value","1");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Email [0].Send Interarrival Time");
                writer.writeAttribute("value","exponential (720)");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Email [0].Send Group Size");
                writer.writeAttribute("value","constant (3)");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Email [0].Receive Interarrival Time");
                writer.writeAttribute("value","exponential (720)");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Email [0].Receive Group Size");
                writer.writeAttribute("value","constant (3)");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Email [0].E-Mail Size");
                writer.writeAttribute("value","constant ("+n.dataAmount+")");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Email [0].Symbolic Server Name");
                writer.writeAttribute("value","Email Server");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Email [0].Type of Service");
                writer.writeAttribute("value","Best Effort (0)");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Email [0].RSVP Parameters");
                writer.writeAttribute("value","None");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Email [0].Back-End Custom Application");
                writer.writeAttribute("value","Not Used");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                break;
            case Voice:
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Name");
                writer.writeAttribute("value","app"+QString::number(counta));
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description.count");
                writer.writeAttribute("value","1");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Voice.count");
                writer.writeAttribute("value","1");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Voice [0].Silence Length");
                writer.writeAttribute("value","default");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Voice [0].Talk Spurt Length.count");
                writer.writeAttribute("value","1");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Voice [0].Talk Spurt Length [0].Incoming Talk Spurt Length");
                writer.writeAttribute("value","constant ("+n.dataAmount+")");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Voice [0].Talk Spurt Length [0].Outgoing Talk Spurt Length");
                writer.writeAttribute("value","constant ("+n.dataAmount+")");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Voice [0].Symbolic Destination Name");
                writer.writeAttribute("value","Voice Destination");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Voice [0].Encoder Scheme");
                writer.writeAttribute("value","G.711 (silence)");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Voice [0].Voice Frames per Packet");
                writer.writeAttribute("value","1");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Voice [0].Type of Service");
                writer.writeAttribute("value","Interactive Voice (6)");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Voice [0].RSVP Parameters");
                writer.writeAttribute("value","None");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Voice [0].Traffic Mix");
                writer.writeAttribute("value","All Discrete");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Voice [0].Signaling");
                writer.writeAttribute("value","None");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Voice [0].Signaling");
                writer.writeAttribute("value","None");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Voice [0].Compression Delay");
                writer.writeAttribute("value","0.02");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Voice [0].Decompression Delay");
                writer.writeAttribute("value","0.02");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Voice [0].Conversation Environment.count");
                writer.writeAttribute("value","1");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Voice [0].Incoming Conversation Environment");
                writer.writeAttribute("value","Land phone - Quiet room");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Voice [0].Outgoing Conversation Environment");
                writer.writeAttribute("value","Land phone - Quiet room");
                writer.writeEndElement();
                break;
            case Video:
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Name");
                writer.writeAttribute("value","app"+QString::number(counta));
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description.count");
                writer.writeAttribute("value","1");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Video Conferencing.count");
                writer.writeAttribute("value","1");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Video Conferencing [0].Frame Interarrival Time Information");
                writer.writeAttribute("value","15 frames/sec");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Video Conferencing [0].Frame Size Information.count");
                writer.writeAttribute("value","1");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Video Conferencing [0].Frame Size Information [0].Incoming Stream Frame Size");
                writer.writeAttribute("value","constant ("+n.dataAmount+")");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Video Conferencing [0].Frame Size Information [0].Outgoing Stream Frame Size");
                writer.writeAttribute("value","constant ("+n.dataAmount+")");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Video Conferencing [0].Symbolic Destination Name");
                writer.writeAttribute("value","Video Destination");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Video Conferencing [0].Type of Service");
                writer.writeAttribute("value","Best Effort (0)");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Video Conferencing [0].RSVP Parameters");
                writer.writeAttribute("value","None");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Video Conferencing [0].Traffic Mix");
                writer.writeAttribute("value","All Discrete");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                break;
            case Image:
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Name");
                writer.writeAttribute("value","app"+QString::number(counta));
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description.count");
                writer.writeAttribute("value","1");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Print.count");
                writer.writeAttribute("value","1");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Print [0].Print Interarrival Time");
                writer.writeAttribute("value","exponential (360)");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Print [0].File Size");
                writer.writeAttribute("value","constant ("+n.dataAmount+")");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Print [0].Symbolic Printer Name");
                writer.writeAttribute("value","Printer");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Print [0].Type of Service");
                writer.writeAttribute("value","Best Effort (0)");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                break;
            default:
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Name");
                writer.writeAttribute("value","app"+QString::number(counta));
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description.count");
                writer.writeAttribute("value","1");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Email.count");
                writer.writeAttribute("value","1");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Email [0].Send Interarrival Time");
                writer.writeAttribute("value","exponential (720)");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Email [0].Send Group Size");
                writer.writeAttribute("value","constant (3)");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Email [0].Receive Interarrival Time");
                writer.writeAttribute("value","exponential (720)");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Email [0].Receive Group Size");
                writer.writeAttribute("value","constant (3)");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Email [0].E-Mail Size");
                writer.writeAttribute("value","constant ("+n.dataAmount+")");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Email [0].Symbolic Server Name");
                writer.writeAttribute("value","Email Server");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Email [0].Type of Service");
                writer.writeAttribute("value","Best Effort (0)");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Email [0].RSVP Parameters");
                writer.writeAttribute("value","None");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                writer.writeStartElement("attr");
                writer.writeAttribute("name","Application Definitions ["+QString::number(counta)+"].Description [0].Email [0].Back-End Custom Application");
                writer.writeAttribute("value","Not Used");
                writer.writeAttribute("symbolic","true");
                writer.writeEndElement();
                break;
            }
            counta++;
        }
    }

    writer.writeEndElement();

    writer.writeStartElement("node");
    writer.writeAttribute("name","Profiles");
    writer.writeAttribute("model","Profile Config");
    writer.writeAttribute("ignore_questions","true");
    writer.writeAttribute("min_match_score","strict matching");

    QFile filepa(":/model/model/proAttribute.csv");
    if(!filepa.open(QIODevice::ReadOnly))
    {
        qDebug()<<QStringLiteral("请正确选择csv文件");
    }
    else
    {
        QTextStream * read = new QTextStream(&filepa);
        QStringList Data = read->readAll().split("\n");
        for(int i = 0 ; i < Data.count(); i++)
        {
            QStringList strLine = Data.at(i).split(",");
            writer.writeStartElement("attr");
            writer.writeAttribute("name",strLine[0]);
            writer.writeAttribute("value",strLine[1]);
            writer.writeEndElement();
        }
        delete read;
    }
    filepa.close();

    writer.writeStartElement("attr");
    writer.writeAttribute("name","Profile Configuration.count");
    writer.writeAttribute("value",QString::number(countapp));
    writer.writeEndElement();

    for (int i = 0; i < countapp; ++i) {
        writer.writeStartElement("attr");
        writer.writeAttribute("name","Profile Configuration ["+QString::number(i)+"].Profile Name");
        writer.writeAttribute("value","pro"+QString::number(i));
        writer.writeEndElement();
        writer.writeStartElement("attr");
        writer.writeAttribute("name","Profile Configuration ["+QString::number(i)+"].Applications.count");
        writer.writeAttribute("value","1");
        writer.writeAttribute("symbolic","true");
        writer.writeEndElement();
        writer.writeStartElement("attr");
        writer.writeAttribute("name","Profile Configuration ["+QString::number(i)+"].Applications [0].Name");
        writer.writeAttribute("value","app"+QString::number(i));
        writer.writeEndElement();
        writer.writeStartElement("attr");
        writer.writeAttribute("name","Profile Configuration ["+QString::number(i)+"].Applications [0].Start Time Offset");
        writer.writeAttribute("value","uniform (5,10)");
        writer.writeEndElement();
        writer.writeStartElement("attr");
        writer.writeAttribute("name","Profile Configuration ["+QString::number(i)+"].Applications [0].Duration");
        writer.writeAttribute("value","End of Profile");
        writer.writeAttribute("symbolic","true");
        writer.writeEndElement();
        writer.writeStartElement("attr");
        writer.writeAttribute("name","Profile Configuration ["+QString::number(i)+"].Applications [0].Repeatability");
        writer.writeAttribute("value","Unlimited");
        writer.writeAttribute("symbolic","true");
        writer.writeEndElement();
        writer.writeStartElement("attr");
        writer.writeAttribute("name","Profile Configuration ["+QString::number(i)+"].Operation Mode");
        writer.writeAttribute("value","Simultaneous");
        writer.writeAttribute("symbolic","true");
        writer.writeEndElement();
        writer.writeStartElement("attr");
        writer.writeAttribute("name","Profile Configuration ["+QString::number(i)+"].Start Time");
        writer.writeAttribute("value","uniform (100,110)");
        writer.writeEndElement();
        writer.writeStartElement("attr");
        writer.writeAttribute("name","Profile Configuration ["+QString::number(i)+"].Duration");
        writer.writeAttribute("value","End of Simulation");
        writer.writeAttribute("symbolic","true");
        writer.writeEndElement();
        writer.writeStartElement("attr");
        writer.writeAttribute("name","Profile Configuration ["+QString::number(i)+"].Repeatability");
        writer.writeAttribute("value","Once at Start Time");
        writer.writeAttribute("symbolic","true");
        writer.writeEndElement();
    }
    writer.writeStartElement("attr");
    writer.writeAttribute("name","condition");
    writer.writeAttribute("value","enabled");
    writer.writeEndElement();

    writer.writeEndElement();

    counta = 0;
    //逐个读节点属性
    foreach(Node n,list)
    {
        writer.writeStartElement("subnet");
        writer.writeAttribute("name",n.name);

        if(n.model.contains("主接驳盒"))
        {
            writer.writeStartElement("node");
            writer.writeAttribute("name","switch");
            writer.writeAttribute("model","ethernet16_switch");
            writer.writeAttribute("ignore_questions","true");
            writer.writeAttribute("min_match_score","strict matching");

            //node的子标签
            //写x、y坐标
            writer.writeStartElement("attr");
            writer.writeAttribute("name","x position");
            writer.writeAttribute("value",n.xPosition);
            writer.writeEndElement();
            writer.writeStartElement("attr");
            writer.writeAttribute("name","y position");
            writer.writeAttribute("value",n.yPosition);
            writer.writeEndElement();
            //非坐标类子标签，直接从csv文件中导入
            QFile filesa(":/model/model/switchAttribute.csv");
            if(!filesa.open(QIODevice::ReadOnly))
            {
                qDebug()<<QStringLiteral("请正确选择csv文件");
            }
            else
            {
                QTextStream * read = new QTextStream(&filesa);
                QStringList Data = read->readAll().split("\n");
                for(int i = 2 ; i < Data.count(); i++)
                {
                    QStringList strLine = Data.at(i).split(",");
                    writer.writeStartElement("attr");
                    writer.writeAttribute("name",strLine[0]);
                    writer.writeAttribute("value",strLine[1]);
                    writer.writeEndElement();
                }
                delete read;
            }
            writer.writeEndElement();
            filesa.close();

            writer.writeStartElement("attr");
            writer.writeAttribute("name","icon name");
            writer.writeAttribute("value","mjb");
            writer.writeEndElement();
        }else if(n.model.contains("高功率放大器"))
        {
            writer.writeStartElement("node");
            writer.writeAttribute("name","switch");
            writer.writeAttribute("model","ethernet16_switch");
            writer.writeAttribute("ignore_questions","true");
            writer.writeAttribute("min_match_score","strict matching");

            //node的子标签
            //写x、y坐标
            writer.writeStartElement("attr");
            writer.writeAttribute("name","x position");
            writer.writeAttribute("value",n.xPosition);
            writer.writeEndElement();
            writer.writeStartElement("attr");
            writer.writeAttribute("name","y position");
            writer.writeAttribute("value",n.yPosition);
            writer.writeEndElement();
            //非坐标类子标签，直接从csv文件中导入
            QFile filesa(":/model/model/switchAttribute.csv");
            if(!filesa.open(QIODevice::ReadOnly))
            {
                qDebug()<<QStringLiteral("请正确选择csv文件");
            }
            else
            {
                QTextStream * read = new QTextStream(&filesa);
                QStringList Data = read->readAll().split("\n");
                for(int i = 2 ; i < Data.count(); i++)
                {
                    QStringList strLine = Data.at(i).split(",");
                    writer.writeStartElement("attr");
                    writer.writeAttribute("name",strLine[0]);
                    writer.writeAttribute("value",strLine[1]);
                    writer.writeEndElement();
                }
                delete read;
            }
            writer.writeEndElement();
            filesa.close();

            writer.writeStartElement("attr");
            writer.writeAttribute("name","icon name");
            writer.writeAttribute("value","rpt");
            writer.writeEndElement();
        }else if(n.model.contains("分支器"))
        {
            writer.writeStartElement("node");
            writer.writeAttribute("name","switch");
            writer.writeAttribute("model","ethernet16_switch");
            writer.writeAttribute("ignore_questions","true");
            writer.writeAttribute("min_match_score","strict matching");

            //node的子标签
            //写x、y坐标
            writer.writeStartElement("attr");
            writer.writeAttribute("name","x position");
            writer.writeAttribute("value",n.xPosition);
            writer.writeEndElement();
            writer.writeStartElement("attr");
            writer.writeAttribute("name","y position");
            writer.writeAttribute("value",n.yPosition);
            writer.writeEndElement();
            //非坐标类子标签，直接从csv文件中导入
            QFile filesa(":/model/model/switchAttribute.csv");
            if(!filesa.open(QIODevice::ReadOnly))
            {
                qDebug()<<QStringLiteral("请正确选择csv文件");
            }
            else
            {
                QTextStream * read = new QTextStream(&filesa);
                QStringList Data = read->readAll().split("\n");
                for(int i = 2 ; i < Data.count(); i++)
                {
                    QStringList strLine = Data.at(i).split(",");
                    writer.writeStartElement("attr");
                    writer.writeAttribute("name",strLine[0]);
                    writer.writeAttribute("value",strLine[1]);
                    writer.writeEndElement();
                }
                delete read;
            }
            writer.writeEndElement();
            filesa.close();

            writer.writeStartElement("attr");
            writer.writeAttribute("name","icon name");
            writer.writeAttribute("value","spl");
            writer.writeEndElement();
        }else if(n.model.contains("岸站OTN设备"))
        {
            writer.writeStartElement("node");
            writer.writeAttribute("name","server");
            writer.writeAttribute("model","ethernet_server_adv");
            writer.writeAttribute("ignore_questions","true");
            writer.writeAttribute("min_match_score","strict matching");

            //写x、y坐标
            writer.writeStartElement("attr");
            writer.writeAttribute("name","x position");
            writer.writeAttribute("value",n.xPosition);
            writer.writeEndElement();
            writer.writeStartElement("attr");
            writer.writeAttribute("name","y position");
            writer.writeAttribute("value",n.yPosition);
            writer.writeEndElement();

            QFile filesva(":/model/model/serverAttribute.csv");
            if(!filesva.open(QIODevice::ReadOnly))
            {
                qDebug()<<QStringLiteral("请正确选择csv文件");
            }
            else
            {
                QTextStream * read = new QTextStream(&filesva);
                QStringList Data = read->readAll().split("\n");
                for(int i = 2 ; i < Data.count(); i++)
                {
                    QStringList strLine = Data.at(i).split(",");
                    writer.writeStartElement("attr");
                    writer.writeAttribute("name",strLine[0]);
                    writer.writeAttribute("value",strLine[1]);
                    writer.writeEndElement();
                }
                delete read;
            }
            filesva.close();

            writer.writeStartElement("attr");
            writer.writeAttribute("name","Server Address");
            writer.writeAttribute("value","server_"+n.name);
            writer.writeEndElement();

            writer.writeEndElement();

            writer.writeStartElement("node");
            writer.writeAttribute("name","switch");
            writer.writeAttribute("model","ethernet16_switch");
            writer.writeAttribute("ignore_questions","true");
            writer.writeAttribute("min_match_score","strict matching");

            //node的子标签
            //写x、y坐标
            writer.writeStartElement("attr");
            writer.writeAttribute("name","x position");
            writer.writeAttribute("value",n.xPosition);
            writer.writeEndElement();
            writer.writeStartElement("attr");
            writer.writeAttribute("name","y position");
            writer.writeAttribute("value",n.yPosition);
            writer.writeEndElement();
            //非坐标类子标签，直接从csv文件中导入
            QFile filesa(":/model/model/switchAttribute.csv");
            if(!filesa.open(QIODevice::ReadOnly))
            {
                qDebug()<<QStringLiteral("请正确选择csv文件");
            }
            else
            {
                QTextStream * read = new QTextStream(&filesa);
                QStringList Data = read->readAll().split("\n");
                for(int i = 2 ; i < Data.count(); i++)
                {
                    QStringList strLine = Data.at(i).split(",");
                    writer.writeStartElement("attr");
                    writer.writeAttribute("name",strLine[0]);
                    writer.writeAttribute("value",strLine[1]);
                    writer.writeEndElement();
                }
                delete read;
            }
            writer.writeEndElement();
            filesa.close();

            writer.writeStartElement("link");
            writer.writeAttribute("name","server to switch");
            writer.writeAttribute("model","10BaseT");
            writer.writeAttribute("class","duplex");
            writer.writeAttribute("srcNode","switch");
            writer.writeAttribute("destNode","server");
            writer.writeAttribute("ignore_questions","true");
            writer.writeAttribute("min_match_score","strict matching");

            //子标签固有属性
            QFile filela(":/model/model/linkAttribute.csv");
            if(!filela.open(QIODevice::ReadOnly))
            {
                qDebug()<<QStringLiteral("请正确选择csv文件");
            }
            else
            {
                QTextStream * read = new QTextStream(&filela);
                QStringList Data = read->readAll().split("\n");
                for(int i = 0 ; i < Data.count(); i++)
                {
                    QStringList strLine = Data.at(i).split(",");
                    writer.writeStartElement("attr");
                    writer.writeAttribute("name",strLine[0]);
                    writer.writeAttribute("value",strLine[1]);
                    writer.writeEndElement();
                }
                delete read;
            }
            filela.close();
            writer.writeEndElement();

            writer.writeStartElement("attr");
            writer.writeAttribute("name","icon name");
            writer.writeAttribute("value","otn");
            writer.writeEndElement();
        }else if(n.model.contains("次接驳盒"))
        {
            writer.writeStartElement("node");
            writer.writeAttribute("name","client");
            writer.writeAttribute("model","ethernet_wkstn_adv");
            writer.writeAttribute("ignore_questions","true");
            writer.writeAttribute("min_match_score","strict matching");

            //写x、y坐标
            writer.writeStartElement("attr");
            writer.writeAttribute("name","x position");
            writer.writeAttribute("value",n.xPosition);
            writer.writeEndElement();
            writer.writeStartElement("attr");
            writer.writeAttribute("name","y position");
            writer.writeAttribute("value",n.yPosition);
            writer.writeEndElement();

            QFile fileca(":/model/model/clientAttribute.csv");
            if(!fileca.open(QIODevice::ReadOnly))
            {
                qDebug()<<QStringLiteral("请正确选择csv文件");
            }
            else
            {
                QTextStream * read = new QTextStream(&fileca);
                QStringList Data = read->readAll().split("\n");
                for(int i = 2 ; i < Data.count(); i++)
                {
                    QStringList strLine = Data.at(i).split(",");
                    writer.writeStartElement("attr");
                    writer.writeAttribute("name",strLine[0]);
                    writer.writeAttribute("value",strLine[1]);
                    writer.writeEndElement();
                }
                delete read;
            }
            fileca.close();

            writer.writeStartElement("attr");
            writer.writeAttribute("name","Application: Supported Profiles [0].Profile Name");
            writer.writeAttribute("value","pro"+QString::number(counta));
            writer.writeEndElement();
            writer.writeStartElement("attr");
            writer.writeAttribute("name","Application: Transport Protocol Specification");
            if(n.protocol==Udp)
            {
                writer.writeAttribute("value","UDP");
            }else
            {
                writer.writeAttribute("value","TCP");
            }
            writer.writeEndElement();

            writer.writeEndElement();

            writer.writeStartElement("node");
            writer.writeAttribute("name","switch");
            writer.writeAttribute("model","ethernet16_switch");
            writer.writeAttribute("ignore_questions","true");
            writer.writeAttribute("min_match_score","strict matching");

            //node的子标签
            //写x、y坐标
            writer.writeStartElement("attr");
            writer.writeAttribute("name","x position");
            writer.writeAttribute("value",n.xPosition);
            writer.writeEndElement();
            writer.writeStartElement("attr");
            writer.writeAttribute("name","y position");
            writer.writeAttribute("value",n.yPosition);
            writer.writeEndElement();
            //非坐标类子标签，直接从csv文件中导入
            QFile filesa(":/model/model/switchAttribute.csv");
            if(!filesa.open(QIODevice::ReadOnly))
            {
                qDebug()<<QStringLiteral("请正确选择csv文件");
            }
            else
            {
                QTextStream * read = new QTextStream(&filesa);
                QStringList Data = read->readAll().split("\n");
                for(int i = 2 ; i < Data.count(); i++)
                {
                    QStringList strLine = Data.at(i).split(",");
                    writer.writeStartElement("attr");
                    writer.writeAttribute("name",strLine[0]);
                    writer.writeAttribute("value",strLine[1]);
                    writer.writeEndElement();
                }
                delete read;
            }
            writer.writeEndElement();
            filesa.close();

            writer.writeStartElement("link");
            writer.writeAttribute("name","client to switch");
            writer.writeAttribute("model","10BaseT");
            writer.writeAttribute("class","duplex");
            writer.writeAttribute("srcNode","switch");
            writer.writeAttribute("destNode","client");
            writer.writeAttribute("ignore_questions","true");
            writer.writeAttribute("min_match_score","strict matching");

            //子标签固有属性
            QFile filela(":/model/model/linkAttribute.csv");
            if(!filela.open(QIODevice::ReadOnly))
            {
                qDebug()<<QStringLiteral("请正确选择csv文件");
            }
            else
            {
                QTextStream * read = new QTextStream(&filela);
                QStringList Data = read->readAll().split("\n");
                for(int i = 0 ; i < Data.count(); i++)
                {
                    QStringList strLine = Data.at(i).split(",");
                    writer.writeStartElement("attr");
                    writer.writeAttribute("name",strLine[0]);
                    writer.writeAttribute("value",strLine[1]);
                    writer.writeEndElement();
                }
                delete read;
            }
            filela.close();
            writer.writeEndElement();

            writer.writeStartElement("attr");
            writer.writeAttribute("name","icon name");
            writer.writeAttribute("value","acn");
            writer.writeEndElement();

            counta++;
        }

        writer.writeStartElement("attr");
        writer.writeAttribute("name","x position");
        writer.writeAttribute("value",n.xPosition);
        writer.writeEndElement();
        writer.writeStartElement("attr");
        writer.writeAttribute("name","y position");
        writer.writeAttribute("value",n.yPosition);
        writer.writeEndElement();

        QFile filesna(":/model/model/subnetAttribute.csv");
        if(!filesna.open(QIODevice::ReadOnly))
        {
            qDebug()<<QStringLiteral("请正确选择csv文件");
        }
        else
        {
            QTextStream * read = new QTextStream(&filesna);
            QStringList Data = read->readAll().split("\n");
            for(int i = 3 ; i < Data.count(); i++)
            {
                QStringList strLine = Data.at(i).split(",");
                writer.writeStartElement("attr");
                writer.writeAttribute("name",strLine[0]);
                writer.writeAttribute("value",strLine[1]);
                writer.writeEndElement();
            }
            delete read;
        }
        filesna.close();

        writer.writeStartElement("characteristic");
        writer.writeAttribute("name","units");
        writer.writeAttribute("value","Degrees");
        writer.writeEndElement();

        writer.writeStartElement("view-props");

        QFile filesnvp(":/model/model/subnetViewProp.csv");
        if(!filesnvp.open(QIODevice::ReadOnly))
        {
            qDebug()<<QStringLiteral("请正确选择csv文件");
        }
        else
        {
            QTextStream * read = new QTextStream(&filesnvp);
            QStringList Data = read->readAll().split("\n");
            for(int i = 0 ; i < Data.count(); i++)
            {
                QStringList strLine = Data.at(i).split(",");
                writer.writeStartElement("attr");
                writer.writeAttribute("name",strLine[0]);
                writer.writeAttribute("value",strLine[1]);
                writer.writeEndElement();
            }
            delete read;
        }
        writer.writeEndElement();
        filesna.close();

        writer.writeEndElement();
    }

    //写link
    //写名字、源节点和目的节点
    for(int i1=0;i1<list.size();i1++)
    {
        for(int j=i1;j<list.size();j++)
        {
            v1.clear();
            v1 = v.at(i1);
            if(v1.at(j))
            {
                writer.writeStartElement("link");
                writer.writeAttribute("name",list.at(i1).name+" to "+list.at(j).name);
                writer.writeAttribute("srcNode",list.at(i1).name+".switch");
                writer.writeAttribute("destNode",list.at(j).name+".switch");

                //固有属性
                writer.writeAttribute("model","1000BaseX");
                writer.writeAttribute("class","duplex");
                writer.writeAttribute("ignore_questions","true");
                writer.writeAttribute("min_match_score","strict matching");

                //子标签固有属性
                QFile filela(":/model/model/linkAttribute.csv");
                if(!filela.open(QIODevice::ReadOnly))
                {
                    qDebug()<<QStringLiteral("请正确选择csv文件");
                }
                else
                {
                    QTextStream * read = new QTextStream(&filela);
                    QStringList Data = read->readAll().split("\n");
                    for(int i = 0 ; i < Data.count(); i++)
                    {
                        QStringList strLine = Data.at(i).split(",");
                        writer.writeStartElement("attr");
                        writer.writeAttribute("name",strLine[0]);
                        writer.writeAttribute("value",strLine[1]);
                        writer.writeEndElement();
                    }
                    delete read;
                }
                filela.close();

                writer.writeEndElement();
            }
        }
    }

    //    foreach(Node n,list)
    //    {
    //        //写从该节点出发的流量线
    //        //写名字、源节点和目的节点
    //        for(int i=0;i<list.size();i++)
    //        {
    //            v1.clear();
    //            v1 = v.at(countnode);
    //            if(v1.at(i)){
    //                writer.writeStartElement("demand");
    //                writer.writeAttribute("name",n.name+" to "+list.at(i).name);
    //                writer.writeAttribute("model","ip_traffic_flow");
    //                writer.writeAttribute("srcNode",n.name);
    //                writer.writeAttribute("destNode",list.at(i).name);
    //                writer.writeAttribute("ignore_questions","true");
    //                writer.writeAttribute("min_match_score","strict matching");
    //                //子标签变动属性
    //                //Traffic (bits/second),GSM_bps@demand_profiles
    //                switch (n.dataAmount) {
    //                case GSM_AAL5_bps:
    //                    traffic = "GSM_AAL5_bps";
    //                    break;
    //                case GSM_bps:
    //                    traffic = "GSM_bps";
    //                    break;
    //                case G729_bps:
    //                    traffic = "G729_bps";
    //                    break;
    //                case G728_bps:
    //                    traffic = "G728_bps";
    //                    break;
    //                case G729_1user_AAL2_bps:
    //                    traffic = "G729_1user_AAL2_bps";
    //                    break;
    //                case G726Voice_bps:
    //                    traffic = "G726Voice_bps";
    //                    break;
    //                case G711_AAL5_bps:
    //                    traffic = "G711_AAL5_bps";
    //                    break;
    //                case G711Voice_bps:
    //                    traffic = "G711Voice_bps";
    //                    break;
    //                default:
    //                    traffic = "G723Voice_bps";
    //                    break;
    //                }
    //                writer.writeStartElement("attr");
    //                writer.writeAttribute("name","Traffic (bits/second)");
    //                writer.writeAttribute("value",traffic+"@demand_profiles");
    //                writer.writeEndElement();

    //                //Protocol,IP
    //                switch (n.protocol) {
    //                case Udp:
    //                    protocol = "UDP";
    //                    break;
    //                default:
    //                    protocol = "TCP";
    //                    break;
    //                }
    //                writer.writeStartElement("attr");
    //                writer.writeAttribute("name","Protocol");
    //                writer.writeAttribute("value",protocol);
    //                writer.writeEndElement();

    //                //Source Port,0
    //                switch (n.dataType) {
    //                case Voice:
    //                    source = "voice";
    //                    break;
    //                case Image:
    //                    source = "printer";
    //                    break;
    //                case Video:
    //                    source = "video conference";
    //                    break;
    //                default:
    //                    source = "email";
    //                    break;
    //                }
    //                writer.writeStartElement("attr");
    //                writer.writeAttribute("name","Source Port");
    //                writer.writeAttribute("value",source);
    //                writer.writeEndElement();

    //                //子标签固有属性
    //                QFile fileda(":/model/model/demandAttribute.csv");
    //                if(!fileda.open(QIODevice::ReadOnly))
    //                {
    //                    qDebug()<<QStringLiteral("请正确选择csv文件");
    //                }
    //                else
    //                {
    //                    QTextStream * read = new QTextStream(&fileda);
    //                    QStringList Data = read->readAll().split("\n");
    //                    for(int i = 0 ; i < Data.count(); i++)
    //                    {
    //                        QStringList strLine = Data.at(i).split(",");
    //                        writer.writeStartElement("attr");
    //                        writer.writeAttribute("name",strLine[0]);
    //                        writer.writeAttribute("value",strLine[1]);
    //                        writer.writeEndElement();
    //                    }
    //                    delete read;
    //                }
    //                writer.writeEndElement();
    //                fileda.close();
    //            }
    //        }
    //        countnode++;
    //    }

    //整个网络的固有子标签及其属性
    QFile filenwa(":/model/model/networkAttribute.csv");

    if(!filenwa.open(QIODevice::ReadOnly))
    {
        qDebug()<<QStringLiteral("请正确选择csv文件");
    }
    else
    {
        QTextStream * read = new QTextStream(&filenwa);
        QStringList Data = read->readAll().split("\n");
        for(int i = 0 ; i < Data.count(); i++)
        {
            QStringList strLine = Data.at(i).split(",");
            writer.writeStartElement("attr");
            writer.writeAttribute("name",strLine[0]);
            writer.writeAttribute("value",strLine[1]);
            writer.writeEndElement();
        }
        delete read;
    }
    filenwa.close();

    writer.writeStartElement("characteristic");
    writer.writeAttribute("name","units");
    writer.writeAttribute("value","Degrees");
    writer.writeEndElement();

    writer.writeStartElement("view-props");
    QFile filenvpa(":/model/model/netViewAttribute.csv");
    if(!filenvpa.open(QIODevice::ReadOnly))
    {
        qDebug()<<QStringLiteral("请正确选择csv文件");
    }
    else
    {
        QTextStream * read = new QTextStream(&filenvpa);
        QStringList Data = read->readAll().split("\n");
        for(int i = 0 ; i < Data.count(); i++)
        {
            QStringList strLine = Data.at(i).split(",");
            writer.writeStartElement("attr");
            writer.writeAttribute("name",strLine[0]);
            writer.writeAttribute("value",strLine[1]);
            writer.writeEndElement();
        }
        delete read;
    }
    filenvpa.close();

    writer.writeEndElement();
    writer.writeEndDocument();

    //文件关闭
    filewrite.close();

    return 0;
}

QVector<QVector<int>> xmloutput::deleteroute(QVector<QVector<int>> v, QVector<QVector<int>> va, int routenum)
{
    int i1,j1;
    QVector<QVector<int>> vout;
    QVector<int> vout1;
    vout.clear();
    for(int i = 0;i<v.size();i++)
    {
        vout1.clear();
        for(int j = 0;j<v.at(i).size();j++)
        {
            vout1.append(v.at(i).at(j));
        }
        vout.append(vout1);
    }

    if(!routenum)
    {
        return vout;
    }
    for(int i=0;i<routenum;i++)
    {
        for(int j=0;j<va.at(i).size();j++)
        {
            if(j+1<va.at(i).size())
            {
                i1 = va.at(i).at(j);
                j1 = va.at(i).at(j+1);
                vout1.clear();
                vout1 = vout.at(i1);
                vout1.replace(j1,0);
                vout.replace(i1,vout1);
                vout1.clear();
                vout1 = vout.at(j1);
                vout1.replace(i1,0);
                vout.replace(j1,vout1);
            }
        }
    }
    return vout;
}
