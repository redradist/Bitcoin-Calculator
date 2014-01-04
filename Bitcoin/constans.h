#ifndef CONSTANS_H
#define CONSTANS_H

enum {
    HASH = 0,
    KHASH,
    MHASH,
    GHASH,
    THASH,
    PHASH,
    EHASH,
    ZHASH,
    YHASH
};

struct Calculation {
    // Котировки на бирже MtGox
    int Cote; // Рост котировок валюты
    int ExchCote; // Пара по которой производится расчет
    int Excher; // Обменик (типа биржа MtGox)
    int DiffiIncs; // 2 недельный рост сложности сети

    double PriceofW; // Цена за килоВатт
    int Days; // Число дней расчета
};

#endif // CONSTANS_H
