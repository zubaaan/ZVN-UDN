#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <QDebug>
#include <QTime>
#include <ctime>

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
    double
            y_T = ui->lineEdit_y_T->text().toDouble(),
            y_M = ui->lineEdit_y_M->text().toDouble(),
            z_M = ui->lineEdit_z_M->text().toDouble(),
            x_C1 = ui->lineEdit_x_C1->text().toDouble(),
            x_C2 = ui->lineEdit_x_C2->text().toDouble(),
            deltaT = ui->lineEdit_deltaT->text().toDouble(),
            T_ob = ui->lineEdit_T_ob->text().toDouble(),
            D = ui->lineEdit_D->text().toDouble(),
            eps = ui->lineEdit_eps->text().toDouble(),
            q = ui->lineEdit_q->text().toDouble(),
            V_G = ui->lineEdit_V_G->text().toDouble(),
            V_C = ui->lineEdit_V_C->text().toDouble(),
            N_k = ui->lineEdit_N_k->text().toDouble(),
            N = 0,
            N_udn = 0;

    std::mt19937 randomGenerator(time(0));
    /* инициализация генератора псевдослучайных чисел текущим
     * системным временем */

    while (N < N_k)
    {
        N++;
        qDebug() << "N = " << N;

        std::uniform_real_distribution<float>
                y(y_T, y_M), z(-z_M, z_M), x_C(x_C2, x_C1);

        qDebug() << "y = " << y(randomGenerator);
        qDebug() << "z = " << z(randomGenerator);
        qDebug() << "x_C = " << x_C(randomGenerator);

        int t = 0, U1 = 0, U;
        bool flag = true;

        while (flag)
        {
            t += deltaT;
            double d = t / T_ob; double x; std::modf(d, &x);
            U = 1 + x;

            if(U = U1)
            {

            } else
            {
                std::normal_distribution<float>
                        D_(D, 1),
                        eps_(eps, 1),
                        q_(q, 1),
                        V_G_(V_G, 1),
                        V_C_(V_C, 1);
                D = D_(randomGenerator);
                eps = eps_(randomGenerator);
                q = q_(randomGenerator);
                V_G = V_G_(randomGenerator);
                V_C = V_C_(randomGenerator);                
            }




        }
    }

    double W_DN = N_udn / N_k;



    QString out = QString("W_DN = %1").arg(W_DN);
    ui->textBrowser->setText(out);
}
