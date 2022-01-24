#ifndef Worker_H
#define Worker_H

#include <QObject>
#include "common.h"
#include <math.h>
#include <QDebug>
#include <QTime>
#include <ctime>
#include <qmath.h>
#include <QThread>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);

signals:

public slots:
    void runWork(initialData initData);

private:
    QVector<QPolygonF> guidanceTriangle(QPointF target_coords, QPointF interc_coords,
                                      QPointF target_speed, qreal interc_speed);

    QPolygonF setVect(QPointF startPoint, QPointF endPoint);
    QPointF toBasis(QPolygonF vect);
    qreal scalarMultVect(QPointF vect1, QPointF vect2);
    qreal vectAbs(QPolygonF vect);
    qreal vectAbs(QPointF vect);
    qreal angleBetweenVectors(QPointF a, QPointF b);
};

#endif // Worker_H
