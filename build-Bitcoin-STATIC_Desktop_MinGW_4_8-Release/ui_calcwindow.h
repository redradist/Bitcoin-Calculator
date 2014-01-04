/********************************************************************************
** Form generated from reading UI file 'calcwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.1.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALCWINDOW_H
#define UI_CALCWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CalcWindow
{
public:
    QGraphicsView *graphicsView;
    QGroupBox *groupBox;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLabel *label_6;
    QWidget *horizontalLayoutWidget_4;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_8;
    QLabel *label_9;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *label_5;
    QWidget *horizontalLayoutWidget_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLabel *label_7;
    QWidget *horizontalLayoutWidget_5;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_4;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;

    void setupUi(QWidget *CalcWindow)
    {
        if (CalcWindow->objectName().isEmpty())
            CalcWindow->setObjectName(QStringLiteral("CalcWindow"));
        CalcWindow->resize(645, 471);
        CalcWindow->setMinimumSize(QSize(645, 471));
        CalcWindow->setMaximumSize(QSize(645, 471));
        QIcon icon;
        icon.addFile(QStringLiteral(":/titles/coin.png"), QSize(), QIcon::Normal, QIcon::Off);
        CalcWindow->setWindowIcon(icon);
        CalcWindow->setWindowOpacity(0);
        graphicsView = new QGraphicsView(CalcWindow);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(5, 20, 631, 351));
        groupBox = new QGroupBox(CalcWindow);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 380, 621, 81));
        horizontalLayoutWidget_2 = new QWidget(groupBox);
        horizontalLayoutWidget_2->setObjectName(QStringLiteral("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(10, 20, 121, 21));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(horizontalLayoutWidget_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        label_6 = new QLabel(horizontalLayoutWidget_2);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(label_6);

        horizontalLayoutWidget_4 = new QWidget(groupBox);
        horizontalLayoutWidget_4->setObjectName(QStringLiteral("horizontalLayoutWidget_4"));
        horizontalLayoutWidget_4->setGeometry(QRect(300, 50, 201, 21));
        horizontalLayout_4 = new QHBoxLayout(horizontalLayoutWidget_4);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_8 = new QLabel(horizontalLayoutWidget_4);
        label_8->setObjectName(QStringLiteral("label_8"));

        horizontalLayout_4->addWidget(label_8);

        label_9 = new QLabel(horizontalLayoutWidget_4);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(label_9);

        horizontalLayoutWidget = new QWidget(groupBox);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(140, 50, 151, 21));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        label_5 = new QLabel(horizontalLayoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_5);

        horizontalLayoutWidget_3 = new QWidget(groupBox);
        horizontalLayoutWidget_3->setObjectName(QStringLiteral("horizontalLayoutWidget_3"));
        horizontalLayoutWidget_3->setGeometry(QRect(10, 50, 121, 21));
        horizontalLayout_3 = new QHBoxLayout(horizontalLayoutWidget_3);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(horizontalLayoutWidget_3);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_3->addWidget(label_3);

        label_7 = new QLabel(horizontalLayoutWidget_3);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(label_7);

        horizontalLayoutWidget_5 = new QWidget(groupBox);
        horizontalLayoutWidget_5->setObjectName(QStringLiteral("horizontalLayoutWidget_5"));
        horizontalLayoutWidget_5->setGeometry(QRect(140, 20, 151, 21));
        horizontalLayout_5 = new QHBoxLayout(horizontalLayoutWidget_5);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        label_13 = new QLabel(horizontalLayoutWidget_5);
        label_13->setObjectName(QStringLiteral("label_13"));

        horizontalLayout_5->addWidget(label_13);

        label_14 = new QLabel(horizontalLayoutWidget_5);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_5->addWidget(label_14);

        label_4 = new QLabel(CalcWindow);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 0, 65, 19));
        label_10 = new QLabel(CalcWindow);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(90, 0, 111, 19));
        label_11 = new QLabel(CalcWindow);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(230, 0, 41, 19));
        label_12 = new QLabel(CalcWindow);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(280, 0, 111, 19));

        retranslateUi(CalcWindow);

        QMetaObject::connectSlotsByName(CalcWindow);
    } // setupUi

    void retranslateUi(QWidget *CalcWindow)
    {
        CalcWindow->setWindowTitle(QApplication::translate("CalcWindow", "Calculated", 0));
        groupBox->setTitle(QApplication::translate("CalcWindow", "Mined Results", 0));
        label_2->setText(QApplication::translate("CalcWindow", "BTC Generete", 0));
        label_6->setText(QApplication::translate("CalcWindow", "0", 0));
        label_8->setText(QApplication::translate("CalcWindow", "Clean Cash, $", 0));
        label_9->setText(QApplication::translate("CalcWindow", "0", 0));
        label->setText(QApplication::translate("CalcWindow", "PowerCost, $", 0));
        label_5->setText(QApplication::translate("CalcWindow", "0", 0));
        label_3->setText(QApplication::translate("CalcWindow", "Cash, $", 0));
        label_7->setText(QApplication::translate("CalcWindow", "0", 0));
        label_13->setText(QApplication::translate("CalcWindow", "Power, kW*h", 0));
        label_14->setText(QApplication::translate("CalcWindow", "0", 0));
        label_4->setText(QApplication::translate("CalcWindow", "Manufactur", 0));
        label_10->setText(QApplication::translate("CalcWindow", "Text", 0));
        label_11->setText(QApplication::translate("CalcWindow", "Model", 0));
        label_12->setText(QApplication::translate("CalcWindow", "Text", 0));
    } // retranslateUi

};

namespace Ui {
    class CalcWindow: public Ui_CalcWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALCWINDOW_H
