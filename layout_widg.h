#ifndef LAYOUT_WIDG_H
#define LAYOUT_WIDG_H

#include <QWidget>

class layout_widg : public QWidget
{
    Q_OBJECT
public:
    explicit layout_widg(QWidget *parent = 0);
    QTimer *iTimer;
    QStringList getNumGpios();

signals:

public slots:

};

#endif // LAYOUT_WIDG_H
