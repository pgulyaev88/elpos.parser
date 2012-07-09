#include <QtGui>
#include <QTimerEvent>
#include <QObject>
#include <QTimer>
#include <QSettings>
#include <QFile>
#include <QEvent>
#include <QDebug>
#include <QRegExp>
#include "parsercore.h"
#include "ui_parsercore.h"

parsercore::parsercore(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::parsercore)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this,SLOT(parsefile()));
    connect(ui->quitButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->startButton,SIGNAL(clicked()),this,SLOT(startParser()));
    connect(ui->stopButton,SIGNAL(clicked()),this,SLOT(stopParser()));
    timer->setInterval(800);
    timer->start();
    int step = 0;
//    parsefile();
}


void parsercore::startParser(){
    timer->start();
    qDebug() << trUtf8("Timer start");
}

void parsercore::stopParser(){
    timer->stop();
    qDebug() << trUtf8("Timer stop");

}

void parsercore::timerEvent(QTimerEvent *event){
    if(event->timerId() == timer->timerId()){
        ++step;
        qDebug() << step;
        qDebug() << "event";
        parsefile();
    } else {
        QObject::timerEvent(event);
        qDebug() << "event";
    }
}

void parsercore::getsettings(){

    QString fileName = "./param.ini";
    QSettings *settings = new QSettings(fileName,QSettings::IniFormat);

    settings->setValue("restaurant/id",1);

    int restId = settings->value("restaurant/id").toInt();
    qDebug() << "Restaurant ID:" << restId;

    QString filePath = settings->value("restaurant/filepath").toString();
    qDebug() << "Restaurant File Path:" << filePath;

    QString restName = settings->value("restaurant/restname").toString();
    qDebug() << "Restaurant Name:" << restName;

}

//static void process_line(const QString &)
//{
//}

void parsercore::parsefile(){
        getsettings();

//        qDebug() << "Start Parsing";
        QFile file("./com1");
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            qDebug() << file.errorString();

        QTextStream in(&file);
        QString line = in.readLine();
//        int x = 0;
        QRegExp rx("(code):(\\d{4}):(\\d{3}|\\d{2}|\\d{1})");
        while(!line.isNull()) {
            line = in.readLine();
            int pos = rx.indexIn(line);
            QStringList list;
            list = rx.capturedTexts();
            QString cap1 = rx.cap(1);
            int cap2 = rx.cap(2).toInt();
            int cap3 = rx.cap(3).toInt();
//            list
//            x++;
//            qDebug() << line;
//            qDebug() << list;
            qDebug() << cap1;
            qDebug() << cap2;
            qDebug() << cap3;
        }
//        qDebug() << x;
    //    clearenv();
}


parsercore::~parsercore()
{
    delete ui;
}
