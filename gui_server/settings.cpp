#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    ui->comboBox->addItem("compare");
    ui->comboBox->addItem("differenz_zeilen");
    ui->comboBox->addItem("counter display");
    comboboxIndex = ui->comboBox->currentIndex();
    //ui->comboBox->addItem("compare");
}
/*
struct Compare {
    bool verbosity;
};
struct Differenz_Zeilen{
};
struct Display{
    int pigpio_channel;
};
*/
Settings::~Settings()
{
    delete ui;
}    int unwichtig;


void Settings::on_pushButton_2_clicked()
{
    close();
}

void Settings::on_comboBox_currentIndexChanged(const int &arg1)
{
    comboboxIndex = arg1;
}
