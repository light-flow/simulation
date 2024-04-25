#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "xmlinput.h"
#include "simdata.h"
#include "linegraphwidget.h"
#include <iostream>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
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
    ui->alternate_paths_box->setPlaceholderText(QString("备用路线"));
    ui->current_node->setPlaceholderText(QString("当前节点"));
    ui->network_widget->installEventFilter(this);
    // 设置不同类型设备的颜色
    this->OTN_Color = QColor(1, 31, 187,160);
    this->MainConnect_Color = QColor(1, 131, 250,150);
    this->SecondConnect_Color = QColor(1, 121, 7, 250);
    this->Branch_Color = QColor(1, 250, 7, 250);
    this->PowerControl_Color = QColor(1, 0, 27, 250);

    this->protocolGroup = new QButtonGroup(this);
    this->dataTypeGroup = new QButtonGroup(this);
    this->mediumGroup = new QButtonGroup(this);

    this->protocolGroup->addButton(this->ui->tcp_protocol, 0);
    this->protocolGroup->addButton(this->ui->udp_protocol, 1);
    this->dataTypeGroup->addButton(this->ui->war_message, 0);
    this->dataTypeGroup->addButton(this->ui->voice_message, 1);
    this->dataTypeGroup->addButton(this->ui->image_message, 2);
    this->dataTypeGroup->addButton(this->ui->video_message, 3);
    this->mediumGroup->addButton(this->ui->medium_653, 0);
    this->mediumGroup->addButton(this->ui->medium_652, 1);
    this->mediumGroup->addButton(this->ui->medium_655, 2);

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
                                                                                   //va暂时代替
        x.xmlserial(filename, &this->network_nodes, &this->adj,&alternate_paths,&links);                //这里需要增加一个存储路线的二维数组，我暂时用va代替了
        auto it = this->network_nodes.begin();
        for (int i = 0;it != this->network_nodes.end(); ++it,++i) {
            auto node = *it;
            this->ui->current_node->addItem(node.name, i);
            if (!node.model.contains("次接驳盒") ) {
                this->ui->current_node->setItemData(i, node.name, Qt::UserRole - 1);
            } else {
                lastCanEditIndex = i;
            }
        }
        // 当有备用路线时，将备用路线加入对应的下拉框
        auto size = alternate_paths.size();
        if (size != 0) {
            for (int i = 1; i <= size; i++)
                ui->alternate_paths_box->addItem(QString("备用路线 %1").arg(i));
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
    int flag = output.exportxml(this->network_nodes, this->adj,alternate_paths,
                                selected_alternate_path == -1 ? 0 : selected_alternate_path);                   //暂时填入va和1，va替换成路线的二维矩阵，数字1替换成所选的路线维度，从0开始
    if (flag == 0) {
        QMessageBox::information(this, "导出结果", "导出成功");
    }
}

