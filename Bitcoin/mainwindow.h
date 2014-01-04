#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "network.h"
#include "miners.h"
#include "constans.h"
#include "calcwindow.h"
#include "help.h"
#include <QMainWindow>
#include <QTimer>
#include <QTableWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QTimer *saving;
    QTimer *table;
    QTimer *reload;
    QTimer *reloadExch;
    QTimer *reloadBlock;
    Network *url;
    Calculation *calc;

    int num;

    bool parserend;
    
private:

    Ui::MainWindow *ui;

    Miner *miner;
    CalcWindow *w;
    Help *hlp;

    QTableWidgetItem *item;

    static bool save;

    bool OpenHirstory(char *file);
    void GraphicsUpdate(ofNetwork *grph, Exchanger *exch);
    void UpdateCalculationData();

private slots:
    bool SaveHirstory();
    bool TableActiviing();
    void OpenHelp();
    void update();
    void on_tableResident_itemChanged(QTableWidgetItem *item);
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();
    void on_lineEdit_2_editingFinished();
    void on_lineEdit_editingFinished();
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    void on_radioButton_3_clicked();
    void on_radioButton_4_clicked();
    void on_radioButton_5_clicked();
    void on_pushButton_2_clicked();
    void on_comboBox_2_currentIndexChanged(int index);
    void on_comboBox_currentIndexChanged(int index);
    void on_comboBox_5_currentIndexChanged(int index);
    void on_comboBox_3_currentIndexChanged(int index);
    void on_tableResident_cellClicked(int row, int column);
};

#endif // MAINWINDOW_H
