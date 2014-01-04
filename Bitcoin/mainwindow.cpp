#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <math.h>
#include <QHeaderView>
#include <QDesktopServices>

#define DAYINTERVAL 1
#define WIDGTHLINE 1
#define MINERPARAM 6

bool MainWindow::save = false;
// Пока не оздано окно вывода графиков
bool CalcWindow::exist = false;

bool Help::exist = false;

/*
 * Конструктор главного окна программы, где инициализируются основные графические элементы
 * и объекты необходимые для работы программы.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Функция предварительной настройки пользовательского интерфейса
    ui->setupUi(this);

    // Инициализация таймера для сохранения параметров калькуляции
    saving = new QTimer(this);
    connect(saving, SIGNAL(timeout()), this, SLOT(SaveHirstory()));
    saving->start(5000);

    // Инициализация таймера для сохранения параметров калькуляции
    table = new QTimer(this);
    connect(table, SIGNAL(timeout()), this, SLOT(TableActiviing()));
    table->start(500);

    calc = new Calculation();

    calc->Cote = ui->comboBox_2->currentIndex();
    calc->ExchCote = ui->comboBox->currentIndex();
    calc->Excher = ui->comboBox_5->currentIndex();
    calc->DiffiIncs = ui->comboBox_3->currentIndex();
    calc->PriceofW = ui->lineEdit_2->text().toDouble();
    calc->Days = ui->lineEdit->text().toInt();
    // Устанавливаем цену на электроэнергию
    calc->PriceofW = 0.15;
    ui->lineEdit_2->setText("0.15");
    // Усанавлдиваем число дней майнинга
    calc->Days = 30;
    ui->lineEdit->setText("30");

    OpenHirstory("Calc.conf");

    ui->radioButton->setChecked(true);

    // Считывание базы данных майнеров для анализа
    miner = new Miner("\Miners.data");

    // Создаем ссылки откуда считываем данные для построения графиков сложности и скорости сети
    url = new Network("http://blockexplorer.com/q/nethash",
                      "http://blockexplorer.com/q/bcperblock",
                      "http://blockexplorer.com/q/interval/8064",
                      "http://blockexplorer.com/q/getblockcount",
                      "http://btcsec.com/",
                      "http://btcsec.com/",
                      "https://metabank.ru/");

    // Инициализация таймера для обновления количества блоков в сети
    reloadBlock = new QTimer(this);
    connect(reloadBlock, SIGNAL(timeout()), url, SLOT(getReplyBlocksofNetwork()));
    connect(reloadBlock, SIGNAL(timeout()), url, SLOT(getReplyTimeblock()));
    reloadBlock->start(15000);

    // Инициализация таймера для обновления параметров котировок
    reloadExch = new QTimer(this);
    connect(reloadExch, SIGNAL(timeout()), url, SLOT(getReplyMtGox()));
    connect(reloadExch, SIGNAL(timeout()), url, SLOT(getReplyBTCe()));
    reloadExch->start(3000);

    // Создаем объекты с содержимым для таблицы списка майнеров
    item = new QTableWidgetItem[miner->numbers*MINERPARAM];
    ui->tableResident->setRowCount(miner->numbers);

    // Инициализация таймера для обновления графического интерфейса пользователя
    reload = new QTimer(this);
    connect(reload, SIGNAL(timeout()), this, SLOT(update()));
    connect(reload, SIGNAL(timeout()), miner, SLOT(MinersUpdate()));
    reload->start(5000);

    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(OpenHelp()));

    //// Заполняем содержимым таблицу майнеров исходными данными.
    for(int i = 0; i<miner->numbers; i++)
    {
        //item[].setFlags(item->flags() & ~Qt::ItemIsEditable);
        item[MINERPARAM*i] = QTableWidgetItem((miner->miner[i]).manufactur);
        //item[MINERPARAM*i].setTextColor(QColor(199, 199, 199, 199));
        item[MINERPARAM*i+1] = QTableWidgetItem((miner->miner[i]).model);
        //item[MINERPARAM*i+1].setTextColor(QColor(199, 199, 199, 240));
        item[MINERPARAM*i+2] = QTableWidgetItem(QString::number((miner->miner)[i].hashrate,'f',1));
        //item[MINERPARAM*i+2].setTextColor(QColor(199, 199, 199, 199));
        item[MINERPARAM*i+3] = QTableWidgetItem(QString::number((miner->miner)[i].price,'f',2));
        //item[MINERPARAM*i+3].setTextColor(QColor(199, 199, 199, 199));
        item[MINERPARAM*i+4] = QTableWidgetItem(QString::number((miner->miner[i]).power,'f',2));
        //item[MINERPARAM*i+4].setTextColor(QColor(199, 199, 199, 199));
        item[MINERPARAM*i+5] = QTableWidgetItem((miner->miner)[i].startdata.toString(Qt::SystemLocaleDate));
        //item[MINERPARAM*i+5].setTextColor(QColor(199, 199, 199, 199));

        ui->tableResident->setItem(i,0,&(item[MINERPARAM*i]));
        ui->tableResident->setItem(i,1,&(item[MINERPARAM*i+1]));
        // Задаем выранивание по центру следующих колонок
        item[MINERPARAM*i+2].setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableResident->setItem(i,2,&(item[MINERPARAM*i+2]));
        item[MINERPARAM*i+3].setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableResident->setItem(i,3,&(item[MINERPARAM*i+3]));
        item[MINERPARAM*i+4].setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableResident->setItem(i,4,&item[MINERPARAM*i+4]);
        item[MINERPARAM*i+5].setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableResident->setItem(i,5,&item[MINERPARAM*i+5]);
    }

    // Разрешение выделения одной строки
    ui->tableResident->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Делаем растягиаемыми первую и вторую колонки
    ui->tableResident->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Interactive);
    ui->tableResident->setColumnWidth(0, 100);
    ui->tableResident->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    // Все остальные колонки имеют размер по содержимому
    ui->tableResident->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    ui->tableResident->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeToContents);
    ui->tableResident->horizontalHeader()->setSectionResizeMode(4,QHeaderView::ResizeToContents);
    ui->tableResident->horizontalHeader()->setSectionResizeMode(5,QHeaderView::ResizeToContents);

    ui->tableResident->horizontalHeader()->setAutoScrollMargin(5);

    ui->pushButton_3->setDisabled(true);

    ui->radioButton->setDisabled(true);
    ui->radioButton_2->setDisabled(true);
    ui->radioButton_3->setDisabled(true);
    ui->radioButton_4->setDisabled(true);
    ui->radioButton_5->setDisabled(true);

    parserend = false;
}

/*
 * Деструктор главного окна программы.
 */
