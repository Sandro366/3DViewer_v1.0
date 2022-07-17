#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  QFile file("../config.conf");
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    ui->bgRedNum->setValue(setValue_config(&file));
    ui->bgGreenNum->setValue(setValue_config(&file));
    ui->bgBlueNum->setValue(setValue_config(&file));
    proectionTypeValue = setValue_config(&file);
    ui->projectionType->setCurrentIndex(proectionTypeValue);
    ui->lineRedNum->setValue(setValue_config(&file));
    ui->lineGreenNum->setValue(setValue_config(&file));
    ui->lineBlueNum->setValue(setValue_config(&file));
    ui->lineFatNum->setValue(setValue_config(&file));
    ui->vertexRedNum->setValue(setValue_config(&file));
    ui->vertexGreenNum->setValue(setValue_config(&file));
    ui->vertexBlueNum->setValue(setValue_config(&file));
    ui->vertexSizeNum->setValue(setValue_config(&file));
    lineTypeValue = setValue_config(&file);
    ui->lineType->setCurrentIndex(lineTypeValue);
    vertexTypeValue = setValue_config(&file);
    ui->vertexType->setCurrentIndex(vertexTypeValue);
  }
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_projectionType_activated(int index) {
  if (index == 0) {
    proectionTypeValue = 0;  // центральная
  } else if (index == 1) {
    proectionTypeValue = 1;  // параллельная
  }
}

void MainWindow::on_lineType_activated(int index) {
  if (index == 0) {
    lineTypeValue = 0;  // сплошная
  } else if (index == 1) {
    lineTypeValue = 1;  // пунктирная
  }
}

void MainWindow::on_vertexType_activated(int index) {
  if (index == 0) {
    vertexTypeValue = 0;  // отсутствуют
  } else if (index == 1) {
    vertexTypeValue = 1;  // круг
  } else if (index == 2) {
    vertexTypeValue = 2;  // квадрат
  }
}

void MainWindow::on_ScaleSpinBox_valueChanged(double k) {
  ui->openGLWidget->broadcast_scale(float(k));
}

void MainWindow::on_rotationXspinBox_valueChanged(double k) {
  ui->openGLWidget->broadcast_rotation_X(float(k));
}

void MainWindow::on_rotationYspinBox_valueChanged(double k) {
  ui->openGLWidget->broadcast_rotation_Y(float(k));
}

void MainWindow::on_rotationZspinBox_valueChanged(double k) {
  ui->openGLWidget->broadcast_rotation_Z(float(k));
}

void MainWindow::on_moveXspinBox_valueChanged(double k) {
  ui->openGLWidget->broadcast_move_X(float(k));
}

void MainWindow::on_moveYspinBox_valueChanged(double k) {
  ui->openGLWidget->broadcast_move_Y(float(k));
}

void MainWindow::on_moveZspinBox_valueChanged(double k) {
  ui->openGLWidget->broadcast_move_Z(float(k));
}

void MainWindow::on_getFileName_clicked() {
  qFileName = QFileDialog::getOpenFileName(this, "Open file", "../", "*.obj");
  ui->openGLWidget->broadcast_file_name(qFileName);
  ui->buttonImageLable->setText(qFileName);
  QString vertex_num_string =
      QString::number(ui->openGLWidget->geometries->figure->countOfVertex);
  ui->vertexCountNum->setText(vertex_num_string);
  QString facet_num_string =
      QString::number(ui->openGLWidget->geometries->figure->countOfFacet);
  ui->facetsCountNum->setText(facet_num_string);
  if (!qFileName.isEmpty())
    ui->statusbar->showMessage("Файл успешно выбран");
  else
    ui->statusbar->showMessage("Файл не выбран");
}

void MainWindow::save_in_file() {
  ofstream file;
  file.open("../config.conf");
  file << QString::number((float)ui->bgRedNum->value()).toStdString() << endl;
  file << QString::number((float)ui->bgGreenNum->value()).toStdString() << endl;
  file << QString::number((float)ui->bgBlueNum->value()).toStdString() << endl;
  file << QString::number(proectionTypeValue).toStdString() << endl;
  file << QString::number((float)ui->lineRedNum->value()).toStdString() << endl;
  file << QString::number((float)ui->lineGreenNum->value()).toStdString()
       << endl;
  file << QString::number((float)ui->lineBlueNum->value()).toStdString()
       << endl;
  file << QString::number((float)ui->lineFatNum->value()).toStdString() << endl;
  file << QString::number((float)ui->vertexRedNum->value()).toStdString()
       << endl;
  file << QString::number((float)ui->vertexGreenNum->value()).toStdString()
       << endl;
  file << QString::number((float)ui->vertexBlueNum->value()).toStdString()
       << endl;
  file << QString::number((float)ui->vertexSizeNum->value()).toStdString()
       << endl;
  file << QString::number(lineTypeValue).toStdString() << endl;
  file << QString::number(vertexTypeValue).toStdString() << endl;
  file.close();
}

