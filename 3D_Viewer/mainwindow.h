#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <QTimer>
#include <fstream>

#include "oglw.h"
#include "qgifimage.h"
using namespace std;
#include <QByteArray>
#include <QFile>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  QVector<float> parametrs;

 private slots:
  void on_ScaleSpinBox_valueChanged(double k);
  void on_rotationXspinBox_valueChanged(double k);
  void on_rotationYspinBox_valueChanged(double k);
  void on_rotationZspinBox_valueChanged(double k);
  void on_moveXspinBox_valueChanged(double k);
  void on_moveYspinBox_valueChanged(double k);
  void on_moveZspinBox_valueChanged(double k);
  void on_getFileName_clicked();
  void on_setParametrs_clicked();
  void on_projectionType_activated(int index);
  void on_lineType_activated(int index);
  void on_vertexType_activated(int index);
  void on_clearButton_clicked();
  void on_BMP_button_clicked();
  void on_JPG_button_clicked();
  void on_GIF_button_clicked();
  void onTimeout();

 private:
  Ui::MainWindow *ui;
  void save_in_file();
  char *fN;
  int proectionTypeValue;
  int lineTypeValue;
  int vertexTypeValue;
  QGifImage gif;
  int startTime, tmpTime;
  QTimer *timer;
  float setValue_config(QFile *file);
  QString qFileName;
};

#endif  // MAINWINDOW_H
