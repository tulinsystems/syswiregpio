#ifndef CENTER_H
#define CENTER_H

void center(QWidget &widget)
{
  int x, y;
  int screenWidth;
  int screenHeight;
  int width, height;
  QSize windowSize;

  QDesktopWidget *desktop = QApplication::desktop();

  width = widget.frameGeometry().width();
  height = widget.frameGeometry().height();

  screenWidth = desktop->width();
  screenHeight = desktop->height();

  x = (screenWidth - width) / 2;
  y = (screenHeight - height) / 2;

  widget.move( x, y );

}

#endif // CENTER_H
