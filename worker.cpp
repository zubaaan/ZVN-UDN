#include "Worker.h"

Worker::Worker(QObject *parent) : QObject(parent)
{

}

void Worker::runWork(initialData initData)
{
        double
            y_T =        initData.y_T,
            y_M =        initData.y_M,
            z_M =        initData.z_M,
            x_C1 =       initData.x_C1,
            x_C2 =       initData.x_C2,
            dt =         initData.dt,
            T_ob =       initData.T_ob,
            D_OBN =      initData.D_OBN,
            T_D =        initData.T_D,
            V_G =        initData.V_G,
            V_C =        initData.V_C,
            sigma_coord =initData.sigma_coord,
            sigma_speed =initData.sigma_speed,
            sigma_pilot =initData.sigma_pilot,
            phi_M =      initData.phi_M,
            N_k =        initData.N_k,
            N_udn = 0,
            N = 0;

        std::mt19937 randomGenerator(time(0));
        /* инициализация генератора псевдослучайных чисел текущим
         * системным временем */

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
    //    stopAnim();

        qDebug() << "Количество успешных: " << N_udn;
        qDebug() << "Всего реализаций: " << N_k;

        double W_DN = N_udn / N_k;

    //    QString out = QString("W_DN = %1").arg(W_DN);
    //    ui->textBrowser->setText(out);
        ui->textBrowser->append(QTime::currentTime().toString("hh:mm:ss"));
        QString out = QString("y_T = %1; y_M = %2; z_M = %3; x_C1 = %4; x_C2 = %5; V_G = %6; V_C = %7; "
                              "N_k = %8; dt = %9; T_ob = %10; phi_M = %11; D_OBN = %12; T_D = %13; "
                              "sigma_coord = %14; sigma_speed = %15; sigma_pilot = %16.\nW_DN = %17\n").arg(y_T)
                .arg(y_M).arg(z_M).arg(x_C1).arg(x_C2).arg(V_G).arg(V_C).arg(N_k).arg(dt).arg(T_ob).arg(phi_M)
                .arg(D_OBN).arg(T_D).arg(sigma_coord).arg(sigma_speed).arg(sigma_pilot).arg(W_DN);
        ui->textBrowser->append(out);

}

QVector<QPolygonF> guidanceTriangle(QPointF target_coords, QPointF interc_coords,
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

QPolygonF setVect(QPointF startPoint, QPointF endPoint) // генерация вектора (зад-го 2-мя точками)
{
    QPolygonF res;

    res.push_back(startPoint);
    res.push_back(endPoint);

    return res;
}

QPointF toBasis(QPolygonF vect) // приведение vect к базису
{
    QPointF res(vect.at(1).x() - vect.at(0).x(), vect.at(1).y() - vect.at(0).y());
    return res;
}

qreal scalarMultVect(QPointF vect1, QPointF vect2) // скалярное произведение (vect1, vect2)
{
    return vect1.x()*vect2.x() + vect1.y()*vect2.y();
}

qreal vectAbs(QPolygonF vect) // модуль vect (зад-го 2-мя точками)
{
    return sqrtf(qPow( toBasis(vect).x(), 2) +
                 qPow( toBasis(vect).y(), 2) );
}

qreal vectAbs(QPointF vect)
{
    return sqrtf(qPow( vect.x(), 2) +
                 qPow( vect.y(), 2) );
}

qreal angleBetweenVectors(QPointF a, QPointF b) // расчёт угла между двумя векторами
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