MainWindow::~MainWindow()
{
    //w->close();
    //delete w;
    // Удаление таймера для обновления графического интерфейса пользователя
    reload = new QTimer(this);
    // Удаление таймера для сохранения параметров калькуляции
    saving = new QTimer(this);
    // Удаление таймера для обновления количества блоков в сети
    reloadBlock = new QTimer(this);
    // Удаление таймера для обновления параметров котировок
    reloadExch = new QTimer(this);
    // Удаление объектов таблицы
    //delete item;
    // Удаление старого майнера
    delete []miner->reserv_miner;
    delete []miner->miner;
    delete miner;
    delete calc;
    delete url;
    //delete sceneDifficulty;
    //delete []textItem;
    //delete []vertical;
    // Удаление интерфейса юзера
    delete ui;
}


void MainWindow::update()
{
    static int first = 0;

    if(first == 0)
    {
        url->getReply1();
        url->getReplyBTCforBlock();
        url->getReplyTimeblock();
        url->getReplyBlocksofNetwork();
        url->getReplyMtGox();
        url->getReplyBTCe();
    }
    else if(first < 2)
    {
        url->getReplyMtGox();
        url->getReplyBTCe();
    }

    if(first <= 2)
        first++;
}

/*
 * Вывод графиков сложности и скорости сети Bitcoin,
 * а также параметров сети.
 */
