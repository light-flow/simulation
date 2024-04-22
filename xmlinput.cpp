#include "xmlinput.h"

xmlinput::xmlinput()
{

}

//xml导入分析函数（inputpath为导入xml文件的路径，list为存储节点属性的列表，v为邻接矩阵，va为备用路线二维矩阵）
int xmlinput::xmlserial(QString inputpath, QList<Node>* list, QVector<QVector<int>>* v, QVector<QVector<int>>* va)
{
    //读xml文件
    links.clear();
    list->clear();
    v->clear();
    va->clear();
    QFile fileread(inputpath);
    if (!fileread.open(QIODevice::ReadOnly | QIODevice::Text))
        return 1;//读取失败
    QXmlStreamReader reader1(&fileread);

    QList<QString> l1;//暂存节点名称的工具

    QVector<int> va1;//暂存路线的工具

    QString nodename,modelname,xposition,yposition,pointname;//暂存节点名、模型名、x坐标、y坐标、路线节点名
    //逐行读xml
    while(!reader1.atEnd() && !reader1.hasError())
    {
        //判断开始标签
        if(reader1.isStartElement())
        {
            //是否为节点模型
            if(reader1.name().toString()=="node")
            {
                nodename = reader1.attributes().value("name").toString();
                modelname = reader1.attributes().value("model").toString();

                //读写x、y坐标
                reader1.readNext();
                while(!reader1.isStartElement() && !reader1.atEnd() && !reader1.hasError())
                {
                    reader1.readNext();
                }
                xposition = reader1.attributes().value("value").toString();
                reader1.readNext();
                while(!reader1.isStartElement() && !reader1.atEnd() && !reader1.hasError())
                {
                    reader1.readNext();
                }
                yposition = reader1.attributes().value("value").toString();

                //将节点名字和坐标存储到list中
                Node n(nodename, modelname, Tcp, Message, "1000", xposition, yposition);
                list->append(n);
                l1.append(nodename);

                reader1.readNext();

                //是否为节点连线
            }else if(reader1.name().toString()=="link")
            {
                //存储到links数组中
                alink.src = reader1.attributes().value("srcNode").toString();
                alink.dest = reader1.attributes().value("destNode").toString();

                //读写链路长度
                reader1.readNext();
                while(!reader1.isStartElement() && !reader1.atEnd() && !reader1.hasError())
                {
                    reader1.readNext();
                }
                alink.distance = reader1.attributes().value("value").toString();//该链路的长度
                links.append(alink);
                // qDebug()<<alink.src<<" to "<<alink.dest<<" length: "<<alink.distance;

                reader1.readNext();

                //是否为备用路线声明
            }else if(reader1.name().toString()=="route")
            {
                va1.clear();
                reader1.readNext();
                while(!reader1.isStartElement() && !reader1.atEnd() && !reader1.hasError())
                {
                    reader1.readNext();
                }
//                qDebug()<<reader1.name().toString()<<reader1.attributes().value("nodename").toString();
                while(reader1.name().toString()=="point")//路线经过的节点
                {
                    pointname = reader1.attributes().value("nodename").toString();
                    va1.append(l1.indexOf(pointname));
                    reader1.readNext();
                    while(!reader1.isStartElement() && !reader1.atEnd() && !reader1.hasError())
                    {
                        reader1.readNext();
                    }
                }
                va->append(va1);
            }else {
                reader1.readNext();
            }
        }else{
            reader1.readNext();
        }
    }

    QVector<int> v1;//操作二维数组的工具

    //初始化二维数组（初始全为0）
    for(int i = 0;i<list->size();i++)
    {
        v1.append(0);
    }
    for(int j = 0;j<list->size();j++)
    {
        v->append(v1);
    }

    //遍历连线，生成二维邻接矩阵
    int i,j;
    foreach (link lin, links)
    {
        i = l1.indexOf(lin.src);
        j = l1.indexOf(lin.dest);
        v1.clear();
        v1 = v->at(i);
        v1.replace(j,1);
        v->replace(i,v1);
        v1.clear();
        v1 = v->at(j);
        v1.replace(i,1);
        v->replace(j,v1);
    }

    //    for(int i = 0;i<list->size();i++)
    //    {
    //        for(int j = 0;j<list->size();j++)
    //        {
    //            qDebug()<<v->at(i).at(j);
    //        }
    //    }

//    double testdis = caltotaldistance(list,v);
//    qDebug()<<"总距离为："<<testdis;

//    for(int i = 0;i<va->size();i++)
//    {
//        qDebug()<<"第"<<i+1<<"条路线";
//        for(int j = 0;j<va->at(i).size();j++)
//            {
//                qDebug()<<va->at(i).at(j);
//            }
//    }

//    fileread.close();

    return 0;//读取成功

}

double xmlinput::caltotaldistance(QList<Node> list,QVector<QVector<int>> v)//计算总距离
{
    if(links.isEmpty()||v.isEmpty())
    {
        return 0;
    }
    double totaldistance = 0;
    link link1;
    for(int i = 0;i<v.size();i++)
    {
        for(int j = 0;j<v.at(i).size();j++)
        {
            if(v.at(i).at(j))
            {
                link1.src = list.at(i).name;
                link1.dest = list.at(j).name;
                foreach (link lin, links)
                {
                    if(lin.src == link1.src && lin.dest == link1.dest)
                    {
                        //                    qDebug()<<alink.src<<alink.dest<<lin.src<<lin.dest;
                        totaldistance = totaldistance + lin.distance.toDouble();
                        //                    qDebug()<<totaldistance;
                    }
                }
            }
        }
    }
    return totaldistance;
}
