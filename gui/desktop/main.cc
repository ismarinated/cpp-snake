/**
 * @file main.cc
 * @brief Contains main function of desktop application.
 */

#include <QApplication>

#include "view.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  View w;
  w.show();
  return a.exec();
}
