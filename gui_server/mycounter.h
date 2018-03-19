#ifndef MYCOUNTER_H
#define MYCOUNTER_H

#include <QDialog>
#include <QEvent>
#include <QKeyEvent>

namespace Ui {
class MyCounter;
}

class MyCounter : public QDialog
{
    Q_OBJECT

public:
    explicit MyCounter(QWidget *parent = 0);
    unsigned long long int counts;
    ~MyCounter();
    void refresh();
    bool eventFilter(QObject *object, QEvent *event);

public slots:
    void reset();
    void countUp();

private:
    Ui::MyCounter *ui;
};

#endif // MYCOUNTER_H
