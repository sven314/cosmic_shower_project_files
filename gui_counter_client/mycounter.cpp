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
   // ui->label->resize(width-50,height-ui->pushButton->height()-50);
    QFont font = ui->label->font();
    font.setBold(true);
    font.setPixelSize(width/6);
    ui->label->setFont(font);
   // ui->label_2->resize(width,height/5);
    font.setPixelSize(width/20);
    ui->label_2->setFont(font);
    QPalette palette = ui->label->palette();
    palette.setColor(ui->label->foregroundRole(),Qt::yellow);
    palette.setColor(ui->label->backgroundRole(),Qt::black);
    ui->label->setPalette(palette);
    ui->label_2->setText("avg rate: 0 cts/(s m"+QString::fromUtf8("\u00B2")+")");
    this->setPalette(palette);
    connect(ui->pushButton,SIGNAL(pressed()),this,SLOT(reset()));
    //countString = "%1 cts";
    counts = 0;
    oldCounts=0;
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(on_timeUp()));
    milliseconds = 10000;
    timer->setInterval(milliseconds);
    timer->setSingleShot(false);
    timer->start();
}

bool MyCounter::eventFilter(QObject *object, QEvent *event)
{ // Keyhandling: closes counter and program if escape is pressed, resets counter if delete
    if (/*object == ui->mListVi1w &&*/ event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Delete)
        {
            if(object==ui->label||object==this||object==ui->pushButton)
            {
                reset();
            }
        }
    }
    else
    {
        return false;
    }
}

void MyCounter::on_timeUp()
{
    double rate = counts - oldCounts;
    oldCounts = counts;
    rate = rate/milliseconds*1000.0*15.625;
    rate = (int)(rate*100);
    rate= rate/100;
    ui->label_2->setText(QString("avg rate: %1 cts/(s m"+QString::fromUtf8("\u00B2")+")").arg(rate));
}

void MyCounter::countUp()
{
    counts ++;
    refresh();
}

void MyCounter::reset()
{
    oldCounts = 0;
    counts = 0;
    refresh();
}

void MyCounter::refresh()
{
    QString ausgabe = QString("%1").arg(counts);
    ui->label->setText(ausgabe);
}

MyCounter::~MyCounter()
{
    delete ui;
}
