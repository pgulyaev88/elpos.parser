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

static void process_line(const QString &)
{
}

void parsercore::parsefile(){
    //    getsettings();

        qDebug() << "Start Parsing";
        QFile file("com1.txt");
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            qDebug() << file.errorString();

        QTextStream in(&file);
        QString line = in.readLine();
        int x = 0;
        while(!line.isNull()) {
            process_line(line);
            line = in.readLine();
            x++;
//            qDebug() << line;
        }
        qDebug() << x;
    //    clearenv();
}


parsercore::~parsercore()
{
    delete ui;
}
