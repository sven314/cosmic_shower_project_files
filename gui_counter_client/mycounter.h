#ifndef MYCOUNTER_H
#define MYCOUNTER_H

#include <QDialog>
#include <QEvent>
#include <QKeyEvent>
#include <QTimer>

namespace Ui {
class MyCounter;
}

class MyCounter : public QDialog
{
    Q_OBJECT

public:
    int milliseconds;
    QString countString;
    explicit MyCounter(QWidget *parent = 0);
    unsigned long long int counts, oldCounts;
    ~MyCounter();
    void refresh();
    bool eventFilter(QObject *object, QEvent *event);
    QTimer *timer;


public slots:
    void on_timeUp();
    void reset();
    void countUp();

private:
    Ui::MyCounter *ui;
};

#endif // MYCOUNTER_H
