#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "animation.h"
#include <math.h>
#include <QDebug>
#include <QTime>
#include <ctime>
#include <qmath.h>
#include <QThread>

const double toGrad = 180 / M_PI;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButt_start_clicked()
{
//    ui->progressBar->setValue(0);
    double
            y_T = ui->lineEdit_y_T->text().toDouble(),
            y_M = ui->lineEdit_y_M->text().toDouble(),
            z_M = ui->lineEdit_z_M->text().toDouble(),
            x_C1 = ui->lineEdit_x_C1->text().toDouble(),
            x_C2 = ui->lineEdit_x_C2->text().toDouble(),
            dt = ui->lineEdit_deltaT->text().toDouble(),
            T_ob = ui->lineEdit_T_ob->text().toDouble(),
            D_OBN = ui->lineEdit_D_OBN->text().toDouble(),
            T_D = ui->lineEdit_T_D->text().toDouble(),
            V_G = ui->lineEdit_V_G->text().toDouble(),
            V_C = ui->lineEdit_V_C->text().toDouble(),
            sigma_coord = ui->lineEdit_sigma_coord->text().toDouble(),
            sigma_speed = ui->lineEdit_sigma_speed->text().toDouble(),
            sigma_pilot = ui->lineEdit_sigma_pilot->text().toDouble(),
            phi_M = ui->lineEdit_phi_M->text().toDouble(),
            N_udn = 0;

    N = 0;
    N_k = ui->lineEdit_N_k->text().toDouble();
    std::mt19937 randomGenerator(time(0));
    /* инициализация генератора псевдослучайных чисел текущим
     * системным временем */

//    initThread();
    while (N < N_k)
    {
        N++;
//        qDebug() << "\n*** НОВАЯ РЕАЛИЗАЦИЯ ***\nN = " << N;

        std::uniform_real_distribution<float>
                y(y_T, y_M), z(-z_M, z_M), x_C(x_C2, x_C1);

        double y_C = y(randomGenerator);

        QPointF coordTgt(x_C(randomGenerator), z(randomGenerator));
        QPointF coordInt(0, 0);
        QPointF speedTgt(-V_C, 0);

        QVector<QPolygonF> spdVect;
        spdVect = guidanceTriangle(coordTgt, coordInt, speedTgt, V_G);

        double t = 0, U1 = 0, U;
        bool Dflag = true, Bflag = false;

        while (!Bflag)
        {
            t += dt;

            double d = t / T_ob; double x; std::modf(d, &x);
            U = 1 + x;

            if(U != U1)
            {
                std::normal_distribution<double>
                        xI(coordInt.x(), coordInt.x()/100*sigma_coord), zI(coordInt.y(), coordInt.y()/100*sigma_coord), vI(V_G, V_G/100*sigma_speed),
                        xC(coordTgt.x(), coordTgt.x()/100*sigma_coord), zC(coordTgt.y(), coordTgt.y()/100*sigma_coord),
                        vxC(speedTgt.x(), speedTgt.x()/100*sigma_speed), vzC(speedTgt.y(), speedTgt.y()/100*sigma_speed);

                QPointF coordInt_(xI(randomGenerator), zI(randomGenerator)),
                        coordTgt_(xC(randomGenerator), zC(randomGenerator)),
                        speedTgt_(vxC(randomGenerator), vzC(randomGenerator));

//                qDebug() << "\nt = " << t;
//                qDebug() << "Координаты цели: " << coordTgt;
//                qDebug() << "Координаты цели с ошибкой: " << coordTgt_;
//                qDebug() << "Координаты перехватчика: " << coordInt;
//                qDebug() << "Координаты перехватчика с ошибкой: " << coordInt_;

                spdVect = guidanceTriangle(coordTgt_, coordInt_, speedTgt_, vI(randomGenerator));

                U1 = U;
//                qDebug() << "U1 = " << U1;
            }

            QPointF IntrcSpdVect = spdVect.at(1).at(0);
            QPolygonF rangeVect;

            std::normal_distribution<double>
                    vxI(IntrcSpdVect.x(), IntrcSpdVect.x()/100*sigma_pilot),
                    vzI(IntrcSpdVect.y(), IntrcSpdVect.y()/100*sigma_pilot);

            coordInt.setX(vxI(randomGenerator)*dt + coordInt.x());
            coordInt.setY(vzI(randomGenerator)*dt + coordInt.y());

            coordTgt.setX(speedTgt.x()*dt + coordTgt.x());
            coordTgt.setY(speedTgt.y()*dt + coordTgt.y());

            rangeVect = setVect(coordInt, coordTgt);

            if(D_OBN > vectAbs(rangeVect))
            {
                if(Dflag)
                {
//                    qDebug() << "\nD_OBN < D";
//                    qDebug() << "angle between speedVect and rangeVect is: "
//                             << angleBetweenVectors(IntrcSpdVect, toBasis(rangeVect))*toGrad;
                    Dflag = false;
                }
                if(angleBetweenVectors(IntrcSpdVect, toBasis(rangeVect))*toGrad < phi_M)
                {
//                    qDebug() << "phi < phi_M";
                    N_udn++;
                    Bflag = true;
                } else if(t > T_D)
                {
//                    qDebug() << "Координаты цели: " << coordTgt;
//                    qDebug() << "Координаты перехватчика: " << coordInt;
//                    qDebug() << "t > T_D";
                    Bflag = true;
                }
            } else if(t > T_D)
            {
//                qDebug() << "Координаты цели: " << coordTgt;
//                qDebug() << "Координаты перехватчика: " << coordInt;
//                qDebug() << "t > T_D";
                Bflag = true;
            }
        }
    }
    stopAnim();

    qDebug() << "Количество успешных: " << N_udn;
    qDebug() << "Всего реализаций: " << N_k;

    double W_DN = N_udn / N_k;



    QString out = QString("W_DN = %1").arg(W_DN);
    ui->textBrowser->setText(out);
}

