#pragma once
#include "ui_MainWindow.h"
#include <QtGui>

namespace Ui {
class MainWindow;
}

namespace app {

class MainWindow
  : public QDialog
  , public Ui::MainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
};

}// namespace app
