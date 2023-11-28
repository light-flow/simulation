#include "xmlinput.h"

xmlinput::xmlinput()
{

}

//xml导入分析函数（inputpath为导入xml文件的路径，list为存储节点属性的列表，v为邻接矩阵）
int xmlinput::xmlserial(QString inputpath, QList<Node>* list, QVector<QVector<int>>* v)
{
    //读xml文件
    QFile fileread(inputpath);
    if (!fileread.open(QIODevice::ReadOnly | QIODevice::Text))
        return 1;//读取失败
    QXmlStreamReader reader1(&fileread);

    QList<QString> l1;//暂存节点名称的工具

    QString nodename,modelname,xposition,yposition;//暂存节点名、模型名、x坐标、y坐标
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

                //是否为节点连线
            }else if(reader1.name().toString()=="link")
            {
                //存储到links数组中
                alink.src = reader1.attributes().value("srcNode").toString();
                alink.dest = reader1.attributes().value("destNode").toString();
                links.append(alink);
                //                qDebug()<<alink.src<<" to "<<alink.dest;
            }
        }
        reader1.readNext();
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

    fileread.close();

    return 0;//读取成功

}