void MainWindow::anim_()
{
//    qDebug() << N*100/N_k;
//    ui->progressBar->setValue(N*100/N_k);
//    QMetaObject::invokeMethod(ui->progressBar,         // obj
//                                     "setValue",         // member: don't put parameters
//                                     Qt::QueuedConnection,     // connection type
//                                     Q_ARG(int, static_cast<int>(N*100/N_k)));     // val1
}

QVector<QPolygonF> MainWindow::guidanceTriangle(QPointF target_coords, QPointF interc_coords,
                                  QPointF target_speed, qreal interc_speed)
{
    QPolygonF rangeVect = setVect(interc_coords, target_coords);                // вектор дальности

    qreal absRangeVect = vectAbs(rangeVect);                                    // модуль вектора дальности

    QPointF unitVector(toBasis(rangeVect).x() / absRangeVect,                   // единичный вектор дальности
                       toBasis(rangeVect).y() / absRangeVect);

    qreal absTgtSpdVect_on_range = scalarMultVect(unitVector, target_speed);    // модуль проекции вектора скорости
                                                                                // цели на вектор дальности

    QPointF TgtSpdVect_on_range(unitVector.x() * absTgtSpdVect_on_range,        // проекция вектора скорости
                                unitVector.y() * absTgtSpdVect_on_range);       // цели на вектор дальности

    qreal absTgtSpdVect_norm_range;                                             // модуль нормали вектора скорости
                                                                                // цели к вектору дальности
    if( (qPow(target_speed.x(),2) +
         qPow(target_speed.y(),2) -
         qPow(absTgtSpdVect_on_range,2)) < 0)
        absTgtSpdVect_norm_range = 0.;
    else
        absTgtSpdVect_norm_range = qSqrt(qPow(target_speed.x(),2) +
                                         qPow(target_speed.y(),2) -
                                         qPow(absTgtSpdVect_on_range,2));

    qreal absIntrcVect_on_range = sqrtf(qPow(interc_speed, 2) -                 // модуль проекции вектора скорости
                                        qPow(absTgtSpdVect_norm_range,2));      // перехватчика  на вектор дальности

    QPointF IntrcVect_on_range(absIntrcVect_on_range*unitVector.x(),            // проекция вектора скорости
                               absIntrcVect_on_range*unitVector.y());           // перехватчика  на вектор дальности

    QPointF TgtSpdVect_norm_range(target_speed.x() - TgtSpdVect_on_range.x(),   // нормаль вектора скорости
                                  target_speed.y() - TgtSpdVect_on_range.y());  // цели к вектору дальности

    QPointF IntrcSpdVect(TgtSpdVect_norm_range.x() + IntrcVect_on_range.x(),    // вектор скорости
                         TgtSpdVect_norm_range.y() + IntrcVect_on_range.y());   // перехватчика

//    qDebug() << "К-ты цели " << target_coords;
//    qDebug() << "К-ты перехв. " << interc_coords;
//    qDebug() << "В-р дальности " << rangeVect;
//    qDebug() << "Модуль в-ра дальности " << absRangeVect;
//    qDebug() << "Единичный в-р " << unitVector;
//    qDebug() << "Модуль в-ра скорости цели\nна вектор дальности " << absTgtSpdVect_on_range;
//    qDebug() << "Проекция вектора скорости\nцели на вектор дальности " << TgtSpdVect_on_range;
//    qDebug() << "Модуль нормали вектора\nскорости цели " << absTgtSpdVect_norm_range;
//    qDebug() << "Модуль проекции вектора скорости\nперехватчика  на вектор дальности " << absIntrcVect_on_range;
//    qDebug() << "Проекция вектора скорости\nперехватчика  на вектор дальности " << IntrcVect_on_range;
//    qDebug() << "Нормаль вектора скорости\nцели к вектору дальности " << TgtSpdVect_norm_range;
//    qDebug() << "Вектор скорости перехватчика " << IntrcSpdVect << endl;


    QVector<QPolygonF> res;
    res.push_back(rangeVect);

    QPolygonF spd;
    spd.push_back(IntrcSpdVect);
    res.push_back(spd);
    return res; // [rangeVect, IntrcSpdVect]
}

