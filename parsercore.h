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
    void parsefile();

private slots:
    void getsettings();
    void startParser();
    void stopParser();


protected:
    void timerEvent(QTimerEvent *event);

private:
    Ui::parsercore *ui;
    QTimer *timer;
    int step;
    QString line;
    QStringList list;

};

#endif // PARSERCORE_H
