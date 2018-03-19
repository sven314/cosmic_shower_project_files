#include "mycounter.h"
#include "ui_mycounter.h"
#include <QDesktopWidget>
#include <QDebug>


MyCounter::MyCounter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyCounter)
{
    ui->setupUi(this);
    ui->label->installEventFilter(this);
    ui->pushButton->installEventFilter(this);
    int height = QApplication::desktop()->screenGeometry().height();
    int width = QApplication::desktop()->screenGeometry().width();
    ui->label->resize(width-50,height-50);
    QFont font = ui->label->font();
    font.setBold(true);
    font.setPixelSize(width/10);
    ui->label->setFont(font);
    QPalette palette = ui->label->palette();
    palette.setColor(ui->label->foregroundRole(),Qt::yellow);
    palette.setColor(ui->label->backgroundRole(),Qt::black);
    ui->label->setPalette(palette);
    this->setPalette(palette);
    connect(ui->pushButton,SIGNAL(pressed()),this,SLOT(reset()));

    counts = 0;
}

bool MyCounter::eventFilter(QObject *object, QEvent *event)
{
    if (/*object == ui->mListVi1w &&*/ event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Delete)
        {
            if(object==ui->label||object==this||object==ui->pushButton)
            {
                reset();
            }
        }
           // special Esc handling here
    }
    else
        return false;
}

void MyCounter::countUp()
{
    counts ++;
    refresh();
}

void MyCounter::reset()
{
    counts = 0;
    refresh();
}

void MyCounter::refresh()
{
    QString ausgabe = QString("counts: %1").arg(counts);
    ui->label->setText(ausgabe);
}

MyCounter::~MyCounter()
{
    delete ui;
}