QPolygonF MainWindow::setVect(QPointF startPoint, QPointF endPoint) // генерация вектора (зад-го 2-мя точками)
{
    QPolygonF res;

    res.push_back(startPoint);
    res.push_back(endPoint);

    return res;
}

QPointF MainWindow::toBasis(QPolygonF vect) // приведение vect к базису
{
    QPointF res(vect.at(1).x() - vect.at(0).x(), vect.at(1).y() - vect.at(0).y());
    return res;
}

qreal MainWindow::scalarMultVect(QPointF vect1, QPointF vect2) // скалярное произведение (vect1, vect2)
{
    return vect1.x()*vect2.x() + vect1.y()*vect2.y();
}

qreal MainWindow::vectAbs(QPolygonF vect) // модуль vect (зад-го 2-мя точками)
{
    return sqrtf(qPow( toBasis(vect).x(), 2) +
                 qPow( toBasis(vect).y(), 2) );
}

qreal MainWindow::vectAbs(QPointF vect)
{
    return sqrtf(qPow( vect.x(), 2) +
                 qPow( vect.y(), 2) );
}

qreal MainWindow::angleBetweenVectors(QPointF a, QPointF b) // расчёт угла между двумя векторами
{
    qreal res = (a.x()*b.x() + a.y()*b.y()) / (vectAbs(a)*vectAbs(b));
    auto temp = static_cast<int>(res);
    if(temp == 1)
        return 0;
    else if(temp == -1)
        return M_PI;
    else
        return qAcos(res);
}

void MainWindow::initThread()
{
    // Создание потока
    QThread* thread = new QThread;
    Animation* animation = new Animation();
    // Передаем права владения "рабочим" классом, классу QThread.
    animation->moveToThread(thread);

    // Связываем сигнал об ошибки со слотом обработки ошибок(не показан).
//                connect(worker, SIGNAL(error(QString)), this, SLOT(errorHandler(QString)));

    // Условие выхода
    connect(this,SIGNAL(stopAnim()),animation,SLOT(stopAnim()));
    // Анимация
    connect(animation, SIGNAL(next()),this,SLOT(anim_()), Qt::DirectConnection);


    // Соединяем сигнал started потока, со слотом process "рабочего" класса, т.е. начинается выполнение нужной работы.
    QObject::connect(thread, SIGNAL(started()), animation, SLOT(process()));

    // По завершению выходим из потока, и удаляем рабочий класс
    QObject::connect(animation, SIGNAL(finished()), thread, SLOT(quit()));
    QObject::connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));

    // Удаляем поток, после выполнения операции
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}
