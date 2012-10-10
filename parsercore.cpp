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
    connect(ui->hideButton,SIGNAL(clicked()),this,SLOT(hideToTray()));
    getsettings();
    dbcon();
    getlastitem();
    timer->setInterval(1500);
    timer->start();
}

void parsercore::hideToTray(){

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

//

    idRest = settings->value("restaurant/id").toInt();
    if(settings->value("restaurant/id").isNull()){
       settings->setValue("restaurant/id",1);
    }
    qDebug() << "Restaurant ID:" << idRest;

    filePath = settings->value("restaurant/filepath").toString();
    if(settings->value("restaurant/filepath").isNull()){
        settings->setValue("restaurant/filepath","C:\\TMP");
    }
    qDebug() << "Restaurant File Path:" << filePath;

    tmpPath = settings->value("restaurant/tmppath").toString();
    if(settings->value("restaurant/tmppath").isNull()){
        settings->setValue("restaurant/tmppath","C:\\TMP");
    }
    qDebug() << "Restaurant File Path:" << tmpPath;

    restName = settings->value("restaurant/restname").toString();
    if(settings->value("restaurant/restname").isNull()){
        settings->setValue("restaurant/restname","Rest");
    }
    qDebug() << "Restaurant Name:" << restName;

}

void parsercore::parsefile(){
//        getsettings();

//        qDebug() << "Start Parsing";
        QDir dir(tmpPath);
        QFile file(dir.filePath("_COM1.LOG"));

        if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
            qDebug() << file.errorString();

        QTextStream in(&file);
        in.setCodec("cp866");
        QString line = in.readLine();
        QRegExp rx("(.*\\S+)\\s+(\\d+)");
//        QRegExp rx("(.*\\S+[Стол|Заказ])\\s+[№|#]\\s+(\\d+)");
//        QRegExp rx("(.*\\S+[Время|доставки])\\s+(\\d+:\\d+)");

        while(!line.isNull()) {
            line = in.readLine();
            int pos = rx.indexIn(line);
            QStringList list;
            list = rx.capturedTexts();
            if(!list.isEmpty()){
                QString cap1 = rx.cap(1);
                count = rx.cap(2).toInt();
                if(!cap1.isEmpty()){
                    itemcode = -1;
                    getitemcode(cap1);
                    if(itemcode == -1){
                        qDebug() << "Incorrect code";
                    } else {
                    insertIn(idRest,itemcode,count);
                    list.clear();
//                    line.clear();
                    }
                }

            }

        }

        if(file.atEnd()){
            file.close();
            file.remove();
            getlastitem();
            getfile();
        }

//            timer->stop();
//            qDebug() << "Timer stop.EOF File";
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

//void parsercore::insertdb(int idRest, QString namefood, int count){

//    QString name = namefood;
//    int itemcount = count;

//    QSqlDatabase::database();
//    QSqlQuery *insertdata = new QSqlQuery;
//    insertdata->prepare("INSERT INTO orders_details2 (id,name,count) "
//                        "VALUES(nextval('orders_details2_id_seq'::regclass), "
//                        ":name, :itemcount)");
//    insertdata->bindValue(":idRest",'1');
//    insertdata->bindValue(":name",name);
//    insertdata->bindValue(":itemcount",itemcount);
//    insertdata->exec();
//    if(insertdata->lastError().isValid()){
//        qDebug() << insertdata->lastError();
//        qDebug() << insertdata->executedQuery();
//    }
//    insertdata->clear();
//}

void parsercore::getitemcode(QString namefood){
    QString name = namefood;
    qDebug() << namefood;
    QSqlDatabase::database();
    QSqlQuery *getcode = new QSqlQuery;
    getcode->prepare("SELECT code FROM menu "
                     "WHERE name=:name ");
    getcode->bindValue(":name",name);
    getcode->exec();
    if(getcode->lastError().isValid()){
        qDebug() << getcode->executedQuery();
        qDebug() << getcode->lastError();
    }
//    qDebug() << getcode->executedQuery();
    while(getcode->next()){
         itemcode = getcode->value(0).toInt();
    }
    qDebug() << "Code:" << itemcode;
}

void parsercore::insertIn(int idRest, int code, int count){

    int itemcode = code;
    int itemcount = count;

//    qDebug() << "Insert";
//    qDebug() << idRest;
//    qDebug() << code;
//    qDebug() << count;
    QSqlDatabase::database();
    QSqlQuery *insertdata = new QSqlQuery;
    insertdata->prepare("INSERT INTO orders_details (id,restid,itemcode,itemcount,insertdate,deleted) "
                        "VALUES(nextval('orders_details_id_seq'::regclass), "
                        ":idRest, :itemcode, :itemcount, now(), false)");
    insertdata->bindValue(":idRest",idRest);
    insertdata->bindValue(":itemcode",itemcode);
    insertdata->bindValue(":itemcount",itemcount);
    insertdata->exec();
    if(insertdata->lastError().isValid()){
        qDebug() << insertdata->lastError();
        qDebug() << insertdata->executedQuery();
    }
    insertdata->clear();
}

void parsercore::getlastitem(){
    QSqlDatabase::database();
    QSqlQueryModel *getlastdata = new QSqlQueryModel;
    getlastdata->setQuery("SELECT mm.name,mm.code,d.count FROM orders_details d "
                          "LEFT JOIN menu mm on mm.code=d.itemcode "
                          "GROUP BY mm.name, mm.code, d.id "
                          "ORDER BY d.id DESC LIMIT 15");
    getlastdata->setHeaderData(0,Qt::Horizontal,QObject::trUtf8("Code"));
    getlastdata->setHeaderData(1,Qt::Horizontal,QObject::trUtf8("Name"));
    getlastdata->setHeaderData(2,Qt::Horizontal,QObject::trUtf8("Count"));
    ui->parserView->setModel(getlastdata);
    ui->parserView->verticalHeader()->hide();
    qDebug() << "Update";
}

void parsercore::getfile(){
    QDir dirParser(tmpPath);
    QFile fileParser(dirParser.filePath("_COM1.LOG"));

    QDir dirTmp(filePath);
    QFile fileTmp(dirTmp.filePath("_COM1.LOG"));

    fileTmp.copy(dirParser.filePath("_COM1.LOG"));
    qDebug() << "Copy File";
}

parsercore::~parsercore()
{
    delete ui;
}
