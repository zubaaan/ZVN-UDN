#ifndef Worker_H
#define Worker_H

#include <QObject>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);

signals:

public slots:

private:
    void simulate();
};

#endif // Worker_H
