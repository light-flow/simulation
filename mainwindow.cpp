#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "xmlinput.h"
#include "simdata.h"
#include "linegraphwidget.h"
#include <iostream>
#include <QPainter>
#include <QFileDialog>
#include <QDialog>
#include "xmloutput.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>

using namespace std;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString button_style = "background-color: rgb(6, 72, 120);color:white;border-radius: 4px;";
    // 设置按钮样式
    ui->import_config->setStyleSheet(button_style);
    ui->generate_project->setStyleSheet(button_style);
    ui->process_result_button->setStyleSheet(button_style);
    // 设置下拉框样式
    ui->current_node->setStyleSheet(button_style);
    ui->network_widget->installEventFilter(this);
    ui->network_widget->setStyleSheet("background-color: rgb(255, 255, 255);color: white;hight:200px;width:300px");
    ui->network_widget->update();
    // 设置不同类型设备的颜色
    this->OTN_Color = QColor(1, 31, 187,160);
    this->MainConnect_Color = QColor(1, 131, 250,150);
    this->SecondConnect_Color = QColor(1, 121, 7, 250);
    this->Branch_Color = QColor(1, 250, 7, 250);
    this->PowerControl_Color = QColor(1, 0, 27, 250);

    this->protocolGroup = new QButtonGroup(this);
    this->dataTypeGroup = new QButtonGroup(this);
    this->protocolGroup->addButton(this->ui->tcp_protocol, 0);
    this->protocolGroup->addButton(this->ui->udp_protocol, 1);
    this->dataTypeGroup->addButton(this->ui->war_message, 0);
    this->dataTypeGroup->addButton(this->ui->voice_message, 1);
    this->dataTypeGroup->addButton(this->ui->image_message, 2);
    this->dataTypeGroup->addButton(this->ui->video_message, 3);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_import_config_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "选择配置文件", "C:/", "*.xml");
    int lastCanEditIndex = -1;
    if (!filename.isEmpty()) {
        cout << filename.toStdString() << endl;
        xmlinput x;
        x.xmlserial(filename, &this->network_nodes, &this->adj);
        auto it = this->network_nodes.begin();
        for (int i = 0;it != this->network_nodes.end(); ++it,++i) {
            auto node = *it;
            this->ui->current_node->addItem(node.name, i);
            if (!node.model.contains("次接驳盒")&&!node.model.contains("预警探测光纤阵")&&!node.model.contains("UUV支持节点")&&!node.model.contains("海洋观测节点")&&!node.model.contains("水声通信节点")&&!node.model.contains("YW")&&!node.model.contains("浮标")) {
                this->ui->current_node->setItemData(i, node.name, Qt::UserRole - 1);
            } else {
                lastCanEditIndex = i;
            }
            cout << node.model.toStdString() <<endl;
        }
    }
    if (lastCanEditIndex != -1) {
        this->ui->current_node->setCurrentIndex(lastCanEditIndex);
        ui->network_widget->update();
    }
}


void MainWindow::on_generate_project_clicked()
{
    xmloutput output;
    int flag = output.exportxml(this->network_nodes, this->adj);
    if (flag == 0) {
        auto box = getMessageBox(this, "导出结果", "导出成功");
        box->exec();
        // QMessageBox::information(this, "导出结果", "导出成功");
    }
}

