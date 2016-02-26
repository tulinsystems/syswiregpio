#include "layout_widg.h"
#include "gpiowidget.h"
//#include "num_gpios.h"
#include <QtGui>
#include <QTimer>
#include <QMessageBox>

int num_gpios;
int driver_ngpios;
int gpio_rem;
int btn_w = 170;
int btn_h = 140;

layout_widg::layout_widg(QWidget *parent) :
    QWidget(parent)
{
    /* timer section*/
    iTimer = new QTimer(this);
    iTimer->start(40);
    int i = 0;

    /* message box */
    QMessageBox msgBox;

    /* get driver_ngpios */
    QFile file3("/sys/class/gpio/gpiochip0/ngpio");
    if (file3.open(QFile::ReadOnly)) {
        QTextStream in3(&file3);
        in3 >> driver_ngpios;
        qDebug() << "driver_ngpios is:" << driver_ngpios << ".";
    } else {
        msgBox.setWindowTitle("Program could not be initialized");
        msgBox.setText("Driver does not appear to be present");
        msgBox.exec();
        //should exit here, after OK
    }

    /* if driver present, do the exporting */
    QFile file4("/sys/class/gpio/export");
    if (file4.open(QFile::WriteOnly)) {
        file4.close();
        for (int i=1; i<=driver_ngpios; i++) {
            file4.open(QFile::WriteOnly);
            QTextStream out4(&file4);
            out4 << i;
            file4.close();
        }
    } else {
        msgBox.setWindowTitle("Program could not be initialized");
        msgBox.setText("Program not run as administrator");
        msgBox.exec();
    }

    QGridLayout *grid = new QGridLayout(this);
      grid->setSpacing(2);

      num_gpios = getNumGpios().size();
      qDebug() << num_gpios;

      //get all the names
      QStringList names = getNumGpios();

      //make all but the last row
      for (i=0; i<(num_gpios / 4); i++) {
        for (int j=0; j<4; j++) {
          int gpioNum = i*4+j;
          QString name = names[gpioNum];
          gpioWidget *btn = new gpioWidget(name, gpioNum);
          btn->setFixedSize(btn_w, btn_h);
          grid->addWidget(btn, i, j);

          connect(this->iTimer, SIGNAL(timeout()), btn, SLOT(updateLED()));
          connect(btn, SIGNAL(stopiTimer()), this->iTimer, SLOT(stop()));
          connect(btn, SIGNAL(startiTimer()), this->iTimer, SLOT(start()));
        }
      }

      gpio_rem = (num_gpios % 4); //from this we know if we need to create a last row or not

      //make last row, if num_gpios is not divisible by 4
      if (gpio_rem != 0) {
          for (int j=0; j<gpio_rem; j++) {
            int gpioNum = i*4+j;
            QString name = names[gpioNum];
            gpioWidget *btn = new gpioWidget(name, gpioNum);
            btn->setFixedSize(btn_w, btn_h);
            grid->addWidget(btn, i, j);

            connect(this->iTimer, SIGNAL(timeout()), btn, SLOT(updateLED()));
            connect(btn, SIGNAL(stopiTimer()), this->iTimer, SLOT(stop()));
            connect(btn, SIGNAL(startiTimer()), this->iTimer, SLOT(start()));
          }
      }

      setLayout(grid);
}

QStringList layout_widg::getNumGpios(){
    QDir dir("/sys/class/gpio");
         dir.setFilter(QDir::Dirs); //print only directories

         QFileInfoList list = dir.entryInfoList();
         QStringList dirNames;

         for (int i = 0; i < list.size(); ++i) {
             QFileInfo fileInfo = list.at(i);
             dirNames << QString("%1").arg(fileInfo.fileName()).toAscii().data();
         }


         QStringList result;
              foreach (const QString &str, dirNames) {
                  if (!str.contains("export") && !str.contains("gpiochip") && !str.startsWith("."))
                      result += str;
         }


return result;
}
