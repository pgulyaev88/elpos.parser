#ifndef PARSERCORE_H
#define PARSERCORE_H

#include <QtGui>
#include <QTimerEvent>
#include <QObject>
#include <QTimer>
#include <QSettings>
#include <QFile>
#include <QEvent>
#include <QDebug>
#include <QWidget>

namespace Ui {
    class parsercore;
}

class parsercore : public QWidget
{
    Q_OBJECT

public:
    explicit parsercore(QWidget *parent = 0);
    ~parsercore();

public slots:
    void getsettings();
    void parsefile();
    void getitemcode(QString namefood);

private slots:
    void getlastitem();
    void startParser();
    void stopParser();
    void insertIn(int idRest, int code, int count);
//    void insertdb(int idRest, QString namefood, int count);
    void dbcon();
    void getfile();
    void hideToTray();

protected:
    void timerEvent(QTimerEvent *event);

private:
    Ui::parsercore *ui;
    QTimer *timer;
    int step;
    QString line;
    QStringList list;
    int idRest;
    int code;
    int count;
    QString namefood;
    int itemcode;
    QString filePath;
    QString restName;
    QString tmpPath;

};

#endif // PARSERCORE_H
