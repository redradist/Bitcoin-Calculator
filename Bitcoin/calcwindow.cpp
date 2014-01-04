#include "calcwindow.h"
#include "ui_calcwindow.h"

//// Функция подсчета натурального  логарифма
double ln(double x)
{
    int n = 1;
    double Ln = 0;

    // Проверяем достигнута ли точность 97%
    do
    {
        Ln += pow((-1), n+1)*pow((x-1), n)/n;
        n++;
    } while(n < 700);

    return Ln;
}

/*
 *  Конструктор дочернего окна программы.
 */
CalcWindow::CalcWindow(FunctionsMiner *miner, ofNetwork *grph, Calculation *calc,
                       Exchanger *exch, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalcWindow)
{
    // Функция предварительной настройки окна вывода графиков
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    this->setWindowFlags(Qt::Dialog | Qt::WindowFullscreenButtonHint);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    exist = true;
    cminer = miner, cgrph = grph, ccalc = calc, cexch = exch;

    num_mass = 4;
    BTC = new double[num_mass];
    COTE = new double[num_mass];
    Timedata = new QDateTime[num_mass];
    // Создаем сцену для вывода графиков.
    sceneDifficulty = new QGraphicsScene(0,0,ui->graphicsView->width()-2,ui->graphicsView->height()-2,ui->graphicsView); //создаём сцены для графиков с чёрным фоном
    // Создаем количество горизонтальных текстовых объектов для добавления на график.
    horizontal = new QGraphicsTextItem[2];
    // Создаем количество вертикальных текстовых объектов для добавления на график.
    vertical = new QGraphicsTextItem[4];

    ui->label_10->setText(cminer->manufactur);
    ui->label_12->setText(cminer->model);
    ui->label_5->setText(QString::number(miner->price,'d', 1));

    if(ccalc->ExchCote == 0)
    {
        ui->label_3->setText("Cash, $");
        ui->label_8->setText("Clean Cash, $");
        ui->label->setText("PowerCost, $");
    }
    else if(ccalc->ExchCote == 1)
    {
        ui->label_3->setText("Cash, €");
        ui->label_8->setText("Clean Cash, €");
        ui->label->setText("PowerCost, $");
    }
    else
    {
        ui->label_3->setText("Cash, P");
        ui->label_8->setText("Clean Cash, P");
        ui->label->setText("PowerCost, $");
    }

    GraphicsCashMining(CalcBTC());
}

/*
 * Деструктор дочернего окна программы.
 */
CalcWindow::~CalcWindow()
{
    delete []BTC;
    delete []COTE;
    delete []Timedata;
    delete []horizontal;
    delete []vertical;
    delete sceneDifficulty;
    delete ui;
    int i = 0;
    while(i < 10000)
        i++;
    exist = false;
}

//// Функция Ламберта для подсчета времени окончания подсчета 2016 блоков
double WLambert(double z)
{
    if(0 < z && z <= 500)
        return 0.665*(1+0.0195*ln(z+1))*ln(z+1)+0.04;
    else if(z > 500)
        return ln(z-4) - (1-1/ln(z))*ln(ln(z));
    return 0;
}

