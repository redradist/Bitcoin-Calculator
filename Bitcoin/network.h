#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QTimer>
#include <QDate>
#include <QTextCodec>
#include <QTextStream>

enum {
    BLOCKS = 8,
    TIMEDATA = 7,
    HASHRATE = 1,
    DIFFICULTY = 4
};

//// Котировки основных обменников Bitcoin
struct ofNetwork {
    long long int hashrateAll[3000];
    double difficultyAll[3000];
    long int timedataAll[3000];
    int historylengthAll;
    int numberOfBlocksAll[3000];

    long long int hashrate[3000];
    double difficulty[3000];
    long int timedata[3000];
    int historylength;

    double BTCblock;
    double Timeblock;
    long int BlockofNetwork;
};

//// Котировки основных обменников Bitcoin
struct Exchanger {
    // Котировки на бирже MtGox
    double MtGoxUSDBTC;
    double MtGoxEURBTC;
    double MtGoxRUBBTC;

    // Котировки на бирже BTC-e
    double BTCeUSDBTC;
    double BTCeEURBTC;
    double BTCeRUBBTC;

    // Котировки в обменнике MetaBank
    double MetaBankUSDBTC;
};

class Network: public QObject
{
    Q_OBJECT

public:
    Network(QString urlString1, QString urlString2, QString urlString3,
            QString urlString4, QString urlString5, QString urlString6,
            QString urlString7);

    void getReply1();
    void getReplyBTCforBlock();
    void getReplyTimeblock();
    void getReplyBlocksofNetwork();
    void getReplyMtGox();
    void getReplyBTCe();

    bool parser1();
    bool parserBTCforBlock();
    bool parserTimeblock();
    bool parserBlocksofNetwork();
    bool parserMtGox();
    bool parserBTCe();

    ofNetwork *bit;
    Exchanger *exch;
    static bool save;

    bool finished;
    bool downloaded;
    QNetworkReply *reply1;
    QNetworkReply *reply2;
    QNetworkReply *reply3;
    QNetworkReply *reply4;
    QNetworkReply *reply5;
    QNetworkReply *reply6;
    QNetworkReply *reply7;

    QTimer *saving;
    bool error;
    bool parsed;
    bool openexch;
private:
    QUrl url1;
    QUrl url2;
    QUrl url3;
    QUrl url4;
    QUrl url5;
    QUrl url6;
    QUrl url7;
    QNetworkAccessManager *manager;

    QByteArray webContent1;
    QByteArray webContent2;
    QByteArray webContent3;
    QByteArray webContent4;
    QByteArray webContent5;
    QByteArray webContent6;
    QByteArray webContent7;
    QString *databcoin;
    bool OpenHirstory();
    bool SaveHirstory();
    bool ExchangerOpen(char *file);

public slots:
    void downloadFinished1();
    void downloadFinishedBTCforBlock();
    void downloadFinishedTimeblock();
    void downloadFinishedBlocksofNetwork();
    void downloadFinishedMtGox();
    void downloadFinishedBTCe();
    void ExchangerUpdate();
};


#endif // NETWORK_H
