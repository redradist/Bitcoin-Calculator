#ifndef MINERWINDOW_H
#define MINERWINDOW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <math.h>
#include "miners.h"
#include "constans.h"

double WLambert(double z);
double ln(double z);

enum Excher
{
    MtGox,
    BTCe,
    MetaBank
};

enum Cote
{
    USD,
    EUR,
    RUB
};

namespace Ui {
class CalcWindow;
}

class CalcWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CalcWindow(FunctionsMiner *miner, ofNetwork *grph, Calculation *calc,
                        Exchanger *exch, QWidget *parent = 0);
    ~CalcWindow();

    static bool exist;
    void UpdateGraphics(FunctionsMiner *miner, ofNetwork *grph, Calculation *calc,
                        Exchanger *exch);

private:
    Ui::CalcWindow *ui;

    double *BTC;
    double *COTE;
    double Power;
    FunctionsMiner *cminer;
    ofNetwork *cgrph;
    Calculation *ccalc;
    Exchanger *cexch;
    QDateTime *Timedata;
    int num_mass;
    QGraphicsScene *sceneDifficulty;
    QGraphicsTextItem *horizontal;
    QGraphicsTextItem *vertical;

    int CalcBTC();
    void GraphicsCashMining(int mass);
};

#endif // MINERWINDOW_H
