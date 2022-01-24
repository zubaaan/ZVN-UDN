#include "Worker.h"

Worker::Worker(QObject *parent) : QObject(parent)
{

}

void Worker::simulate()
{
////    ui->progressBar->setValue(0);
//    double
//            y_T = ui->lineEdit_y_T->text().toDouble(),
//            y_M = ui->lineEdit_y_M->text().toDouble(),
//            z_M = ui->lineEdit_z_M->text().toDouble(),
//            x_C1 = ui->lineEdit_x_C1->text().toDouble(),
//            x_C2 = ui->lineEdit_x_C2->text().toDouble(),
//            dt = ui->lineEdit_deltaT->text().toDouble(),
//            T_ob = ui->lineEdit_T_ob->text().toDouble(),
//            D_OBN = ui->lineEdit_D_OBN->text().toDouble(),
//            T_D = ui->lineEdit_T_D->text().toDouble(),
//            V_G = ui->lineEdit_V_G->text().toDouble(),
//            V_C = ui->lineEdit_V_C->text().toDouble(),
//            sigma_coord = ui->lineEdit_sigma_coord->text().toDouble(),
//            sigma_speed = ui->lineEdit_sigma_speed->text().toDouble(),
//            sigma_pilot = ui->lineEdit_sigma_pilot->text().toDouble(),
//            phi_M = ui->lineEdit_phi_M->text().toDouble(),
//            N_udn = 0;

//    N = 0;
//    N_k = ui->lineEdit_N_k->text().toDouble();
//    std::mt19937 randomGenerator(time(0));
//    /* инициализация генератора псевдослучайных чисел текущим
//     * системным временем */

////    initThread();
//    while (N < N_k)
//    {
//        N++;
////        qDebug() << "\n*** НОВАЯ РЕАЛИЗАЦИЯ ***\nN = " << N;

//        std::uniform_real_distribution<float>
//                y(y_T, y_M), z(-z_M, z_M), x_C(x_C2, x_C1);

//        double y_C = y(randomGenerator);

//        QPointF coordTgt(x_C(randomGenerator), z(randomGenerator));
//        QPointF coordInt(0, 0);
//        QPointF speedTgt(-V_C, 0);

//        QVector<QPolygonF> spdVect;
//        spdVect = guidanceTriangle(coordTgt, coordInt, speedTgt, V_G);

//        double t = 0, U1 = 0, U;
//        bool Dflag = true, Bflag = false;

//        while (!Bflag)
//        {
//            t += dt;

//            double d = t / T_ob; double x; std::modf(d, &x);
//            U = 1 + x;

//            if(U != U1)
//            {
//                std::normal_distribution<double>
//                        xI(coordInt.x(), coordInt.x()/100*sigma_coord), zI(coordInt.y(), coordInt.y()/100*sigma_coord), vI(V_G, V_G/100*sigma_speed),
//                        xC(coordTgt.x(), coordTgt.x()/100*sigma_coord), zC(coordTgt.y(), coordTgt.y()/100*sigma_coord),
//                        vxC(speedTgt.x(), speedTgt.x()/100*sigma_speed), vzC(speedTgt.y(), speedTgt.y()/100*sigma_speed);

//                QPointF coordInt_(xI(randomGenerator), zI(randomGenerator)),
//                        coordTgt_(xC(randomGenerator), zC(randomGenerator)),
//                        speedTgt_(vxC(randomGenerator), vzC(randomGenerator));

////                qDebug() << "\nt = " << t;
////                qDebug() << "Координаты цели: " << coordTgt;
////                qDebug() << "Координаты цели с ошибкой: " << coordTgt_;
////                qDebug() << "Координаты перехватчика: " << coordInt;
////                qDebug() << "Координаты перехватчика с ошибкой: " << coordInt_;

//                spdVect = guidanceTriangle(coordTgt_, coordInt_, speedTgt_, vI(randomGenerator));

//                U1 = U;
////                qDebug() << "U1 = " << U1;
//            }

//            QPointF IntrcSpdVect = spdVect.at(1).at(0);
//            QPolygonF rangeVect;

//            std::normal_distribution<double>
//                    vxI(IntrcSpdVect.x(), IntrcSpdVect.x()/100*sigma_pilot),
//                    vzI(IntrcSpdVect.y(), IntrcSpdVect.y()/100*sigma_pilot);

//            coordInt.setX(vxI(randomGenerator)*dt + coordInt.x());
//            coordInt.setY(vzI(randomGenerator)*dt + coordInt.y());

//            coordTgt.setX(speedTgt.x()*dt + coordTgt.x());
//            coordTgt.setY(speedTgt.y()*dt + coordTgt.y());

//            rangeVect = setVect(coordInt, coordTgt);

//            if(D_OBN > vectAbs(rangeVect))
//            {
//                if(Dflag)
//                {
////                    qDebug() << "\nD_OBN < D";
////                    qDebug() << "angle between speedVect and rangeVect is: "
////                             << angleBetweenVectors(IntrcSpdVect, toBasis(rangeVect))*toGrad;
//                    Dflag = false;
//                }
//                if(angleBetweenVectors(IntrcSpdVect, toBasis(rangeVect))*toGrad < phi_M)
//                {
////                    qDebug() << "phi < phi_M";
//                    N_udn++;
//                    Bflag = true;
//                } else if(t > T_D)
//                {
////                    qDebug() << "Координаты цели: " << coordTgt;
////                    qDebug() << "Координаты перехватчика: " << coordInt;
////                    qDebug() << "t > T_D";
//                    Bflag = true;
//                }
//            } else if(t > T_D)
//            {
////                qDebug() << "Координаты цели: " << coordTgt;
////                qDebug() << "Координаты перехватчика: " << coordInt;
////                qDebug() << "t > T_D";
//                Bflag = true;
//            }
//        }
//    }
////    stopAnim();

//    qDebug() << "Количество успешных: " << N_udn;
//    qDebug() << "Всего реализаций: " << N_k;

//    double W_DN = N_udn / N_k;

////    QString out = QString("W_DN = %1").arg(W_DN);
////    ui->textBrowser->setText(out);
//    ui->textBrowser->append(QTime::currentTime().toString("hh:mm:ss"));
//    QString out = QString("y_T = %1; y_M = %2; z_M = %3; x_C1 = %4; x_C2 = %5; V_G = %6; V_C = %7; "
//                          "N_k = %8; dt = %9; T_ob = %10; phi_M = %11; D_OBN = %12; T_D = %13; "
//                          "sigma_coord = %14; sigma_speed = %15; sigma_pilot = %16.\nW_DN = %17\n").arg(y_T)
//            .arg(y_M).arg(z_M).arg(x_C1).arg(x_C2).arg(V_G).arg(V_C).arg(N_k).arg(dt).arg(T_ob).arg(phi_M)
//            .arg(D_OBN).arg(T_D).arg(sigma_coord).arg(sigma_speed).arg(sigma_pilot).arg(W_DN);
//    ui->textBrowser->append(out);
}