void MainWindow::paint_network(QList<Node> nodes)
{
     if (nodes.isEmpty()) return;
     auto width = this->ui->network_widget->width();
     auto height = this->ui->network_widget->height();
     auto radius = height/2 - 20;
     float delta =  (360.0/(nodes.size()))*3.14159/180.0;

     QPoint center(width/2, height/2);

     QPainter painter(this->ui->network_widget);

     QVector<QPoint> points;

     // 绘制节点
     for (int i = 0; i < nodes.size(); i++) {
        QPoint c;
        c.setX(center.x() + radius*sin(delta*i));
        c.setY(center.y() + radius*cos(delta*i));
        points.append(c);
        if (nodes[i].model.contains("OTN")) {
            painter.setBrush(QBrush(this->OTN_Color));
        } else if (nodes[i].model.contains("主接驳盒")) {
            painter.setBrush(QBrush(this->MainConnect_Color));
        } else if (nodes[i].model.contains("次接驳盒")||nodes[i].model.contains("预警探测光纤阵")||nodes[i].model.contains("UUV支持节点")||nodes[i].model.contains("海洋观测节点")||nodes[i].model.contains("水声通信节点")||nodes[i].model.contains("YW")||nodes[i].model.contains("浮标")) {
            painter.setBrush(QBrush(this->SecondConnect_Color));
        } else if (nodes[i].model.contains("功率放大")) {
            painter.setBrush(QBrush(this->PowerControl_Color));
        } else if (nodes[i].model.contains("分支器")) {
            painter.setBrush(QBrush(this->Branch_Color));
        }else{//默认值为分支器
            painter.setBrush(QBrush(this->Branch_Color));
        }
        painter.setPen(QPen(QColor(255,255,255,0)));
        painter.drawEllipse(c, 15, 15);
        QPen pen(QColor(255, 255, 255));
        pen.setWidth(4);
        painter.setPen(pen);
        painter.setFont(QFont("Consolas",8));
        QPoint tc(c.x()-20,c.y()+20);
        painter.drawText(tc, nodes[i].name);
         cout << 3 <<endl;
     }

     painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
     painter.setRenderHint(QPainter::Antialiasing);
     painter.setBrush(Qt::black);

     // 连接节点
     for (int i = 0; i < this->adj.size(); i++) {
        for (int j = 0; j < this->adj[0].size(); j++) {
            if (this->adj[i][j] == 1) {
                painter.drawLine(points[i], points[j]);
                cout << 4 <<endl;
            }
        }
     }

     // 绘制图例
     QPen pen(QColor(255, 255, 255));
     pen.setWidth(4);
     painter.setBrush(QBrush(this->MainConnect_Color));
     painter.setPen(QPen(MainConnect_Color));
     // painter.drawLine(QPoint(10, 10), QPoint(40, 10));
     painter.drawEllipse(QPoint(25, 10), 8, 8);
     painter.setPen(pen);
     painter.setFont(QFont("Consolas",10));
     painter.drawText(QPoint(40, 15), "主接驳盒");

     painter.setBrush(QBrush(this->SecondConnect_Color));
     painter.setPen(QPen(SecondConnect_Color));
     // painter.drawLine(QPoint(10, 30), QPoint(40, 30));
     painter.drawEllipse(QPoint(25, 30), 8, 8);
     painter.setPen(pen);
     painter.setFont(QFont("Consolas",10));
     painter.drawText(QPoint(40, 35), "业务节点");//次接驳盒改称为业务节点

     painter.setBrush(QBrush(this->PowerControl_Color));
     painter.setPen(QPen(PowerControl_Color));
     painter.drawEllipse(QPoint(25, 50), 8, 8);
     painter.setPen(pen);
     painter.setFont(QFont("Consolas",10));
     painter.drawText(QPoint(40, 55), "高功率放大器");

     painter.setBrush(QBrush(this->Branch_Color));
     painter.setPen(QPen(Branch_Color));
     painter.drawEllipse(QPoint(25, 70), 8, 8);
     painter.setPen(pen);
     painter.setFont(QFont("Consolas",10));
     painter.drawText(QPoint(40, 75), "分支器");

     painter.setBrush(QBrush(this->OTN_Color));
     painter.setPen(QPen(OTN_Color));
     painter.drawEllipse(QPoint(25, 90), 8, 8);
     painter.setPen(pen);
     painter.setFont(QFont("Consolas",10));
     painter.drawText(QPoint(40, 95), "OTN设备");


}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
     if (watched == ui->network_widget
         && event->type() == QEvent::Paint
         && !this->network_nodes.isEmpty()) {
        paint_network(this->network_nodes);
     }
     return true;
}

