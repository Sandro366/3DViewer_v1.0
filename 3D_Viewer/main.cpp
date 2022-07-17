#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
#if defined(__linux__)
  setlocale(LC_NUMERIC, "C");
#endif  // __linux__
  MainWindow w;
  w.show();
  return a.exec();
}