void MainWindow::GraphicsUpdate(ofNetwork *grph, Exchanger *exch)
{
    // Просто переменная.
    long long int i = 0;

    // Переменные хранения максимального значения сложности и скорости сети.
    long long max_difficulty = 0;
    long long max_hashrate = 0;

    // Отступ для построения графиков.
    int STEP = 60;

    // Создаем переменные хранения максимальных размеров графиков.
    int GraphHeight = ui->graph_Difficulty->height();
    int GraphWidth = ui->graph_Difficulty->width();

    // Делаем отступы для построеня графиков.
    GraphHeight -= STEP;
    GraphWidth -= STEP;

    // Указатели на строки для вывода на графики.
    char *hash, *diff;

    //// Поиск нужной степени скорости.
    i = 0;
    while(pow(10, 3) <= (grph->hashrate[0])/pow(10,3*i))
        i++;

    //// Вывод нужного сокращения скорости сети.
    switch(i)
    {
        case HASH:
            ui->label_2->setText(QString("HashRate, H/s"));
            hash = "H/s";
            break;
        case KHASH:
            ui->label_2->setText(QString("HashRate, kH/s"));
            hash = "kH/s";
            break;
        case MHASH:
            ui->label_2->setText(QString("HashRate, MH/s"));
            hash = "MH/s";
            break;
        case GHASH:
            ui->label_2->setText(QString("HashRate, GH/s"));
            hash = "GH/s";
            break;
        case THASH:
            ui->label_2->setText(QString("HashRate, TH/s"));
            hash = "TH/s";
            break;
        case PHASH:
            ui->label_2->setText(QString("HashRate, PH/s"));
            hash = "PH/s";
            break;
        case EHASH:
            ui->label_2->setText(QString("HashRate, EH/s"));
            hash = "EH/s";
            break;
        case ZHASH:
            ui->label_2->setText(QString("HashRate, ZH/s"));
            hash = "ZH/s";
            break;
        case YHASH:
            ui->label_2->setText(QString("HashRate, YH/s"));
            hash = "YH/s";
            break;
    }

    //// Вывод значения скорости сети.
    ui->label_8->setText(QString::number(grph->hashrate[0]/pow(10,3*i), 'f', 3));
    int poh = i;

    //// Поиск нужной степени сложности.
    i = 0;
    while(pow(10, 3) <= (grph->difficulty[0])/pow(10,3*i))
        i++;

    //// Вывод нужного сокращения сложности сети.
    switch(i)
    {
        case HASH:
            ui->label->setText(QString("Difficulty"));
            ui->label_5->setText(QString("Difficulty Next"));
            diff = " ";
            break;
        case KHASH:
            ui->label->setText(QString("Difficulty, kilo"));
            ui->label_5->setText(QString("Difficulty Next, kilo"));
            diff = "kilo";
            break;
        case MHASH:
            ui->label->setText(QString("Difficulty, Mega"));
            ui->label_5->setText(QString("Difficulty Next, Mega"));
            diff = "Mega";
            break;
        case GHASH:
            ui->label->setText(QString("Difficulty, Giga"));
            ui->label_5->setText(QString("Difficulty Next, Giga"));
            diff = "Giga";
            break;
        case THASH:
            ui->label->setText(QString("Difficulty, Tera"));
            ui->label_5->setText(QString("Difficulty Next, Tera"));
            diff = "Tera";
            break;
        case PHASH:
            ui->label->setText(QString("Difficulty, Peta"));
            ui->label_5->setText(QString("Difficulty Next, Peta"));
            diff = "Peta";
            break;
        case EHASH:
            ui->label->setText(QString("Difficulty, Exa"));
            ui->label_5->setText(QString("Difficulty Next, Exa"));
            diff = "Exa";
            break;
        case ZHASH:
            ui->label->setText(QString("Difficulty, Zetta"));
            ui->label_5->setText(QString("Difficulty Next, Zetta"));
            diff = "Zetta";
            break;
        case YHASH:
            ui->label->setText(QString("Difficulty, Yotta"));
            ui->label_5->setText(QString("Difficulty Next, Yotta"));
            diff = "Yotta";
            break;
    }

    //// Вывод значения сложности сети.
    ui->label_9->setText(QString::number(grph->difficulty[0]/pow(10,3*i), 'f', 3));
    int pod = i;

    i = 0;
    ui->label_10->setText(QString::number(10*grph->difficulty[0]/(grph->Timeblock*pow(10,3*pod)), 'f', 3));
    ui->label_11->setText(QString::number(grph->BTCblock, 'f', 1));
    ui->label_12->setText(QString::number(grph->BlockofNetwork, 'f', 0));
    ui->label_13->setText(QString::number(grph->Timeblock, 'f', 2));
    ui->label_14->setText(QString::number(420000 - grph->BlockofNetwork, 'f', 0));
    ui->label_16->setText(QString::number(exch->MtGoxUSDBTC, 'f', 1));
    ui->label_18->setText(QString::number(exch->MtGoxEURBTC, 'f', 1));
    ui->label_20->setText(QString::number(exch->MtGoxRUBBTC, 'f', 1));
    ui->label_22->setText(QString::number(exch->BTCeUSDBTC, 'f', 1));
    ui->label_24->setText(QString::number(exch->BTCeEURBTC, 'f', 1));
    ui->label_26->setText(QString::number(exch->BTCeRUBBTC, 'f', 1));
    ui->label_28->setText(QString::number(exch->MetaBankUSDBTC, 'f', 1));
    ui->label_32->setText(QString::number(2016 - grph->BlockofNetwork%2016, 'f', 0));

//// Начало прорисовка графиков скорости и сложности сети.
    //Создаём разные фломастеры:
    QPen pen_dgray(Qt::gray,WIDGTHLINE,Qt::DotLine); //линии сетки серые пунктирные
    QPen pen_dgray1(Qt::gray,2*WIDGTHLINE,Qt::SolidLine); //линии сетки серые пунктирные
    QPen pen_red(Qt::red,2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin); //график красной линией
    QPen pen_yellow(Qt::yellow,2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin); //график жёлтой линией, и т.п.
    QPen pen_green(Qt::green,2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    QPen pen_blue(Qt::cyan,2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);

    // Создаем сцену для вывода графиков.
    //delete sceneDifficulty;
    QGraphicsScene *sceneDifficulty = new QGraphicsScene(0,0,ui->graph_Difficulty->width()-2,ui->graph_Difficulty->height()-2,ui->graph_Difficulty); //создаём сцены для графиков с чёрным фоном

    // Усанавливаем задний фон гарфика черным цветом
    ui->graph_Difficulty->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));

    if(url->error)
    {
        QGraphicsTextItem *textItem = new QGraphicsTextItem("Network Error");
        textItem->setFont(QFont("Courier", 31, QFont::Bold, true));
        textItem->setDefaultTextColor(QColor(Qt::lightGray));
        textItem->setPos(101, 111);
        sceneDifficulty->addItem(textItem);
        // Вывод сцены на график
        ui->graph_Difficulty->setScene(sceneDifficulty);
        return;
    }

    // Определяем максимальные значения сложности и скорости сети для ввода графиков.
    i=0;
    while(i < grph->historylength){
        if(grph->difficulty[i] > max_difficulty)
            max_difficulty = grph->difficulty[i];

        if(grph->hashrate[i] > max_hashrate)
            max_hashrate = grph->hashrate[i];

        i++;
    }

    // Постороение горизонтальных сеток.
    float l=0;
    while (l < 3*GraphHeight/4){
        sceneDifficulty->addLine(0,l,GraphWidth,l,pen_dgray);

        l+= GraphHeight/4;
    }

    // Добавление нижней завершающей линии графика.
    sceneDifficulty->addLine(0,GraphHeight,GraphWidth,GraphHeight,pen_dgray1);

    // Постороение вертикальных сеток.
    l=0;
    int m = 0;
    while (l<=GraphWidth){
        if(ui->radioButton->isChecked() || ui->radioButton_5->isChecked())
        {
            if(m <= 1)
            {
                if(l == GraphWidth)
                {
                    sceneDifficulty->addLine(l,0,l,GraphHeight, pen_dgray1);
                    break;
                }
                sceneDifficulty->addLine(l,0,l,GraphHeight, pen_dgray);
            }

            l+= (float) GraphWidth;
        }
        else if(ui->radioButton_2->isChecked())
        {
            if(m <= 3)
            {
                if(l == GraphWidth)
                {
                    sceneDifficulty->addLine(l,0,l,GraphHeight, pen_dgray1);
                    break;
                }
                sceneDifficulty->addLine(l,0,l,GraphHeight, pen_dgray);
            }

            l+= (float) GraphWidth/3;

            if(l < (GraphWidth+GraphWidth*0.1/3) && l > (GraphWidth-GraphWidth*0.1/3))
                l = GraphWidth;
        }
        else if(ui->radioButton_3->isChecked())
        {
            if(m <= 6)
            {
                if(l == GraphWidth)
                {
                    sceneDifficulty->addLine(l,0,l,GraphHeight, pen_dgray1);
                    break;
                }
                sceneDifficulty->addLine(l,0,l,GraphHeight, pen_dgray);
            }

            l+= (float) GraphWidth/6;

            if(l < (GraphWidth+GraphWidth*0.1/6) && l > (GraphWidth-GraphWidth*0.1/6))
                l = GraphWidth;
        }
        else if(ui->radioButton_4->isChecked())
        {
            if(m <= 12)
            {
                if(l == GraphWidth)
                {
                    sceneDifficulty->addLine(l,0,l,GraphHeight, pen_dgray1);
                    break;
                }
                sceneDifficulty->addLine(l,0,l,GraphHeight, pen_dgray);
            }

            l+= (float) GraphWidth/12;

            if(l < (GraphWidth+GraphWidth*0.1/12) && l > (GraphWidth-GraphWidth*0.1/12))
                l = GraphWidth;
        }
        m++;
    }
    m++;

    // Создаем количество горизонтальных текстовых объектов для добавления на график.
    //delete []textItem;
    QGraphicsTextItem *textItem = new QGraphicsTextItem[m];

    // Добавление горзонтальных текстовых объектов на график.
    i=0;
    m = 0;
    int p = 0;
    int currentday = QDateTime::fromTime_t(grph->timedata[p]).date().day();
    while (i<=GraphWidth){
        if(ui->radioButton->isChecked())
        {
            QDateTime data;
            data = QDateTime::fromTime_t(grph->timedata[p]);
            (textItem+m)->setPlainText(QDate(data.date().year(),data.date().month(),currentday).toString(Qt::DefaultLocaleShortDate));
            if(i < GraphWidth)
                (textItem+m)->setPos(GraphWidth-15, GraphHeight+STEP);
            else
                (textItem+m)->setPos(0, GraphHeight+STEP);
            (textItem+m)->setRotation(-90.0);
            (textItem+m)->setDefaultTextColor(QColor(Qt::magenta));
            sceneDifficulty->addItem(textItem+m);
            i+= GraphWidth;
            p += 31;
        }
        else if(ui->radioButton_2->isChecked())
        {
            QDateTime data;
            data = QDateTime::fromTime_t(grph->timedata[p]);
            data.addDays(currentday);
            (textItem+m)->setPlainText(QDate(data.date().year(),data.date().month(),currentday).toString(Qt::DefaultLocaleShortDate));
            if(i < GraphWidth*0.9)
                (textItem+m)->setPos(GraphWidth-15 - i, GraphHeight+STEP);
            else
                (textItem+m)->setPos(0, GraphHeight+STEP);
            (textItem+m)->setRotation(-90.0);
            (textItem+m)->setDefaultTextColor(QColor(Qt::magenta));
            sceneDifficulty->addItem(textItem+m);
            i+= GraphWidth/3;
            p += 31;
        }
        else if(ui->radioButton_3->isChecked())
        {
            QDateTime data;
            data = QDateTime::fromTime_t(grph->timedata[p]);
            data.addDays(currentday);
            (textItem+m)->setPlainText(QDate(data.date().year(),data.date().month(),currentday).toString(Qt::DefaultLocaleShortDate));
            if(i < GraphWidth*0.9)
                (textItem+m)->setPos(GraphWidth-15 - i, GraphHeight+STEP);
            else
                (textItem+m)->setPos(0, GraphHeight+STEP);
            (textItem+m)->setRotation(-90.0);
            (textItem+m)->setDefaultTextColor(QColor(Qt::magenta));
            sceneDifficulty->addItem(textItem+m);
            i+= GraphWidth/6;
            p += 31;
        }
        else if(ui->radioButton_4->isChecked())
        {
            QDateTime data;
            data = QDateTime::fromTime_t(grph->timedata[p]);
            data.addDays(currentday);
            (textItem+m)->setPlainText(QDate(data.date().year(),data.date().month(),currentday).toString(Qt::DefaultLocaleShortDate));
            if(i < GraphWidth*0.95)
                (textItem+m)->setPos(GraphWidth-15 - i, GraphHeight+STEP);
            else
                (textItem+m)->setPos(0, GraphHeight+STEP);
            (textItem+m)->setRotation(-90.0);
            (textItem+m)->setDefaultTextColor(QColor(Qt::magenta));
            sceneDifficulty->addItem(textItem+m);
            i+= GraphWidth/12;
            p += 31;
        }
        else if(ui->radioButton_5->isChecked())
        {
            QDateTime data;
            data = QDateTime::fromTime_t(grph->timedata[p]);
            if(i != GraphWidth)
                (textItem+m)->setPlainText(QDate(data.date().year(),data.date().month(),currentday).toString(Qt::DefaultLocaleShortDate));
            else
                (textItem+m)->setPlainText(QDate(data.date().year(),data.date().month(),data.date().day()).toString(Qt::DefaultLocaleShortDate));
            if(i < GraphWidth*0.95)
                (textItem+m)->setPos(GraphWidth-15 - i, GraphHeight+STEP);
            else
                (textItem+m)->setPos(0, GraphHeight+STEP);
            (textItem+m)->setRotation(-90.0);
            (textItem+m)->setDefaultTextColor(QColor(Qt::magenta));
            sceneDifficulty->addItem(textItem+m);
            i+= GraphWidth;
            p += grph->historylength-1;
        }
        m++;
    }

    // Создаем количество вертикальных текстовых объектов для добавления на график.
    //delete []vertical;
    QGraphicsTextItem *vertical = new QGraphicsTextItem[10];

    // Вывод вертикальных текстовых объектов на график.
    m = 0;
    while(m < 8)
    {
        if(m%2 == 0)
        {
            (vertical+m)->setPlainText(QString::number(1.1*max_hashrate*(4-abs(m/2))/(4*pow(10, 3*poh)),'f',2)+" "+QString(hash));
            (vertical+m)->setDefaultTextColor(QColor(Qt::green));
            (vertical+m)->setPos(GraphWidth, GraphHeight/4*abs(m/2)-5);
        }
        else
        {
            (vertical+m)->setPlainText(QString::number(1.1*max_difficulty*(4-abs(m/2))/(4*pow(10, 3*pod)),'f',2)+"\r\n"+QString(diff));
            (vertical+m)->setDefaultTextColor(QColor(Qt::red));
            (vertical+m)->setPos(GraphWidth, GraphHeight/4*abs(m/2)+10-5);
        }

        sceneDifficulty->addItem(vertical+m);
        m++;
    }

    (vertical+m)->setPlainText(QString::number(0));
    (vertical+m)->setDefaultTextColor(QColor(Qt::green));
    (vertical+m)->setPos(GraphWidth, GraphHeight-15);
    sceneDifficulty->addItem(vertical+(m++));
    (vertical+m)->setPlainText(QString::number(0));
    (vertical+m)->setDefaultTextColor(QColor(Qt::red));
    (vertical+m)->setPos(GraphWidth+8, GraphHeight-15);
    sceneDifficulty->addItem(vertical+m);

    //// Заполнение графиков линиями скорости и сложности сети.
    i=0;

    double elem = 0;
    int days = 0;

    if(ui->radioButton->isChecked())
        days = 31;
    else if(ui->radioButton_2->isChecked())
        days = 3 * 31;
    else if(ui->radioButton_3->isChecked())
        days = 6 * 31;
    else if(ui->radioButton_4->isChecked())
        days = 12 * 31;
    else
        days = grph->historylength;

    elem = days/DAYINTERVAL;

    int now = GraphHeight - (grph->hashrate[i]/(1.1 * max_hashrate)) * GraphHeight;
    int next = GraphHeight - (grph->hashrate[i+1]/(1.1 * max_hashrate)) * GraphHeight;

    double width = (double)GraphWidth;
    double width_next = width - (double)GraphWidth/elem;

    while(i < grph->historylength && i < (int)elem)
    {
        sceneDifficulty->addLine((int)width_next, next,(int)width, now, pen_green);
        i++;

        now = GraphHeight - (grph->hashrate[i]/(1.1 * max_hashrate)) * GraphHeight;
        next = GraphHeight - (grph->hashrate[i+1]/(1.1 * max_hashrate)) * GraphHeight;

        width = (double)width_next;
        width_next = width - (double)GraphWidth/elem;
    }

    i=0;
    elem = days/DAYINTERVAL;

    now = GraphHeight - (grph->difficulty[i]/(1.1 * max_difficulty)) * GraphHeight;
    next = GraphHeight - (grph->difficulty[i+1]/(1.1 * max_difficulty)) * GraphHeight;

    width = (double)GraphWidth;
    width_next = width - (double)GraphWidth/elem;

    while(i < grph->historylength && i < (int)elem)
    {
        sceneDifficulty->addLine((int)width_next, next,(int)width, now, pen_red);
        i++;

        now = GraphHeight - (grph->difficulty[i]/(1.1 * max_difficulty)) * GraphHeight;
        next = GraphHeight - (grph->difficulty[i+1]/(1.1 * max_difficulty)) * GraphHeight;

        width = (double)width_next;
        width_next = width - (double)GraphWidth/elem;
    }

    // Вывод сцены на график.
    ui->graph_Difficulty->setScene(sceneDifficulty);
}

