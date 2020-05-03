#ifndef ANIMATION_H
#define ANIMATION_H

#include <QObject>

class Animation : public QObject
{
    Q_OBJECT
public:
    Animation();
    ~Animation();

public slots:
    void process();
    void stopAnim();

signals:
    void finished();
    void next();

private:
    bool stop;

};

#endif // ANIMATION_H
