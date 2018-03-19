#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    int comboboxIndex;
    ~Settings();

private slots:
    void on_pushButton_2_clicked();

    void on_comboBox_currentIndexChanged(const int &arg1);

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
