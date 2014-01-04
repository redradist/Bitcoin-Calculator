#include "network.h"

bool Network::save = false;

/*
 * Конструктор класса обращения к сети и распарсиванию данных
 */
Network::Network(QString urlString1, QString urlString2, QString urlString3,
                 QString urlString4, QString urlString5, QString urlString6,
                 QString urlString7){
    url1 = QUrl(urlString1);
    url2 = QUrl(urlString2);
    url3 = QUrl(urlString3);
    url4 = QUrl(urlString4);
    url5 = QUrl(urlString5);
    url6 = QUrl(urlString6);
    url7 = QUrl(urlString7);
    manager = new QNetworkAccessManager(this);
    manager->clearAccessCache();

    databcoin = new QString();
    bit = new ofNetwork();
    exch = new Exchanger();
    finished = false;

    bit->historylength = 0;
    bit->BTCblock = 0;
    bit->Timeblock = 0;
    bit->BlockofNetwork = 0;
    parsed = 0;
    error = 0;
    openexch = 0;

    ExchangerOpen("Exchanger.exh");

    saving = new QTimer(this);
    connect(saving, SIGNAL(timeout()), this, SLOT(ExchangerUpdate()));
    saving->start(5000);
}

void Network::getReply1() {
    QEventLoop loop;
    finished = false;
    reply1 = manager->get(QNetworkRequest(url1));
    connect(reply1, SIGNAL(finished()), this, SLOT(downloadFinished1()));
    connect(reply1, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
}

void Network::getReplyBTCforBlock() {
    QEventLoop loop;
    finished = false;
    reply2 = manager->get(QNetworkRequest(url2));
    connect(reply2, SIGNAL(finished()), this, SLOT(downloadFinishedBTCforBlock()));
    connect(reply2, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
}

void Network::getReplyTimeblock() {
    QEventLoop loop;
    finished = false;
    reply3 = manager->get(QNetworkRequest(url3));
    connect(reply3, SIGNAL(finished()), this, SLOT(downloadFinishedTimeblock()));
    connect(reply3, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
}

void Network::getReplyBlocksofNetwork() {
    QEventLoop loop;
    finished = false;
    reply4 = manager->get(QNetworkRequest(url4));
    connect(reply4, SIGNAL(finished()), this, SLOT(downloadFinishedBlocksofNetwork()));
    connect(reply4, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
}

void Network::getReplyMtGox() {
    QEventLoop loop;
    finished = false;
    reply5 = manager->get(QNetworkRequest(url5));
    connect(reply5, SIGNAL(finished()), this, SLOT(downloadFinishedMtGox()));
    connect(reply5, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
}

void Network::getReplyBTCe() {
    QEventLoop loop;
    finished = false;
    reply6 = manager->get(QNetworkRequest(url6));
    connect(reply6, SIGNAL(finished()), this, SLOT(downloadFinishedBTCe()));
    connect(reply6, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
}

void Network::downloadFinished1(){
    if(reply1->error() == QNetworkReply::NoError)
    {
        webContent1 = reply1->readAll();
        finished = true;
        parser1();
    }
    error = 0;
}

void Network::downloadFinishedBTCforBlock(){
    if(reply2->error() == QNetworkReply::NoError)
    {
        webContent2 = reply2->readAll();
        finished = true;
        parserBTCforBlock();
    }
}

void Network::downloadFinishedTimeblock(){
    if(reply3->error() == QNetworkReply::NoError)
    {
        webContent3 = reply3->readAll();
        finished = true;
        parserTimeblock();
    }
}

void Network::downloadFinishedBlocksofNetwork(){
    if(reply4->error() == QNetworkReply::NoError)
    {
        webContent4 = reply4->readAll();
        finished = true;
        parserBlocksofNetwork();
        parsed = true;
    }
}

void Network::downloadFinishedMtGox(){
    if(reply5->error() == QNetworkReply::NoError)
    {
        webContent5 = reply5->readAll();
        finished = true;
        parserMtGox();
        save = true;
    }
}

void Network::downloadFinishedBTCe(){
    if(reply6->error() == QNetworkReply::NoError)
    {
        webContent6 = reply6->readAll();
        finished = true;
        parserBTCe();
        save = true;
    }
}

/* Здесь парсим текущую сложность, скорость и день работы сети */
bool Network::parser1()
{
//// Если загрузка закончена, то
    if(finished)
    {
        //// переходим к распарсиванию полученых данных
        // Инициализация переменной хранения последнего символа
        long long int last = 0;
        // Инициализация переменной храненияпервого символа
        long long int fist = 0;
        // Объявление массива хранения распарсиваемых строк
        char str[100];
        // Объявление массива хранения скачного контента
        char *content;

        // Инициализация флага
        bool parserend = 0;
        // Инициализация счетчика
        unsigned char count = 0;

        // Передаем указатель на скачаный контент
        content = webContent1.data();

        // Поиск пследнего символа массива
        while(content[last] != '\0')
            last++;

        // Уменьшаем номер последнего символа массива на 1
        last--;
        fist = last;

        // Находимся в цикле пока не закончим парсер принятых данных
        while (!parserend)
        {
            // Ищем символ ',' в строке
            while(content[fist] != ',')
            {
                // Ищем символ '\n' в строке
                if(content[fist] == '\n')
                {
                    static bool first = false;
                    if(content[fist-1] == 'A' && content[fist-2] == 'T' && content[fist-3] == 'A')
                        parserend = true;

                    if(!first || parserend)
                    {
                        last = fist-1;
                        first = true;
                        if(parserend)
                            break;
                        else
                            count = 0;
                    }
                    else if(!parserend)
                        break;

                }

                fist--;
            }

            count++;

            // Определение размера строки
            int size;
            size = last-fist;

            // Инициализация переменной для
            // считывания строки посимвольно
            int i;
            i = 0;

            while(i < size)
                str[i] = content[fist+i+1], i++;

            str[i] = '\0';
            *databcoin = str;
            last = --fist;

            // Если еще не закончили парсить, раскладываем данные по полочкам
            if(!parserend || count == BLOCKS)
            {
                switch(count)
                {
                    case BLOCKS:
                        bit->numberOfBlocksAll[bit->historylengthAll] = databcoin->toLong();
                        count = 0;
                        last = fist;
                        fist--;
                        bit->historylengthAll++;
                        break;
                    case TIMEDATA:
                        bit->timedataAll[bit->historylengthAll] = databcoin->toDouble();
                        break;

                    case DIFFICULTY:
                        bit->difficultyAll[bit->historylengthAll] = databcoin->toDouble();
                        break;

                    case HASHRATE:
                        bit->hashrateAll[bit->historylengthAll] = databcoin->toLongLong();
                        break;
                }
            }
        }

        // Устанавливаем флаги завершения парсера
        finished = false;
        downloaded = false;

//// Поиск реальных дней и усреднение по ним скорости и сложности
        // Переменная хранения количества повторяющихся дней
        int tobe = 1;
        // Счетчик выборок из истории
        long int counter = 0;
        // Обнуляем историческую длинну дней
        bit->historylength = 0;
        // Инициализация переменной для хранения усредненного значения скорости сети
        double hashratemid = bit->hashrateAll[counter];
        // Инициализация переменной для хранения усредненного занчения сложности сети
        double difficultymid = bit->difficultyAll[counter];
        // Объявление переменной хранения последнего деня из выборки истории
        int lastday;
        // Инициализация объекта даны для получения текущего дня
        QDateTime data = QDateTime::fromTime_t(bit->timedata[counter]);
        // Инициализация последнего дня выборки
        lastday = data.date().day();
        // Увеличение счетчика выборки истории
        counter++;

        // Проверяем не выходим ли мы за пределы истории
        while(counter < bit->historylengthAll)
        {
            data = QDateTime::fromTime_t(bit->timedataAll[counter]);
            // Если последний день истории совпадает с текущей выборкой истории, то
            if(lastday == data.date().day())
            {
                // увеличиваем значения для последуюющего усредненния
                hashratemid += bit->hashrateAll[counter];
                difficultymid += bit->difficultyAll[counter];
                tobe++;
            }
            else
            {
                // Усредняем значения по количеству дней
                bit->hashrate[bit->historylength] = hashratemid/tobe;
                bit->difficulty[bit->historylength] = difficultymid/tobe;
                bit->timedata[bit->historylength] = bit->timedataAll[counter-1];
                bit->historylength++;

                // Инициализация значений для следующего усреднения
                lastday = data.date().day();
                hashratemid = bit->hashrateAll[counter];
                difficultymid = bit->difficultyAll[counter];
                tobe = 1;
            }

            // Увеличение счетчика выборки статистики
            counter++;
        }

        reply1->deleteLater();
        webContent1.clear();
        return true;
    }
    else if(downloaded)
        return true;
}

/* Здесь парсим текущее вознаграждение за блок */
bool Network::parserBTCforBlock(){
    QString find = QString(webContent2);
    bit->BTCblock = find.toDouble();
    reply2->deleteLater();
    webContent2.clear();
}

bool Network::parserTimeblock(){
    QString find = QString(webContent3);
    bit->Timeblock = find.toDouble()/60;
    reply3->deleteLater();
    webContent3.clear();
}

/* Здесь парсим текущее вознаграждение за блок */
bool Network::parserBlocksofNetwork(){
    QString find = QString(webContent4);
    bit->BlockofNetwork = find.toLong();
    reply4->deleteLater();
    webContent4.clear();
}

/* Здесь парсим котировки для различных курсов валют */
bool Network::parserMtGox(){
    int i = 0;
    long int fist = 0, last = 0;
    int length = 0;
    char str[30];
    QString findBTC = QString(webContent5);
    char *content = webContent5.data();

////// Поиск отношения курс биткоинов к USD
    QString buffer = "btc_curr_mgoxU";
    fist = findBTC.indexOf(QString(">"), findBTC.indexOf(buffer));
    buffer = ">";
    last = findBTC.indexOf(QString("<"), fist);
    fist++;

    i = 0;
    length = last - fist;
    while(i < length)
        str[i] = content[fist+i], i++;
    str[i] = '\0';

    *databcoin = str;
    exch->MtGoxUSDBTC = databcoin->toDouble();
/////////////////////////////////////////////

////// Поиск отношения курс биткоинов к EUR
    buffer = "btc_curr_mgoxE";
    fist = findBTC.indexOf(QString(">"), findBTC.indexOf(buffer));
    buffer = ">";
    last = findBTC.indexOf(QString("<"), fist);
    fist++;

    i = 0;
    length = last - fist;
    while(i < length)
        str[i] = content[fist+i], i++;
    str[i] = '\0';

    *databcoin = str;
    exch->MtGoxEURBTC = databcoin->toDouble();
/////////////////////////////////////////////

////// Поиск отношения курс биткоинов к RUB
    buffer = "btc_curr_mgoxR";
    fist = findBTC.indexOf(QString(">"), findBTC.indexOf(buffer));
    buffer = ">";
    last = findBTC.indexOf(QString("<"), fist);
    fist++;

    i = 0;
    length = last - fist;
    while(i < length)
        str[i] = content[fist+i], i++;
    str[i] = '\0';

    *databcoin = str;
    exch->MtGoxRUBBTC = databcoin->toDouble();
    str[i-1] = str[i];
/////////////////////////////////////////////

    reply5->deleteLater();
    webContent5.clear();
    return true;
}

/* Здесь парсим котировки для различных курсов валют */
bool Network::parserBTCe(){
    int i = 0;
    long int fist = 0, last = 0;
    int length = 0;
    char str[30];
    QString findBTC = QString(webContent6);
    char *content = webContent6.data();

////// Поиск отношения курс биткоинов к USD
    QString buffer = "btc_curr_btceU";
    fist = findBTC.indexOf(QString(">"), findBTC.indexOf(buffer));
    buffer = ">";
    last = findBTC.indexOf(QString("<"), fist);
    fist++;

    i = 0;
    length = last - fist;
    while(i < length)
        str[i] = content[fist+i], i++;
    str[i] = '\0';

    *databcoin = str;
    exch->BTCeUSDBTC = databcoin->toDouble();
    exch->MetaBankUSDBTC = exch->BTCeUSDBTC;
/////////////////////////////////////////////

////// Поиск отношения курс биткоинов к EUR
    buffer = "btc_curr_btceE";
    fist = findBTC.indexOf(QString(">"), findBTC.indexOf(buffer));
    buffer = ">";
    last = findBTC.indexOf(QString("<"), fist);
    fist++;

    i = 0;
    length = last - fist;
    while(i < length)
        str[i] = content[fist+i], i++;
    str[i] = '\0';

    *databcoin = str;
    exch->BTCeEURBTC = databcoin->toDouble();
/////////////////////////////////////////////

////// Поиск отношения курс биткоинов к RUB
    buffer = "btc_curr_btceR";
    fist = findBTC.indexOf(QString(">"), findBTC.indexOf(buffer));
    buffer = ">";
    last = findBTC.indexOf(QString("<"), fist);
    fist++;

    i = 0;
    length = last - fist;
    while(i < length)
        str[i] = content[fist+i], i++;
    str[i] = '\0';

    *databcoin = str;
    exch->BTCeRUBBTC = databcoin->toDouble();
    str[i-1] = str[i];
/////////////////////////////////////////////

    reply6->deleteLater();
    webContent6.clear();
    return true;
}

// Открываем файл с котировками обменников
bool Network::ExchangerOpen(char *file)
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
        exch->MtGoxUSDBTC = str->mid(fist,length-fist).toDouble();
        // Сохраняем валютную пару по которой будут проводиться расчеты
        fist = length + 3;
        index = str->indexOf(QString("="), fist);
        fist = index + 2;
        length = str->indexOf(QString(","), fist);
        exch->MtGoxEURBTC = str->mid(fist,length-fist).toDouble();
        // Сохраняем обменник по которому будем проводить расчеты
        fist = length + 3;
        index = str->indexOf(QString("="), fist);
        fist = index + 2;
        length = str->indexOf(QString(","), fist);
        exch->MtGoxRUBBTC = str->mid(fist,length-fist).toDouble();
        // Сохраняем величину увеличения сложности сети
        fist = length + 3;
        index = str->indexOf(QString("="), fist);
        fist = index + 2;
        length = str->indexOf(QString(","), fist);
        QString strr = str->mid(fist,length-fist);
        exch->BTCeUSDBTC = str->mid(fist,length-fist).toDouble();
        exch->MetaBankUSDBTC = exch->BTCeUSDBTC;
        // Сохраняем цену одного ватта электроэнергии
        fist = length + 3;
        index = str->indexOf(QString("="), fist);
        fist = index + 2;
        length = str->indexOf(QString(","), fist);
        exch->BTCeEURBTC = str->mid(fist,length-fist).toDouble();
        // Сохраняем количество дней майнинга
        fist = length + 3;
        index = str->indexOf(QString("="), fist);
        fist = index + 2;
        length = str->indexOf(QString("\r\n"), fist);
        exch->BTCeRUBBTC = str->mid(fist,length-fist).toDouble();
        openexch = true;
    }

    archOFminers->close();
    delete archOFminers;
}

// Обновляем архив майнеров
void Network::ExchangerUpdate()
{
    if(save == true)
    {
        QString str;

        QFile *archOFminers = new QFile("Exchanger.exh");
        archOFminers->open(QIODevice::WriteOnly | QIODevice::Truncate);

        QTextStream out(archOFminers);

        str = "&Mt.GoxUSD = " + QString::number(exch->MtGoxUSDBTC,'f',1) + ",\r\n";
        str += "Mt.GoxEUR = " + QString::number(exch->MtGoxEURBTC,'f',1) + ",\r\n";
        str += "Mt.GoxRUB = " + QString::number(exch->MtGoxRUBBTC,'f',1) + ",\r\n";
        str += "BTCeUSD = " + QString::number(exch->BTCeUSDBTC,'f',1) + ",\r\n";
        str += "BTCeEUR = " + QString::number(exch->BTCeEURBTC,'f',1) + ",\r\n";
        str += "BTCeRUB = " + QString::number(exch->BTCeRUBBTC,'f',1) + "\r\n" + ":;";
        out << str;

        archOFminers->close();
        delete archOFminers;
        save = false;
    }
}
