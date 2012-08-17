#include <QtGui>
#include <QtSql>
#include <QTimerEvent>
#include <QObject>
#include <QTimer>
#include <QSettings>
#include <QFile>
#include <QEvent>
#include <QSqlDatabase>
#include <QSqlQueryModel>
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
    getsettings();
    dbcon();
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

    int idRest = settings->value("restaurant/id").toInt();
    qDebug() << "Restaurant ID:" << idRest;

    QString filePath = settings->value("restaurant/filepath").toString();
    qDebug() << "Restaurant File Path:" << filePath;

    QString restName = settings->value("restaurant/restname").toString();
    qDebug() << "Restaurant Name:" << restName;

}

void parsercore::parsefile(){
//        getsettings();

//        qDebug() << "Start Parsing";
        QFile file("./_COM1.LOG");
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
            qDebug() << file.errorString();

        QTextStream in(&file);
        in.setCodec("cp866");
        QString line = in.readLine();
        QRegExp rx("(.*\\S+)\\s+(\\d+)");
//        QRegExp order("(\\S+[Стол|Заказ])\\s+[№|#]\\s+(\\d+)");
//        QRegExp timeorder("(.*\\S+[Время|доставки])\\s+(\\d+:\\d+)");

        while(!line.isNull()) {
            line = in.readLine();
            int pos = rx.indexIn(line);
            QStringList list;
            list = rx.capturedTexts();
            if(!list.isEmpty()){
                pos++;
                QString cap1 = rx.cap(1);
                int cap2 = rx.cap(2).toInt();
                qDebug() << cap1;
                qDebug() << cap2;
                if(cap1!=NULL){
                    qDebug() << cap1;
                    qDebug() << cap2;
//                    insertdb(1,cap1,cap2);
                }
                qDebug() << "Incorrect SQL-query";

            }
//            qDebug() << "List is empty";

        }
        if(file.atEnd())
            file.close();
            timer->stop();
            qDebug() << "Timer stop";
}


void parsercore::dbcon(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("parser");
    db.setUserName("parser");
    db.setPassword("parser");
    if (!db.open()){
        qDebug() << QObject::trUtf8("Database error connect") << db.lastError().text();
    }
}

void parsercore::insertdb(int idRest, QString namefood, int count){

    QString name = namefood;
    int itemcount = count;

    QSqlDatabase::database();
    QSqlQuery *insertdata = new QSqlQuery;
    insertdata->prepare("INSERT INTO orders_details2 (id,name,count) "
                        "VALUES(nextval('orders_details2_id_seq'::regclass), "
                        ":name, :itemcount)");
    insertdata->bindValue(":idRest",'1');
    insertdata->bindValue(":name",name);
    insertdata->bindValue(":itemcount",itemcount);
    insertdata->exec();
    if(insertdata->lastError().isValid())
        qDebug() << insertdata->lastError();
        qDebug() << insertdata->executedQuery();

}

void parsercore::insertIn(int idRest, int code, int count){

    int itemcode = code;
    int itemcount = count;

//    qDebug() << idRest;
//    qDebug() << code;
//    qDebug() << count;
    QSqlDatabase::database();
    QSqlQuery *insertdata = new QSqlQuery;
    insertdata->prepare("INSERT INTO orders_details (id,restid,itemcode,itemcount,insertdate,deleted) "
                        "VALUES(nextval('orders_details_id_seq'::regclass), "
                        ":idRest, :itemcode, :itemcount, now(), false)");
    insertdata->bindValue(":idRest",'1');
    insertdata->bindValue(":itemcode",itemcode);
    insertdata->bindValue(":itemcount",itemcount);
    insertdata->exec();
    if(insertdata->lastError().isValid())
        qDebug() << insertdata->lastError();
}

parsercore::~parsercore()
{
    delete ui;
}