//// Если произошли изменения в каком-то элементе таблицы таблице.
void MainWindow::on_tableResident_itemChanged(QTableWidgetItem *item)
{
    // Сохраняем номер строки элемента
    int row = item->row();

    // РАзрешаем сохранение данных
    Miner::save = true;

    // Ищем тип измененного значения по номеру столбца
    switch(item->column())
    {
        // Был изменен производитель
        case 0:
            if(item->text().length() > 20)
                item->setText((miner->reserv_miner)[row].manufactur);
            else
            {
                (miner->miner)[row].manufactur = item->text();
                (miner->reserv_miner)[row].manufactur = (miner->miner)[row].manufactur;
                // РАзрешаем сохранение данных
                Miner::save = true;
            }
            break;
        // Была изменена модель майнера
        case 1:
            if(item->text().length() > 30)
                item->setText((miner->reserv_miner)[row].model);
            else
            {
                (miner->miner)[row].model = item->text();
                (miner->reserv_miner)[row].model = (miner->miner)[row].model;
                // РАзрешаем сохранение данных
                Miner::save = true;
            }
            break;
        // Была отредактированна скорость майнера
        case 2:
            if(item->text().toDouble() == NULL)
            {
                item->setText(QString::number((miner->reserv_miner)[row].hashrate,'f',1));
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            }
            else
            {
                (miner->miner)[row].hashrate = item->text().toDouble();
                (miner->reserv_miner)[row].hashrate = (miner->miner)[row].hashrate;
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                // РАзрешаем сохранение данных
                Miner::save = true;
            }
            break;
        // Была отредактированна цена майнера
        case 3:
            if(item->text().toDouble() == NULL)
            {
                item->setText(QString::number((miner->reserv_miner)[row].price,'f',2));
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            }
            else
            {
                (miner->miner)[row].price = item->text().toDouble();
                (miner->reserv_miner)[row].price = (miner->miner)[row].price;
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                // РАзрешаем сохранение данных
                Miner::save = true;
            }
            break;
        // Была отредактированна потребляемая мощность майнером
        case 4:
            if(item->text().toDouble() == NULL)
            {
                item->setText(QString::number((miner->reserv_miner)[row].power,'f',2));
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            }
            else
            {
                (miner->miner)[row].power = item->text().toDouble();
                (miner->reserv_miner)[row].hashrate = (miner->miner)[row].power;
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                // РАзрешаем сохранение данных
                Miner::save = true;
            }
            break;
        // Время начала работы майнера
        case 5:
            if(QDate::fromString(item->text(),Qt::SystemLocaleDate).isNull() == 1)
                item->setText((miner->reserv_miner)[row].startdata.toString(Qt::SystemLocaleDate));
            else
            {
                if(QDate::fromString(item->text(),Qt::SystemLocaleDate)
                        > QDateTime::fromTime_t(url->bit->timedataAll[url->bit->historylengthAll-1]).date())
                {
                    (miner->miner)[row].startdata = QDate::fromString(item->text(),Qt::SystemLocaleDate);
                    (miner->reserv_miner)[row].startdata = (miner->miner)[row].startdata;
                    // РАзрешаем сохранение данных
                    Miner::save = true;
                }
                else
                    item->setText((miner->reserv_miner)[row].startdata.toString(Qt::SystemLocaleDate));
            }
            break;
    }
}

