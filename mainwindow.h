#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "animation.h"
#include <math.h>
#include <QDebug>
#include <QTime>
#include <ctime>
#include <qmath.h>
#include <QThread>
#include "Worker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initThread();

signals:
    void startWorker(initialData initData);
    void stopAnim();

private slots:
    void on_pushButt_start_clicked();
    void anim_();

private:
    Ui::MainWindow *ui;
    double N = 0, N_k = 0;
    Worker *worker;
};

#endif // MAINWINDOW_H
