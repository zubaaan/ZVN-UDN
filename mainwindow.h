#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QVector<QPolygonF> guidanceTriangle(QPointF target_coords, QPointF interc_coords,
                                      QPointF target_speed, qreal interc_speed);

    QPolygonF setVect(QPointF startPoint, QPointF endPoint);
    QPointF toBasis(QPolygonF vect);
    qreal scalarMultVect(QPointF vect1, QPointF vect2);
    qreal vectAbs(QPolygonF vect);
    qreal vectAbs(QPointF vect);
    qreal angleBetweenVectors(QPointF a, QPointF b);
    void initThread();

signals:
    void stopAnim();

private slots:
    void on_pushButt_start_clicked();
    void anim_();

private:
    Ui::MainWindow *ui;
    double N = 0, N_k = 0;
};

#endif // MAINWINDOW_H
