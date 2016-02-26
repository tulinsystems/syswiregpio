#ifndef GPIOWIDGET_H
#define GPIOWIDGET_H

#include <QWidget>
#include <QtGui>
#include "qtsvgslideswitch.h"
#include "qtsvgtoggleswitch.h"

class gpioWidget : public QFrame
{
    Q_OBJECT
public:
    explicit gpioWidget(QString name, int gpioNum,QWidget *parent = 0);
    QtSvgToggleSwitch *toggleswitch;
    QtSvgSlideSwitch *slider1;
    QWidget *pic;
    QLabel *widgetLabel;
    QLabel *LEDlabel;
    QLabel *toggleLabel;

private:
    void gpioDirectionOUT();
    void gpioDirectionIN();
    void gpioOutputHI();
    void gpioOutputLO();
    QString gpioString;
    QString nameString;
    int gpioNumber;
    void makeErrorBox(QString errorTitle, QString details);
    bool gpioReadValue();
    bool gpioReadDirection();

signals:
    void stopiTimer();
    void startiTimer();

public slots:
    void updateLED();
    void sliderLeftRight(bool state);
    void toggleButtonUpDown(bool state);
};

#endif // GPIOWIDGET_H