int CalcWindow::CalcBTC()
{
    // Просто переменная.
    long long int i = 0;
    // Стандартное время расчета одного блока
    const int TimeCalcBlock = 600;
    // Время роста котировок за 4 недели
    const long int TimeCote = 3600 * 24 * 31;
    /// Константа хранения количества пересчетов
    /// по которым будет вестись усреднение в режиме Default
    const int OptimizeDif = 3;

    // Текущая дата системгых часов
    QDateTime CurrentDate = QDateTime::currentDateTime();
    // Последняя изветная дата истории
    QDateTime LastDate = QDateTime::fromTime_t(cgrph->timedata[0]);
    // Дата начала расчета
    QDateTime CalcStartDate = QDateTime(cminer->startdata,QTime(0,0));
    // Дата конца расчета
    QDateTime CalcLastDate = CalcStartDate.addDays(ccalc->Days);
    // Количество прогнозируемых дней
    int CalcDays;
    if(LastDate <= CalcStartDate)
        CalcDays = LastDate.daysTo(CalcLastDate);
    else
        CalcDays = CalcStartDate.daysTo(CalcLastDate);

    // Массив с предсказуемыми датами изменения сложности
    QDateTime nextTimedata[CalcDays];
    double nextDifficulty[CalcDays];

    // Буфер для данных времени
    QDateTime buffer;

    // Переменная записи даннызх в истории
    int k = 0;
    long long int lu = 0;
    // Переменная поиск первых доступных данных в истории
    int p = 0;
    // Если начало расчета в пределах истории, то поиск данных из истории
    if(CalcStartDate < LastDate)
    {
        // Поиск первых данных истории
        while(CalcStartDate < QDateTime::fromTime_t(cgrph->timedataAll[p++]));
        // Проверяем, если есть разница сложности у текущих и предыдущих данных
        if(cgrph->difficultyAll[p-1] != cgrph->difficultyAll[p])
        {
            // ПАраметны функции линии
            long int K, B;
            // Количество оставшихся блоков
            long int remainBlock;
            // Записываем в буфер найденные данные
            buffer = QDateTime::fromTime_t(cgrph->timedataAll[p]);
            // Находим параметры функции линии
            B = abs((double)(cgrph->numberOfBlocksAll[p] - cgrph->numberOfBlocksAll[p-1])/
                    (cgrph->timedataAll[p-1] - cgrph->timedataAll[p]));
            K = (cgrph->numberOfBlocksAll[p] - B)/cgrph->timedataAll[p];
            // Подсчет оставшихся блоков
            remainBlock = 2016 - cgrph->numberOfBlocksAll[p]%2016;

            // Поиск времени пересчета
            buffer.addSecs(K*(cgrph->numberOfBlocksAll[p]+remainBlock)+B);
            // Запись первого значения времени в массив
            nextTimedata[k] = CalcStartDate;
            lu = nextTimedata[k].toTime_t();

            // Запись первой сложности в массив
            if(CalcStartDate >= buffer)
                nextDifficulty[k] = cgrph->difficultyAll[p-1], p--;
            else
                nextDifficulty[k] = cgrph->difficultyAll[p];
        }
        else
        {
            // Запись первых данных в массив
            nextTimedata[k] = CalcStartDate;
            nextDifficulty[k] = cgrph->difficultyAll[p];
            lu = nextTimedata[k].toTime_t();
        }

        // Запись следующих данных истории
        while(p > 0 && nextTimedata[k] <= CalcLastDate)
        {
            while((nextDifficulty[k] == cgrph->difficultyAll[p]) && p > 0)
                p--;

            // Если ышли из цикла из-за нек совпадения сложностей
            if(nextDifficulty[k] != cgrph->difficultyAll[p] && p > 0)
            {
                k++;
                nextDifficulty[k] = cgrph->difficultyAll[p];
                nextTimedata[k] = QDateTime::fromTime_t(cgrph->timedataAll[p]);
                lu = nextTimedata[k].toTime_t();
            }
        }
    }

    // Количество блоков до пересчета
    int RemainBlocks = 2016 - cgrph->BlockofNetwork%2016;
    // Времмя до следующего пересчета
    long long int Tperc = 0;
    // Множитель наращивания скорости сети
    double Dec = 0;
    // Проверяем параметры наращивания сложности
    if(ccalc->DiffiIncs != 0)
        Dec = 1+(double)(ccalc->DiffiIncs-1)/100;
    else
    {
        // Подсчет наращивания сложности по умолчанию в сети
        int i = 0, m = 0;

        // Оптимизация наращивания скорости сети по 3 параметрам
        while(m < 3)
        {
            if(cgrph->difficultyAll[i] != cgrph->difficultyAll[i+1])
                Dec += (double)cgrph->difficultyAll[i]/cgrph->difficultyAll[i+1], m++;
            i++;
        }

        // Получаем значение пересчета по умолчанию
        Dec /= m;
    }

    // Если дата больше или равна последней дате истории
    if(CalcStartDate >= LastDate)
    {
        nextDifficulty[k] = cgrph->difficultyAll[p];
        nextTimedata[k] = QDateTime::fromTime_t(cgrph->timedata[p]);
        lu = nextTimedata[k].toTime_t();
    }

    // Время начала прогнозирования
    buffer.setTime_t(cgrph->timedataAll[p]);

    // Уменьшение времени пересчета
    double mensh = (double)RemainBlocks/2016;

    lu = nextTimedata[k].toTime_t();
    bool start = false;
    // Просчет оставшихся сложностей
    while(nextTimedata[k] <= CalcLastDate)
    {
        // Подсчет времени до следкющего пересчета
        if(Dec != 1)
            Tperc = abs(TimeCalcBlock*mensh*RemainBlocks*ln(Dec)/(Dec-1));
        else
            Tperc = TimeCalcBlock*mensh*RemainBlocks;

        k++;

        if(!start)
        {
            nextTimedata[k] = LastDate.addSecs(Tperc);
            start = true;
        }
        else
            nextTimedata[k] = nextTimedata[k-1].addSecs(Tperc);
        lu = nextTimedata[k].toTime_t();
        nextDifficulty[k] = nextDifficulty[k-1]*Dec;
        mensh = 1, RemainBlocks = 2016;
    }

    // Сохранение массива времени и сложности
    nextTimedata[k] = CalcLastDate;
    nextDifficulty[k] = nextDifficulty[k-1];
    lu = nextTimedata[k].toTime_t();

    // Сетчик начала работы на истории
    int s = 0;
    if(CalcStartDate > nextTimedata[0])
    {
        while(CalcStartDate > nextTimedata[s])
            s++;
        nextTimedata[s-1] = CalcStartDate;
    }
    else
        s++;

    // Здесь уже получены все данные для расчета биткоинов
    delete []BTC;
    delete []COTE;
    delete []Timedata;
    num_mass = k-s+2;
    BTC = new double[num_mass];
    COTE = new double[num_mass];
    Timedata = new QDateTime[num_mass];
    // Очистка массива для хранения намайненых биткоинов
    // и эквивалента в иностранной валюте
    int l = 0;
    while(l < k-s+1)
    {
        BTC[l] = 0;
        COTE[l] = 0;
        l++;
    }
    // Создаем переменную для хранения намайненых биткоино
    // и общего времени майнинга
    double AllBTC = 0;
    long long int AllTime = 0;
    // Сохранить разы роста котировок валюты
    double DCote = 1+(double)ccalc->Cote/100;
    // Сетчик массива полученых биткоинов
    l = 1;
    // Создаем переменную для хранения котировкок валют
    double ExchCote = 0;
//// Сохраняем следующие параметры обменника и роста курса
    switch(ccalc->Excher)
    {
        // Если выбран обменник Mt.Gox
        case MtGox:
            switch(ccalc->ExchCote)
            {
                // Котировка по USD
                case USD:
                    ExchCote = cexch->MtGoxUSDBTC;
                    break;
                // Котировка по EUR
                case EUR:
                    ExchCote = cexch->MtGoxEURBTC;
                    break;
                // Котировка по RUB
                case RUB:
                    ExchCote = cexch->MtGoxRUBBTC;
                    break;
            }
            break;
        // Если выбран обменник BTC-e
        case BTCe:
            switch(ccalc->ExchCote)
            {
                // Котировка по USD
                case USD:
                    ExchCote = cexch->BTCeUSDBTC;
                    break;
                // Котировка по EUR
                case EUR:
                    ExchCote = cexch->BTCeEURBTC;
                    break;
                // Котировка по RUB
                case RUB:
                    ExchCote = cexch->BTCeRUBBTC;
                    break;
            }
            break;
        // Если выбран обменник MetaBank
        case MetaBank:
            switch(ccalc->ExchCote)
            {
                // Котировка по USD
                case USD:
                    ExchCote = cexch->MetaBankUSDBTC;
                break;
            }
            break;

    }

    Power = 0;
    int first = s-1;
//// Получаем массивы для вывода на графики
    do
    {
        // Подсчитываем количество намайненых BTC
        BTC[l] = BTC[l-1] + (double)cgrph->BTCblock*(nextTimedata[s].toTime_t() - nextTimedata[s-1].toTime_t()) * cminer->hashrate * pow(10, 9)/
                (nextDifficulty[s-1] * pow(2, 32));
        // Подсчитываем прибыль в иностарной валюте с учетом возрастания котировок
        COTE[l] = BTC[l]*ExchCote*
                pow(DCote,((double)(nextTimedata[s].toTime_t() - nextTimedata[first].toTime_t())/TimeCote));
        AllBTC += (double)cgrph->BTCblock*(nextTimedata[s].toTime_t() - nextTimedata[s-1].toTime_t()) * cminer->hashrate * pow(10, 9)/
                (nextDifficulty[s-1] * pow(2, 32));
        AllTime += nextTimedata[s].toTime_t() - nextTimedata[s-1].toTime_t();
        Timedata[l-1] = nextTimedata[s-1];
        Power += cminer->power * (nextTimedata[s].toTime_t()-nextTimedata[s-1].toTime_t());

        l++, s++;
    } while(s <= k);

    Timedata[l-1] = nextTimedata[s-1];
    s = 0;

    ui->label_6->setText(QString::number(BTC[l-1], 'f', 2));
    ui->label_7->setText(QString::number(COTE[l-1], 'f', 2));

    // Возврат значение длины просчитанного массива
    return l;
}

