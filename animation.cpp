#include "animation.h"
#include "QDebug"
#include "QTimer"
#include "QThread"
#include "QEventLoop"
#include "QApplication"

Animation::Animation(): QObject()
{
    stop = false;
}

Animation::~Animation()
{
    qDebug() << "ANIMATION DELETE";
}

void Animation::process()
{
    while(!stop)
    {
        qDebug() << "next";
        QThread::msleep(1);
        QApplication::processEvents();
        emit next();
    }
    emit finished();
}

void Animation::stopAnim()
{
    stop = true;
}
