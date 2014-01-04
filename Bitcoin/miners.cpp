#include "miners.h"

/*/ Описание класса вывода исторических данных на график.
 Предполагаетсся считыватьь данныне с exel файла
 Возможно будет реализованаа динамическая считка данных с сайта  BitCoin.
/*/
bool Miner::save = false;

// Конструктор и первый парсер файла инициализации.
Miner::Miner(char *file)
{
    archOFminers = new QFile(file);

    // Проверяем, возможно ли открыть наш файл для чтения
    if (archOFminers->open(QFile::ReadWrite))
    {
        byte = archOFminers->readAll(); //считываем все данные с файла в объект data
        int i = 0;
        bool exit = 0;
        int fist = 0, index = 0;
        QString *str = new QString(byte);
        int length = 0;

        // Находим количество майнеров в файле
        while(!exit)
        {
            index = str->indexOf(QString("&"), fist);
            if((index != fist && index > 0) || (index == fist && i == 0))
                fist = index+1, i++;
            else
                exit = 1;
        }

        // Запоминаем количество майнеров и создаем указатели на объекты
        numbers = i;
        miner = new FunctionsMiner[numbers];
        reserv_miner = new FunctionsMiner[numbers];

        // Инициализация переменных перед заполнением данных майнеров в структуру
        exit = 0;
        i = 0;
        fist = 0;

        // Заполнение данных майнеров в структуру
        while(!exit)
        {
            index = str->indexOf(QString("&"), fist);
            if(i < numbers)
            {
                fist = index+1;
                length = str->indexOf(QString(","), fist);
                miner[i].manufactur = str->mid(fist,length-fist);
                length = str->indexOf(QString(","), fist=length+1);
                miner[i].model = str->mid(fist,length-fist);
                length = str->indexOf(QString(","), fist=length+1);
                miner[i].hashrate = str->mid(fist,length-fist).toDouble();
                length = str->indexOf(QString(","), fist=length+1);
                miner[i].price = str->mid(fist,length-fist).toDouble();
                length = str->indexOf(QString(","), fist=length+1);
                miner[i].power = str->mid(fist,length-fist).toDouble();
                int day, month, years;
                length = str->indexOf(QString("."), fist=length+1);
                day = str->mid(fist,length-fist).toInt();
                length = str->indexOf(QString("."), fist=length+1);
                month = str->mid(fist,length-fist).toInt();
                length = str->indexOf(QString("\r\n"), fist=length+1);
                years = str->mid(fist,length-fist).toInt();
                miner[i].startdata.setDate(years,month,day);

                // Копирование майнеров для некоторых изменений
                reserv_miner[i] = miner[i];
                i++;
            }
            else
                exit = 1;
        }

        qDebug() << "Обнаружено " << i << "майнеров.";
    }
    else
    {
        qDebug() << "Невозможно открыть файл, возможно файл не существует.";
    }

    archOFminers->close();
    delete archOFminers;
}

// Обновляем архив майнеров
void Miner::MinersUpdate(){
    if(save == true)
    {
        int i = 0;
        QString str;

        archOFminers = new QFile("\Miners.data");
        archOFminers->open(QIODevice::WriteOnly | QIODevice::Truncate);

        QTextStream out(archOFminers);

        while(i < numbers)
        {
            str = '&' + miner[i].manufactur + ',';
            str += miner[i].model + ',';
            str += QString::number(miner[i].hashrate, 'f', 1) + ',';
            str += QString::number(miner[i].price, 'f', 2) + ',';
            str += QString::number(miner[i].power, 'f', 2) + ',';
            str += QString::number(miner[i].startdata.day()) + '.' +
                   QString::number(miner[i].startdata.month()) + '.' +
                   QString::number(miner[i].startdata.year()) + "\r\n";
            out << str;
            i++;
        }
        archOFminers->close();
        delete archOFminers;
        save = false;
    }
}
