#include "gpiowidget.h"
#include <QtGui>
#include <QRect>
#include <QFile>
#include <QTextStream>
#include <QFrame>
#include <QMessageBox>

QString gpio_path = "/sys/class/gpio/";

gpioWidget::gpioWidget(QString name, int gpioNum, QWidget *parent) : QFrame(parent)
{
    /*save the arguments as both strings and integers*/
    gpioString = QString::number(gpioNum);
    gpioNumber = gpioNum;
    nameString = name;

    /*set the style of the box surrounding the widget*/
    this->setFrameStyle(QFrame::Box | QFrame::Plain);
    this->setLineWidth(1);

    /*set the name label for the widget, so the user knows what it controls*/
    widgetLabel = new QLabel();
    widgetLabel->setText(name);
    widgetLabel->setFont(QFont("Arial", 7));
    widgetLabel->setAlignment(Qt::AlignTop | Qt::AlignRight);

    /*make the indicator LED*/
    pic = new QWidget();
    pic->setStyleSheet("image: url(:/leds/images/led-disabled.png);");
    pic->setFixedSize(20,20);

    /*make the toggle button*/
    toggleswitch = new QtSvgToggleSwitch(this);
    toggleswitch->setSkin("button-skin");
    toggleswitch->setFixedSize(55,90);

    /*make the sliding switch*/
    slider1 = new QtSvgSlideSwitch(this);
    slider1->setSkin("slider-skin");
    slider1->setFixedSize(80,27);

    /*make a label next to the LED*/
    LEDlabel = new QLabel();
    LEDlabel->setText("input: ");
    LEDlabel->setAlignment(Qt::AlignRight | Qt::AlignCenter);
    LEDlabel->setDisabled(1);

    /*get the current state of the corresponding GPIO and flick the switches accordingly*/
    //Slider switch
    if (gpioReadDirection()) slider1->setChecked(0);
    else { slider1->setChecked(1); sliderLeftRight(1); }
    //Toggle switch
    if (gpioReadValue()) toggleswitch->setChecked(1);
    else toggleswitch->setChecked(0);

    /*finally, lay all the components of a widget out on a grid*/
    QGridLayout *grid = new QGridLayout(this);
    grid->setSpacing(2);
    grid->addWidget(widgetLabel, 1, 2, 2, Qt::AlignRight);
    grid->addWidget(toggleswitch, 1, 1);
    grid->addWidget(slider1, 2, 1);
    grid->addWidget(LEDlabel,2,2);
    grid->addWidget(pic, 2,3);

    this->setLayout(grid);

    connect(slider1,SIGNAL(toggled(bool)),this,SLOT(sliderLeftRight(bool)));
    connect(toggleswitch,SIGNAL(toggled(bool)),this,SLOT(toggleButtonUpDown(bool)));
}


void gpioWidget::sliderLeftRight(bool state) {
    if (state) { //activated
        LEDlabel->setEnabled(1);
        pic->setStyleSheet("image: url(:/leds/images/led-off.png);");
        toggleswitch->setCheckable(0);
        toggleswitch->setSkin("button-skin-disabled");
        gpioDirectionIN();
    } else {    //deactivated
        LEDlabel->setDisabled(1);
        pic->setStyleSheet("image: url(:/leds/images/led-disabled.png);");
        toggleswitch->setCheckable(1);
        toggleswitch->setSkin("button-skin");
        gpioOutputLO();
        gpioDirectionOUT();
    }
}

void gpioWidget::toggleButtonUpDown(bool state) {
    if (state) {
        gpioOutputHI();
    } else {
        gpioOutputLO();
    }
}



void gpioWidget::updateLED() {
    //if the slider is off, keep the led disabled
    if (slider1->isChecked() == 0) {
        pic->setStyleSheet("image: url(:/leds/images/led-disabled.png);");
        return;
    }

    //read from 'value'
    if (gpioReadValue()) {
        pic->setStyleSheet("image: url(:leds/images/led-on.png)");
    } else {
        pic->setStyleSheet("image: url(:leds/images/led-off.png)");
    }
}

bool gpioWidget::gpioReadValue() {
    QFile vFileName(gpio_path + nameString + "/value");

    if (!vFileName.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //makeErrorBox("Error", "Could not update input status.");
        return 0;
    }

    //read the file
    QTextStream in(&vFileName);
    QString line = in.readLine();

    if (line == "1") return 1;
    else return 0;
}

bool gpioWidget::gpioReadDirection() {
    QFile dFileName(gpio_path + nameString + "/direction");

    if (!dFileName.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //makeErrorBox("Error", "Could not update input status.");
        return 0;
    }

    //read the file
    QTextStream in(&dFileName);
    QString line = in.readLine();

    if (line == "out") return 1;
    else return 0;
}

void gpioWidget::gpioDirectionOUT() {
    //set the direction - OUT
    QFile dFileName(gpio_path + nameString + "/direction");
            if (dFileName.open(QFile::WriteOnly | QFile::Truncate)) {
                QTextStream out(&dFileName);
                out << "out" << endl;
    } else {
                makeErrorBox("Error", "GPIO direction could not be set.");
            }
}

void gpioWidget::gpioDirectionIN() {
    //set the direction - OUT
    QFile dFileName(gpio_path + nameString + "/direction");
            if (dFileName.open(QFile::WriteOnly | QFile::Truncate)) {
                QTextStream out(&dFileName);
                out << "in" << endl;
    } else {
                makeErrorBox("Error", "GPIO direction could not be set.");
            }

}

void gpioWidget::gpioOutputHI() {
    //set the value - HIGH
    QFile vFileName(gpio_path + nameString + "/value");
            if (vFileName.open(QFile::WriteOnly | QFile::Truncate)) {
                 QTextStream out(&vFileName);
                 out << "1" << endl;
    } else {
                makeErrorBox("Error", "GPIO output level could not be set.");
            }
}


void gpioWidget::gpioOutputLO() {
    //set the value - LOW
    QFile vFileName(gpio_path + nameString + "/value");
            if (vFileName.open(QFile::WriteOnly | QFile::Truncate)) {
                 QTextStream out(&vFileName);
                 out << "0" << endl;
    } else {
                makeErrorBox("Error", "GPIO output level could not be set.");
            }
}

void gpioWidget::makeErrorBox(QString errorTitle, QString details) {
    QMessageBox errorBox;
    errorBox.setWindowTitle(errorTitle);
    errorBox.setText(details);
    errorBox.exec();
}
