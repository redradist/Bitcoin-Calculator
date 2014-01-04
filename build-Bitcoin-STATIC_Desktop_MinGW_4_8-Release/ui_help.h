/********************************************************************************
** Form generated from reading UI file 'help.ui'
**
** Created by: Qt User Interface Compiler version 5.1.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELP_H
#define UI_HELP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Help
{
public:
    QLabel *label;
    QLabel *label_2;

    void setupUi(QWidget *Help)
    {
        if (Help->objectName().isEmpty())
            Help->setObjectName(QStringLiteral("Help"));
        Help->setEnabled(true);
        Help->resize(331, 241);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Help->sizePolicy().hasHeightForWidth());
        Help->setSizePolicy(sizePolicy);
        Help->setMinimumSize(QSize(331, 241));
        Help->setMaximumSize(QSize(331, 241));
        QIcon icon;
        icon.addFile(QStringLiteral(":/titles/question.png"), QSize(), QIcon::Normal, QIcon::Off);
        Help->setWindowIcon(icon);
        label = new QLabel(Help);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 0, 271, 31));
        label_2 = new QLabel(Help);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 50, 311, 181));
        label_2->setMinimumSize(QSize(311, 0));
        label_2->setAcceptDrops(false);
        label_2->setScaledContents(false);
        label_2->setWordWrap(false);

        retranslateUi(Help);

        QMetaObject::connectSlotsByName(Help);
    } // setupUi

    void retranslateUi(QWidget *Help)
    {
        Help->setWindowTitle(QApplication::translate("Help", "About", 0));
        label->setText(QApplication::translate("Help", "<html><head/><body><p><span style=\" font-size:14pt; font-style:italic; color:#ff0000;\">About BitCoin Mining Calculator</span></p></body></html>", 0));
        label_2->setText(QApplication::translate("Help", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt;\">Created by </span><span style=\" font-size:12pt; font-weight:600; font-style:italic; color:#aa0000;\">redradist</span><span style=\" font-size:12pt;\">.</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:11pt;\">   This free software was writed to help BitCoin </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a name=\"result_bo"
                        "x\"></a><span style=\" font-size:11pt;\">c</span><span style=\" font-size:11pt;\">ommunity to undestend who match must pay </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:11pt;\">for BitCoin Mining Board.</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:2px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:9pt; color:#55aa00;\">GitHub</span><span style=\" font-size:9pt;\">: https://github.com/redradist</span></p>\n"
"<p style=\" margin-top:2px; margin-bottom:2px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:9pt; color:#00aaff;\">Skype</span><span style=\" font-size:9pt;\">: god_of_radist</span></p>\n"
"<p style=\" margin-top:2px; margin-bottom:2px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:9pt; color:#ff0000;\">E-mail</span><span "
                        "style=\" font-size:9pt;\">: redradist@gmail.com </span></p>\n"
"<p style=\" margin-top:2px; margin-bottom:2px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:9pt;\">(please write in topic: &quot;Like yU software&quot;)</span></p>\n"
"<p style=\" margin-top:2px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:9pt; color:#ffaa00;\">BTC address</span><span style=\" font-size:9pt;\">: 1LPdb7N344DuzwmozJfVmPfLYsuiGin2m8</span></p></body></html>", 0));
    } // retranslateUi

};

namespace Ui {
    class Help: public Ui_Help {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELP_H