//// Вызов окна с выводом графика расчета прибыльности майнинга
void MainWindow::on_pushButton_3_clicked()
{
    // Проверка на существование окна ?
    if(CalcWindow::exist == false && ui->tableResident->currentRow() != -1)
    {
        ui->pushButton_3->setDisabled(true);
        // Создаем новый объект класса и выводим его на экран.
        w = new CalcWindow(miner->miner+ui->tableResident->currentRow(),url->bit, calc, url->exch);
        w->show();
    }
}

//// Нажали на кнопку добавить нового майнера
void MainWindow::on_pushButton_clicked()
{
    if(ui->tableResident->currentRow() == -1)
        return;
    int i;
    int row = ui->tableResident->currentRow();
    delete []item;
    miner->numbers++;

    // Создаем объекты с содержимым для таблицы списка майнеров
    item = new QTableWidgetItem[miner->numbers*MINERPARAM];
    ui->tableResident->setRowCount(miner->numbers);

//// Заполняем содержимым таблицу майнеров исходными данными.
    for(i = 0; i<(miner->numbers-1); i++)
    {
        item[MINERPARAM*i] = QTableWidgetItem((miner->miner[i]).manufactur);
        item[MINERPARAM*i+1] = QTableWidgetItem((miner->miner[i]).model);
        item[MINERPARAM*i+2] = QTableWidgetItem(QString::number((miner->miner)[i].hashrate,'f',1));
        item[MINERPARAM*i+3] = QTableWidgetItem(QString::number((miner->miner)[i].price,'f',2));
        item[MINERPARAM*i+4] = QTableWidgetItem(QString::number((miner->miner[i]).power,'f',2));
        item[MINERPARAM*i+5] = QTableWidgetItem((miner->miner)[i].startdata.toString(Qt::SystemLocaleDate));

        ui->tableResident->setItem(i,0,&(item[MINERPARAM*i]));
        ui->tableResident->setItem(i,1,&(item[MINERPARAM*i+1]));
        // Задаем выранивание по центру следующих колонок
        item[MINERPARAM*i+2].setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableResident->setItem(i,2,&(item[MINERPARAM*i+2]));
        item[MINERPARAM*i+3].setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableResident->setItem(i,3,&(item[MINERPARAM*i+3]));
        item[MINERPARAM*i+4].setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableResident->setItem(i,4,&item[MINERPARAM*i+4]);
        item[MINERPARAM*i+5].setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableResident->setItem(i,5,&item[MINERPARAM*i+5]);
    }

    // Удаление старых майнеров и создание указщателя на новые
    delete []miner->miner;
    miner->miner = new FunctionsMiner[miner->numbers];

    int k = 0;

    // Переписыываем не тронутую часть таблицы
    while(k < miner->numbers-1)
        miner->miner[k] = miner->reserv_miner[k], k++;
    miner->miner[k] = miner->reserv_miner[row];

    // Удаление резервной копии майнеров и создание новой резервной копии
    delete []miner->reserv_miner;
    miner->reserv_miner = new FunctionsMiner[miner->numbers];

    k = 0;
    while(k < miner->numbers)
        miner->reserv_miner[k] = miner->miner[k], k++;

//// Добавление характеристик нового майнера с характеристиками указанными в row
    item[MINERPARAM*i] = QTableWidgetItem((miner->miner[row]).manufactur);
    item[MINERPARAM*i+1] = QTableWidgetItem((miner->miner[row]).model);
    item[MINERPARAM*i+2] = QTableWidgetItem(QString::number((miner->miner)[row].hashrate,'f',1));
    item[MINERPARAM*i+3] = QTableWidgetItem(QString::number((miner->miner)[row].price,'f',2));
    item[MINERPARAM*i+4] = QTableWidgetItem(QString::number((miner->miner[row]).power,'f',2));
    item[MINERPARAM*i+5] = QTableWidgetItem((miner->miner)[row].startdata.toString(Qt::SystemLocaleDate));

//// Добавление в таблицу нового майнера с характеристиками указанными в row
    ui->tableResident->setItem(i,0,&(item[MINERPARAM*i]));
    ui->tableResident->setItem(i,1,&(item[MINERPARAM*i+1]));
    // Задаем выранивание по центру следующих колонок
    item[MINERPARAM*i+2].setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableResident->setItem(i,2,&(item[MINERPARAM*i+2]));
    item[MINERPARAM*i+3].setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableResident->setItem(i,3,&(item[MINERPARAM*i+3]));
    item[MINERPARAM*i+4].setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableResident->setItem(i,4,&item[MINERPARAM*i+4]);
    item[MINERPARAM*i+5].setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableResident->setItem(i,5,&item[MINERPARAM*i+5]);
    ui->tableResident->selectRow(miner->numbers-1);
    ui->tableResident->selectColumn(0);

//// Указаваем сохраняльщику сохранить файл с новым майнером
    Miner::save = true;
}