void MainWindow::on_setParametrs_clicked() {
  parametrs.push_front((float)ui->bgRedNum->value());        // 13
  parametrs.push_front((float)ui->bgGreenNum->value());      // 12
  parametrs.push_front((float)ui->bgBlueNum->value());       // 11
  parametrs.push_front((float)proectionTypeValue);           // 10
  parametrs.push_front((float)ui->lineRedNum->value());      // 9
  parametrs.push_front((float)ui->lineGreenNum->value());    // 8
  parametrs.push_front((float)ui->lineBlueNum->value());     // 7
  parametrs.push_front((float)ui->lineFatNum->value());      // 6
  parametrs.push_front((float)ui->vertexRedNum->value());    // 5
  parametrs.push_front((float)ui->vertexGreenNum->value());  // 4
  parametrs.push_front((float)ui->vertexBlueNum->value());   // 3
  parametrs.push_front((float)ui->vertexSizeNum->value());   // 2
  parametrs.push_front((float)lineTypeValue);                // 1
  parametrs.push_front((float)vertexTypeValue);              // 0
  save_in_file();
  if (!qFileName.isNull()) ui->openGLWidget->broabcast_parametrs(parametrs);
  ui->statusbar->showMessage("Настройки применены и успешно сохранены");
}

void MainWindow::on_clearButton_clicked() {
  ui->ScaleSpinBox->setValue(1.0);
  ui->rotationXspinBox->setValue(0.0);
  ui->rotationYspinBox->setValue(0.0);
  ui->rotationZspinBox->setValue(0.0);
  ui->moveXspinBox->setValue(0.0);
  ui->moveYspinBox->setValue(0.0);
  ui->moveZspinBox->setValue(0.0);
  ui->bgRedNum->setValue(1.0);
  ui->bgGreenNum->setValue(1.0);
  ui->bgBlueNum->setValue(1.0);
  ui->lineRedNum->setValue(0.0);
  ui->lineGreenNum->setValue(0.0);
  ui->lineBlueNum->setValue(0.0);
  ui->vertexRedNum->setValue(0.0);
  ui->vertexGreenNum->setValue(0.0);
  ui->vertexBlueNum->setValue(0.0);
  ui->lineFatNum->setValue(1.0);
  ui->vertexSizeNum->setValue(1.0);
  ui->projectionType->setCurrentIndex(0);
  ui->lineType->setCurrentIndex(0);
  ui->vertexType->setCurrentIndex(0);
  on_setParametrs_clicked();
  ui->statusbar->showMessage("Настройки сброшены");
}

void MainWindow::on_BMP_button_clicked() {
  QPixmap screenshot(ui->openGLWidget->size());
  ui->openGLWidget->render(&screenshot);
  screenshot.save(QString::fromStdString("bmp_screen.bmp"), "BMP");
  ui->statusbar->showMessage("Создан скриншот в формате BMP");
}

void MainWindow::on_JPG_button_clicked() {
  QPixmap screenshot(ui->openGLWidget->size());
  ui->openGLWidget->render(&screenshot);
  screenshot.save(QString::fromStdString("jpg_screen.jpg"), "JPG");
  ui->statusbar->showMessage("Создан скриншот в формате JPG");
}

void MainWindow::on_GIF_button_clicked() {
  startTime = 0, tmpTime = 100;
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
  timer->start(100);
  ui->statusbar->showMessage("Начата запись картинки в GIF формате (5 секунд)");
}

void MainWindow::onTimeout() {
  if (startTime == tmpTime) {
    QPixmap screenshot1(ui->openGLWidget->size());
    ui->openGLWidget->render(&screenshot1);
    screenshot1.save(QString::fromStdString("tmp.jpg"), "JPG");
    QImage image("tmp.JPG");
    gif.addFrame(image, 100);
    tmpTime += 100;
  }
  if (startTime == 5000) {
    gif.save("demo.gif");
    timer->stop();
    remove("tmp.JPG");
    ui->statusbar->showMessage("Создана картинка в формате GIF");
  }
  startTime += 100;
}

float MainWindow::setValue_config(QFile *file) {
  QByteArray line = file->readLine();
  QString str = line.data();
  str.chop(1);
  float result = str.toFloat();
  return result;
}
