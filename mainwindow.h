#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "node.h"
#include <Qvector>
#include <QButtonGroup>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void paint_network(QList<Node> nodes);
    void set_panel(Node node);
    QColor OTN_Color;
    QColor MainConnect_Color;
    QColor SecondConnect_Color;
    QColor Branch_Color;
    QColor PowerControl_Color;
    ~MainWindow();

private slots:

    void on_import_config_clicked();

    void on_generate_project_clicked();


    void on_current_node_currentIndexChanged(int index);

    void on_tcp_protocol_clicked();

    void on_udp_protocol_clicked();

    void on_war_message_clicked();

    void on_voice_message_clicked();


    void on_image_message_clicked();

    void on_video_message_clicked();

    void on_process_result_button_clicked();

    void onDataAmountEditTextChanged(const QString &arg1);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::MainWindow *ui;
    QList<Node> network_nodes;
    QVector<QVector<int>> adj;
    QButtonGroup* protocolGroup;
    QButtonGroup* dataTypeGroup;
    Node * current_edit_node;
};
#endif // MAINWINDOW_H