void MainWindow::set_panel(Node node)
{
    if (node.dataType == DataType::Voice) {
        this->ui->amountUnit->setText("seconds");
    } else {
        this->ui->amountUnit->setText("bytes");
    }
     this->ui->current_node_name->setText(node.name);
     this->protocolGroup->button(node.protocol)->setChecked(true);
     this->dataTypeGroup->button(node.dataType)->setChecked(true);
     this->ui->dataAmountEdit->setText(node.dataAmount);
     this->ui->XPosition_label->setText(node.xPosition);
     this->ui->YPosition_label->setText(node.yPosition);
}

void MainWindow::on_current_node_currentIndexChanged(int index)
{
     this->current_edit_node = &network_nodes[index];
     this->set_panel(this->network_nodes[index]);
}


void MainWindow::on_tcp_protocol_clicked()
{
     this->current_edit_node->protocol = Protocol::Tcp;
}


void MainWindow::on_udp_protocol_clicked()
{
     this->current_edit_node->protocol = Protocol::Udp;
}


void MainWindow::on_war_message_clicked()
{
     this->current_edit_node->dataType = DataType::Message;
     this->ui->amountUnit->setText("bytes");
}


void MainWindow::on_voice_message_clicked()
{
     this->current_edit_node->dataType = DataType::Voice;
     this->ui->amountUnit->setText("seconds");
}



void MainWindow::on_image_message_clicked()
{
     this->current_edit_node->dataType = DataType::Image;
     this->ui->amountUnit->setText("bytes");

}


void MainWindow::on_video_message_clicked()
{
     this->current_edit_node->dataType = DataType::Video;
     this->ui->amountUnit->setText("bytes");
}


void MainWindow::on_process_result_button_clicked()
{
     QDialog dialog(this);
     dialog.setWindowTitle("仿真结果处理");
     dialog.setStyleSheet("color: white;");
     QFormLayout form(&dialog);
     form.addRow(new QLabel("请输入项目名与场景名"));
     // Value1
     QString value1 = QString("项目名: ");
     auto *spinbox1 = new QLineEdit(&dialog);
     form.addRow(value1, spinbox1);
     // Value2
     QString value2 = QString("场景名: ");
     auto *spinbox2 = new QLineEdit(&dialog);
     form.addRow(value2, spinbox2);
     // Add Cancel and OK button
     QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                Qt::Horizontal, &dialog);
     buttonBox.setStyleSheet("background-color: rgb(6, 72, 120);color:white;border-radius: 1px;");
     form.addRow(&buttonBox);
     QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
     QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
     if (dialog.exec()  == QDialog::Accepted) {
         simdata sim;
         auto projectName = spinbox1->text();
         auto scenname = spinbox2->text();
         if (sim.simres(projectName, scenname) == 0) {
             // QMessageBox::information(this, "仿真结果处理", "处理完成");
             auto box = getMessageBox(this, "仿真结果处理", "处理完成");
             box->exec();
             lineGraphWidget* w = new lineGraphWidget();
             w->show();
         } else {
             // QMessageBox::information(this, "仿真结果处理", "处理失败");
             auto box = getMessageBox(this, "仿真结果处理", "处理失败");
             box->exec();
         }
     }
}


void MainWindow::onDataAmountEditTextChanged(const QString &arg1)
{
    this->current_edit_node->dataAmount = arg1;
    cout << "sucess" << endl;
}

QMessageBox* MainWindow::getMessageBox(QWidget *parent, QString title, QString text)
{
    QMessageBox* box = new QMessageBox(parent);
    QPushButton* okbtn = new QPushButton("Ok");
    okbtn->setStyleSheet("background-color: rgb(6, 72, 120);color:white;border-radius: 1px;");
    QString styleStr = "color: white;";
    box->setWindowTitle(title);
    box->setText(text);
    box->setStyleSheet(styleStr);
    box->addButton(okbtn, QMessageBox::AcceptRole);
    // box.addButton("OK", QMessageBox::AcceptRole);
    // box.addButton("Cancel", QMessageBox::RejectRole);
    return box;
}