//// Нажали на кнопку удаления майнера
void MainWindow::on_pushButton_2_clicked()
{
    if(miner->numbers <= 1 || ui->tableResident->currentRow() == -1)
        return;

    int i;
    int row = ui->tableResident->currentRow();
    delete []item;
    miner->numbers--;

    // Создаем объекты с содержимым для таблицы списка майнеров
    item = new QTableWidgetItem[miner->numbers*MINERPARAM];
    ui->tableResident->setRowCount(miner->numbers);

//// Заполняем содержимым таблицу майнеров исходными данными
    for(i = 0; i<row; i++)
    {
        item[MINERPARAM*i] = QTableWidgetItem((miner->miner[i]).manufactur);
        item[MINERPARAM*i+1] = QTableWidgetItem((miner->miner[i]).model);
        item[MINERPARAM*i+2] = QTableWidgetItem(QString::number((miner->miner)[i].hashrate,'f',1));
        item[MINERPARAM*i+3] = QTableWidgetItem(QString::number((miner->miner)[i].price,'f',2));
        item[MINERPARAM*i+4] = QTableWidgetItem(QString::number((miner->miner[i]).power,'f',2));
        item[MINERPARAM*i+5] = QTableWidgetItem((miner->miner)[i].startdata.toString(Qt::SystemLocaleDate));

        ui->tableResident->setItem(i,0,&(item[MINERPARAM*i]));
        ui->tableResident->setItem(i,1,&(item[MINERPARAM*i+1]));
        // Задаем выранивание по центру следующих колонок
        item[MINERPARAM*i+2].setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableResident->setItem(i,2,&(item[MINERPARAM*i+2]));
        item[MINERPARAM*i+3].setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableResident->setItem(i,3,&(item[MINERPARAM*i+3]));
        item[MINERPARAM*i+4].setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableResident->setItem(i,4,&item[MINERPARAM*i+4]);
        item[MINERPARAM*i+5].setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableResident->setItem(i,5,&item[MINERPARAM*i+5]);
    }

//// Заполняем содержимым таблицу майнеров исходными данными
    while(i < miner->numbers)
    {
        miner->miner[i] = miner->reserv_miner[i+1];
        miner->reserv_miner[i] = miner->miner[i];
    //// Добавление характеристик нового майнера с характеристиками указанными в row
        item[MINERPARAM*i] = QTableWidgetItem((miner->miner[i]).manufactur);
        item[MINERPARAM*i+1] = QTableWidgetItem((miner->miner[i]).model);
        item[MINERPARAM*i+2] = QTableWidgetItem(QString::number((miner->miner)[i].hashrate,'f',3));
        item[MINERPARAM*i+3] = QTableWidgetItem(QString::number((miner->miner)[i].price,'f',2));
        item[MINERPARAM*i+4] = QTableWidgetItem(QString::number((miner->miner[i]).power,'f',2));
        item[MINERPARAM*i+5] = QTableWidgetItem((miner->miner)[i].startdata.toString(Qt::SystemLocaleDate));

    //// Добавление в таблицу нового майнера с характеристиками указанными в row
        ui->tableResident->setItem(i,0,&(item[MINERPARAM*i]));
        ui->tableResident->setItem(i,1,&(item[MINERPARAM*i+1]));
        // Задаем выранивание по центру следующих колонок
        item[MINERPARAM*i+2].setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableResident->setItem(i,2,&(item[MINERPARAM*i+2]));
        item[MINERPARAM*i+3].setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableResident->setItem(i,3,&(item[MINERPARAM*i+3]));
        item[MINERPARAM*i+4].setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableResident->setItem(i,4,&item[MINERPARAM*i+4]);
        item[MINERPARAM*i+5].setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableResident->setItem(i,5,&item[MINERPARAM*i+5]);
        i++;
    }

    // Выбор строки таблицы
    if(miner->numbers != 0)
    {
        if(row < miner->numbers-1)
            ui->tableResident->selectRow(row);
        else
            ui->tableResident->selectRow(miner->numbers-1);
    }

//// Указаваем сохраняльщику сохранить файл с новым майнером
    Miner::save = true;
}