void CalcWindow::GraphicsCashMining(int mass)
{
    // Уже есть все для построения графиков

    // Переменные хранения максимального значения сложности и скорости сети.
    double max_BTC = BTC[mass-1];
    double max_COTE = COTE[mass-1];

    // Отступ для построения графиков
    int STEP = 60;

    // Создаем переменные хранения максимальных размеров графиков.
    int GraphHeight = ui->graphicsView->height();
    int GraphWidth = ui->graphicsView->width();

    // Делаем отступы для построеня графиков.
    GraphHeight -= STEP;
    GraphWidth -= STEP;

    int i = 0;
    int pod = i;

//// Начало прорисовка графиков скорости и сложности сети.
    //Создаём разные фломастеры:
    QPen pen_dgray(Qt::gray,1,Qt::DotLine); //линии сетки серые пунктирные
    QPen pen_dgray1(Qt::gray,2*1,Qt::SolidLine); //линии сетки серые пунктирные
    QPen pen_red(Qt::red,2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin); //график красной линией
    QPen pen_yellow(Qt::yellow,2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin); //график жёлтой линией, и т.п.
    QPen pen_green(Qt::green,2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
    QPen pen_blue(Qt::cyan,2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);

    // Усанавливаем задний фон гарфика черным цветом.
    ui->graphicsView->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));

    // Определяем максимальные значения сложности и скорости сети для ввода графиков.
    i=0;

    ui->label_14->setText(QString::number(Power/(1000*3600),'f',1));
    ui->label_5->setText(QString::number((Power/(1000*3600))*ccalc->PriceofW,'f', 1));

    if(ccalc->ExchCote == 0)
        ui->label_9->setText(QString::number(max_COTE-cminer->price-(Power/
                                                       (1000*3600))*ccalc->PriceofW,'f',1));
    else if(ccalc->ExchCote == 1)
        ui->label_9->setText(QString::number(max_COTE-(cminer->price+(Power/
            (1000*3600))*ccalc->PriceofW)*cexch->BTCeEURBTC/cexch->BTCeUSDBTC,'f',1));
    else
        ui->label_9->setText(QString::number(max_COTE-(cminer->price+(Power/
            (1000*3600))*ccalc->PriceofW)*cexch->BTCeRUBBTC/cexch->BTCeUSDBTC,'f',1));

    // Добавление нижней завершающих линий графика
    sceneDifficulty->addLine(0,GraphHeight,GraphWidth,GraphHeight,pen_dgray1);
    sceneDifficulty->addLine(GraphWidth,0,GraphWidth,GraphHeight, pen_dgray1);

    // Добавление горзонтальных текстовых объектов на график.
    i=0;
    int m = 0;
    while (i <= GraphWidth){
        QDate data;
        if(m == 0)
            data = cminer->startdata.addDays(ccalc->Days);
        else
            data = cminer->startdata;

        (horizontal+m)->setPlainText(data.toString(Qt::DefaultLocaleShortDate));

        if(i < GraphWidth)
            (horizontal+m)->setPos(GraphWidth-15, GraphHeight+STEP);
        else
            (horizontal+m)->setPos(0, GraphHeight+STEP);

        (horizontal+m)->setRotation(-90.0);
        (horizontal+m)->setDefaultTextColor(QColor(Qt::magenta));
        sceneDifficulty->addItem(horizontal+m);
        i+= GraphWidth;
        m++;
    }

    // Вывод вертикальных текстовых объектов на график.
    m = 0;
    while(m < 2)
    {
        if(m%2 == 0)
        {
            (vertical+m)->setPlainText(QString::number(max_BTC,'f',2)+"\r\n"+QString("BTC"));
            (vertical+m)->setDefaultTextColor(QColor(Qt::green));
            (vertical+m)->setPos(GraphWidth, GraphHeight/4*abs(m/2)-5);
        }
        else
        {
            (vertical+m)->setPlainText(QString::number(max_COTE,'f',2)+"\r\n"+QString("USD"));
            (vertical+m)->setDefaultTextColor(QColor(Qt::red));
            (vertical+m)->setPos(GraphWidth, GraphHeight/4*abs(m/2)+20);
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

//////// Заполнение графиков линиями скорости и сложности сети.
//// Вывод графика в денежном эквиваленте
    // Инициализация переменных
    i=0;
    double now = 0;
    double next = 0;
    double width = 0;
    double width_next = 0;
    // Прорисовака непосредственно линий графика в иностранной валюте
    while(i < mass-1)
    {
        now = GraphHeight - (BTC[i]/(1.1 * max_BTC)) * GraphHeight;
        next = GraphHeight - (BTC[i+1]/(1.1 * max_BTC)) * GraphHeight;

        width = width_next;
        width_next = width + (double)GraphWidth*(Timedata[i+1].toTime_t()-Timedata[i].toTime_t())/
                (Timedata[mass-1].toTime_t()-Timedata[0].toTime_t());
        sceneDifficulty->addLine((int)width, (int)now,(int)width_next, (int)next, pen_green);
        i++;
    }

//// Вывод графика в денежном эквиваленте
    // Инициализация переменных
    i=0;
    now = 0;
    next = 0;
    width = 0;
    width_next = 0;
    // Прорисовака непосредственно линий графика в иностранной валюте
    while(i < mass-1)
    {
        now = GraphHeight - (COTE[i]/(1.2 * max_COTE)) * GraphHeight;
        next = GraphHeight - (COTE[i+1]/(1.2 * max_COTE)) * GraphHeight;

        width = width_next;
        width_next = width + (double)GraphWidth*(Timedata[i+1].toTime_t()-Timedata[i].toTime_t())/
                (Timedata[mass-1].toTime_t()-Timedata[0].toTime_t());
        sceneDifficulty->addLine((int)width, (int)now,(int)width_next, (int)next, pen_red);
        i++;
    }

    // Вывод сцены на график
    ui->graphicsView->setScene(sceneDifficulty);
}

/*
 * Обновление данных на графике
 */
void CalcWindow::UpdateGraphics(FunctionsMiner *miner, ofNetwork *grph, Calculation *calc,
                                Exchanger *exch)
{
    cminer = miner, cgrph = grph, ccalc = calc, cexch = exch;
    GraphicsCashMining(CalcBTC());
}