void MainWindow::paint_network(QList<Node> nodes, int alter_path_index)
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
        } else if (nodes[i].model.contains("次接驳盒")) {
            painter.setBrush(QBrush(this->SecondConnect_Color));
        } else if (nodes[i].model.contains("功率放大")) {
            painter.setBrush(QBrush(this->PowerControl_Color));
        } else if (nodes[i].model.contains("分支器")) {
            painter.setBrush(QBrush(this->Branch_Color));
        }
        painter.setPen(QPen(QColor(255,255,255,0)));
        painter.drawEllipse(c, 15, 15);
        QPen pen(QColor(54, 52, 51));
        pen.setWidth(4);
        painter.setPen(pen);
        painter.setFont(QFont("Consolas",8));
        QPoint tc(c.x()-20,c.y()+20);
        painter.drawText(tc, nodes[i].name);

     }

     painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
     painter.setRenderHint(QPainter::Antialiasing);
     painter.setBrush(Qt::black);

     // 连接节点
     for (int i = 0; i < this->adj.size(); i++) {
        for (int j = 0; j < this->adj[0].size(); j++) {
            if (this->adj[i][j] == 1) {
                painter.drawLine(points[i], points[j]);
            }
        }
     }

     // 绘制备用路径
     painter.setPen(QPen(Qt::red, 3,Qt::SolidLine, Qt::RoundCap));
     painter.setRenderHint(QPainter::Antialiasing);
     painter.setBrush(Qt::red);
     if (alter_path_index != -1) {
         auto path = alternate_paths[alter_path_index];
         for (int i = 0; i < path.size() - 1; i++) {
             int from = path[i];
             int to = path[i + 1];
             painter.drawLine(points[from], points[to]);
         }
     }

     // 绘制图例
     QPen pen(QColor(54, 52, 51));
     pen.setWidth(4);
     painter.setPen(QPen(MainConnect_Color, 3, Qt::SolidLine, Qt::RoundCap));
     painter.drawLine(QPoint(10, 10), QPoint(40, 10));
     painter.setPen(pen);
     painter.setFont(QFont("Consolas",10));
     painter.drawText(QPoint(50, 15), "主接驳盒");

     painter.setPen(QPen(SecondConnect_Color, 3, Qt::SolidLine, Qt::RoundCap));
     painter.drawLine(QPoint(10, 30), QPoint(40, 30));
     painter.setPen(pen);
     painter.setFont(QFont("Consolas",10));
     painter.drawText(QPoint(50, 35), "次接驳盒");

     painter.setPen(QPen(PowerControl_Color, 3, Qt::SolidLine, Qt::RoundCap));
     painter.drawLine(QPoint(10, 50), QPoint(40, 50));
     painter.setPen(pen);
     painter.setFont(QFont("Consolas",10));
     painter.drawText(QPoint(50, 55), "高功率放大器");

     painter.setPen(QPen(Branch_Color, 3, Qt::SolidLine, Qt::RoundCap));
     painter.drawLine(QPoint(10, 70), QPoint(40, 70));
     painter.setPen(pen);
     painter.setFont(QFont("Consolas",10));
     painter.drawText(QPoint(50, 75), "分支器");

     painter.setPen(QPen(OTN_Color, 3, Qt::SolidLine, Qt::RoundCap));
     painter.drawLine(QPoint(10, 90), QPoint(40, 90));
     painter.setPen(pen);
     painter.setFont(QFont("Consolas",10));
     painter.drawText(QPoint(50, 95), "OTN设备");


}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
     if (watched == ui->network_widget
         && event->type() == QEvent::Paint
         && !this->network_nodes.isEmpty()) {
        paint_network(this->network_nodes, this->selected_alternate_path);
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
     this->mediumGroup->button(0)->setChecked(true);
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
     form.addRow(&buttonBox);
     QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
     QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
     if (dialog.exec()  == QDialog::Accepted) {
         simdata sim;
         auto projectName = spinbox1->text();
         auto scenname = spinbox2->text();
         if (sim.simres(projectName, scenname, selected_medium, this->network_nodes,
                        this->adj, alternate_paths, selected_alternate_path == -1 ? 0 : selected_alternate_path,links) == 0)
         {
             QMessageBox::information(this, "仿真结果处理", "处理完成");
             lineGraphWidget* w = new lineGraphWidget();
             w->show();
         } else {
             QMessageBox::information(this, "仿真结果处理", "处理失败");
         }
     }


}


void MainWindow::onDataAmountEditTextChanged(const QString &arg1)
{
    this->current_edit_node->dataAmount = arg1;
    cout << "sucess" << endl;
}


void MainWindow::on_medium_653_clicked()
{
    this->selected_medium = "G.653";
}


void MainWindow::on_medium_652_clicked()
{
     this->selected_medium = "G.652";
}


void MainWindow::on_medium_655_clicked()
{
    this->selected_medium = "G.655";
}


void MainWindow::on_alternate_paths_box_currentIndexChanged(int index)
{
    this->selected_alternate_path = index < alternate_paths.size() ? index : -1;
    this->ui->network_widget->update();
}