//// Проверка коректности ввода данных в строку стоимости энергопотребления
void MainWindow::on_lineEdit_2_editingFinished()
{
    // Проверяем допустимое ли значение было введено
    if(ui->lineEdit_2->text().toDouble() == NULL)
    {
        ui->lineEdit_2->clear();
        calc->PriceofW = 0.15;
        ui->lineEdit_2->insert("0.15");
    }
    else
        calc->PriceofW = ui->lineEdit_2->text().toDouble();

    // Вызов функции обновления параметров калькуляции
    UpdateCalculationData();
}

//// Проверка коректности ввода данных в строку времени майнинга
void MainWindow::on_lineEdit_editingFinished()
{
    // Проверяем допустимое ли значение было введено
    if(ui->lineEdit->text().toInt() == NULL)
    {
        ui->lineEdit->clear();
        calc->Days = 30;
        ui->lineEdit->insert("30");
    }
    else
        calc->Days = ui->lineEdit_2->text().toInt();

    // Вызов функции обновления параметров калькуляции
    UpdateCalculationData();
}

//// Перерисовываем график скорости если сменилась кнопка
void MainWindow::on_radioButton_clicked()
{
    // Вызов функции перерисовки
    GraphicsUpdate(url->bit, url->exch);
}
//// Перерисовываем график скорости если сменилась кнопка
void MainWindow::on_radioButton_2_clicked()
{
    // Вызов функции перерисовки
    GraphicsUpdate(url->bit, url->exch);
}
//// Перерисовываем график скорости если сменилась кнопка
void MainWindow::on_radioButton_3_clicked()
{
    // Вызов функции перерисовки
    GraphicsUpdate(url->bit, url->exch);
}
//// Перерисовываем график скорости если сменилась кнопка
void MainWindow::on_radioButton_4_clicked()
{
    // Вызов функции перерисовки
    GraphicsUpdate(url->bit, url->exch);
}
//// Перерисовываем график скорости если сменилась кнопка
void MainWindow::on_radioButton_5_clicked()
{
    // Вызов функции перерисовки
    GraphicsUpdate(url->bit, url->exch);
}

//// Обновляем параметры калькуляции
void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    // Вызов функции обновления параметров калькуляции
    UpdateCalculationData();
}
//// Обновляем параметры калькуляции
void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    // Вызов функции обновления параметров калькуляции
    UpdateCalculationData();
}
//// Обновляем параметры калькуляции
void MainWindow::on_comboBox_5_currentIndexChanged(int index)
{
    // Вызов функции обновления параметров калькуляции
    UpdateCalculationData();
}
//// Обновляем параметры калькуляции
void MainWindow::on_comboBox_3_currentIndexChanged(int index)
{
    // Вызов функции обновления параметров калькуляции
    UpdateCalculationData();
}

//// Функция обновления параметров калькуляции
void MainWindow::UpdateCalculationData()
{
    calc->Cote = ui->comboBox_2->currentIndex();
    calc->ExchCote = ui->comboBox->currentIndex();
    calc->Excher = ui->comboBox_5->currentIndex();
    calc->DiffiIncs = ui->comboBox_3->currentIndex();
    calc->PriceofW = ui->lineEdit_2->text().toDouble();
    calc->Days = ui->lineEdit->text().toLong();
    save = true;
}

/*
 * Открыть файл с историей
 */
