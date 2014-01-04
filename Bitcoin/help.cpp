#include "help.h"
#include "ui_help.h"

/*
 *  Конструктор дочернего окна программы.
 */
Help::Help(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Help)
{
    // Функция предварительной настройки окна вывода графиков
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    this->setWindowFlags(Qt::Dialog | Qt::WindowFullscreenButtonHint);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->label_2->setTextInteractionFlags(ui->label_2->textInteractionFlags() | Qt::TextSelectableByMouse);

    exist = true;
}

/*
 * Деструктор дочернего окна программы.
 */
Help::~Help()
{
    delete ui;
    exist = false;
}
