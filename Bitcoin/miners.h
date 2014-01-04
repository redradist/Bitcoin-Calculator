#ifndef MINER_H
#define MINER_H

#include <QObject>
#include <QFile>
#include <QTimer>
#include <QDate>
#include <QGlobal.h>
#include <QTextCodec>
#include <QTextStream>
#include <QDebug>
#include "network.h"
#include "constans.h"

struct FunctionsMiner {
    QString manufactur;
    QString model;
    double hashrate;
    double price;
    double power;
    QDate startdata;

private:
    double powerPerGHs;
    double powerPerDate;
    double incomePerDate;
    double profitPerDate;
};

class Miner: public QObject
{
    Q_OBJECT

public:
    Miner(char *file);

    static bool save;

    QFile *difficulty;
    QFile *speed;

    FunctionsMiner *miner;
    FunctionsMiner *reserv_miner;

    int numbers;

    QFile *archOFminers;
    QByteArray byte;

public slots:
    void MinersUpdate();
};

#endif // MINER_H