bool MainWindow::OpenHirstory(char *file)
{
    QFile *archOFminers = new QFile(file);

    // Проверяем, возможно ли открыть наш файл для чтения
    if (archOFminers->open(QFile::ReadWrite))
    {
        QByteArray byte = archOFminers->readAll(); //считываем все данные с файла в объект data
        int i = 0;
        bool exit = 0;
        int fist = 0, index = 0;
        QString *str = new QString(byte);
        int length = 0;

        // Инициализация переменных перед заполнением данных майнеров в структуру
        exit = 0;
        i = 0;
        fist = 0;

        // Поиск данных в файле
        index = str->indexOf(QString("&"), fist);
        // Сохраняем величину увеличения стоимости в валюте
        fist = index+1;
        index = str->indexOf(QString("="), fist);
        fist = index+2;
        length = str->indexOf(QString(","), fist);
        calc->Cote = str->mid(fist,length-fist).toInt();
        ui->comboBox_2->setCurrentIndex(calc->Cote);
        // Сохраняем валютную пару по которой будут проводиться расчеты
        fist = length + 3;
        index = str->indexOf(QString("="), fist);
        fist = index + 2;
        length = str->indexOf(QString(","), fist);
        if(str->mid(fist,length-fist) == "USD\/BTC")
            calc->ExchCote = 0;
        else if(str->mid(fist,length-fist) == "EUR\/BTC")
            calc->ExchCote = 1;
        else
            calc->ExchCote = 2;
        ui->comboBox->setCurrentIndex(calc->ExchCote);
        // Сохраняем обменник по которому будем проводить расчеты
        fist = length + 3;
        index = str->indexOf(QString("="), fist);
        fist = index + 2;
        length = str->indexOf(QString(","), fist);
        if(str->mid(fist,length-fist) == "Mt.Gox")
            calc->Excher = 0;
        else if(str->mid(fist,length-fist) == "BTC-e")
            calc->Excher = 1;
        else
            calc->Excher = 2;
        ui->comboBox_5->setCurrentIndex(calc->Excher);
        // Сохраняем величину увеличения сложности сети
        fist = length + 3;
        index = str->indexOf(QString("="), fist);
        fist = index + 2;
        length = str->indexOf(QString(","), fist);
        QString strr = str->mid(fist,length-fist);
        if(str->mid(fist,length-fist) == "Default")
            calc->DiffiIncs = 0;
        else
            calc->DiffiIncs = str->mid(fist,length-fist).toInt()+1;
        ui->comboBox_3->setCurrentIndex(calc->DiffiIncs);
        // Сохраняем цену одного ватта электроэнергии
        fist = length + 3;
        index = str->indexOf(QString("="), fist);
        fist = index + 2;
        length = str->indexOf(QString(","), fist);
        calc->PriceofW = str->mid(fist,length-fist).toDouble();

        ui->lineEdit_2->setText(QString::number(calc->PriceofW,'f', 2));
        // Сохраняем количество дней майнинга
        fist = length + 3;
        index = str->indexOf(QString("="), fist);
        fist = index + 2;
        length = str->indexOf(QString("\r\n"), fist);
        calc->Days = str->mid(fist,length-fist).toInt();
        ui->lineEdit->setText(QString::number(calc->Days));
    }
    else
    {
        qDebug() << "Невозможно открыть файл, возможно файл не существует.";
    }

    archOFminers->close();
    delete archOFminers;
}

/*
 * Сохранить файл с историей
 */
bool MainWindow::SaveHirstory()
{
    if(save == true)
    {
        QString str;

        QFile *archOFminers = new QFile("\Calc.conf");
        archOFminers->open(QIODevice::WriteOnly | QIODevice::Truncate);

        QTextStream out(archOFminers);

        str = "&Cote = " + QString::number(calc->Cote) + ",\r\n";
        if(calc->ExchCote == 0)
            str += "ExchCote = USD\/BTC,\r\n";
        else if(calc->ExchCote == 1)
            str += "ExchCote = EUR\/BTC,\r\n";
        else
            str += "ExchCote = RUB\/BTC,\r\n";
        if(calc->Excher == 0)
            str += "Exchanger = Mt.Gox,\r\n";
        else if(calc->Excher == 1)
            str += "Exchanger = BTC-e,\r\n";
        else
            str += "Exchanger = MetaBank,\r\n";
        if(calc->DiffiIncs == 0)
            str += "DiffInsc = Default,\r\n";
        else
            str += "DiffInsc = " + QString::number(calc->DiffiIncs-1) + ",\r\n";
        str += "PwCost = " + QString::number(calc->PriceofW,'f',2) + ",\r\n";
        str += "Days = " + QString::number(calc->Days) + "\r\n" + ":;";
        out << str;

        archOFminers->close();
        delete archOFminers;
        save = false;
    }
}

//// Если нажали на таблицу разблокируем кнопку калькуляции
void MainWindow::on_tableResident_cellClicked(int row, int column)
{
    TableActiviing();
}

//// Проверка на возможность разблокировки таблицы и вывода графика
bool MainWindow::TableActiviing()
{
    if(url->parsed && !(url->error) && url->openexch && ui->tableResident->currentRow() != -1 && CalcWindow::exist == false)
        ui->pushButton_3->setDisabled(false);
    if(url->parsed && !(url->error) && url->openexch)
    {
        static int first = 0;
        ui->radioButton->setDisabled(false);
        ui->radioButton_2->setDisabled(false);
        ui->radioButton_3->setDisabled(false);
        ui->radioButton_4->setDisabled(false);
        ui->radioButton_5->setDisabled(false);
        if(first < 2)
        {
            GraphicsUpdate(url->bit, url->exch);
            first++;
        }
    }
    else if(url->error)
    {
        static int first = 0;
        if(first < 2)
        {
            GraphicsUpdate(url->bit, url->exch);
            first++;
        }
    }

    // Обновляем данные котировок по соответствующим биржам
    ui->label_16->setText(QString::number(url->exch->MtGoxUSDBTC, 'f', 1));
    ui->label_18->setText(QString::number(url->exch->MtGoxEURBTC, 'f', 1));
    ui->label_20->setText(QString::number(url->exch->MtGoxRUBBTC, 'f', 1));
    ui->label_22->setText(QString::number(url->exch->BTCeUSDBTC, 'f', 1));
    ui->label_24->setText(QString::number(url->exch->BTCeEURBTC, 'f', 1));
    ui->label_26->setText(QString::number(url->exch->BTCeRUBBTC, 'f', 1));
    ui->label_28->setText(QString::number(url->exch->MetaBankUSDBTC, 'f', 1));
}

/*
 * Открываем меню справки о программе.
 */
void MainWindow::OpenHelp()
{
    if(!Help::exist)
    {
        hlp = new Help();
        hlp->show();
    }
}
