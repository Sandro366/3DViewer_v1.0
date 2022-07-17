#include "oglw.h"

OGLW::OGLW(QWidget *parent) : QOpenGLWidget{parent} {
  bgRedNum = 1.0;
  bgGreenNum = 1.0;
  bgBlueNum = 1.0;
  projectionType = 0;
}

OGLW::~OGLW() {
  makeCurrent();
  delete geometries;
  doneCurrent();
}

void OGLW::initializeGL() {
  initializeOpenGLFunctions();
  glClearDepth(1.0);  //Разрешить очистку буфера глубины
  initShaders();
  // Enable depth buffer
  glEnable(GL_DEPTH_TEST);  // Разрешить тест глубины
  geometries = new GraphicEngine;
}

void OGLW::resizeGL(int w, int h) {
  float aspect = float(w) / float(h ? h : 1);
  float zNear = 0.1, zFar = 10000.0, angle = 45.0;
  // Установка единичной матрицы
  projectionMatrix.setToIdentity();
  // Установка проекции
  if (projectionType == 1) {
    projectionMatrix.ortho(-1, 1, -1, 1, zNear, zFar);
  } else if (projectionType == 0) {
    projectionMatrix.perspective(angle, aspect, zNear, zFar);
  }
}

void OGLW::paintGL() {
  glClearColor(bgRedNum, bgGreenNum, bgBlueNum, 1.0f);  // цвет фона
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  QMatrix4x4 matrixView;
  matrixView.translate(0.0, 0.0, -10.0);
  // Установка модельно-видовой матрицы
  program.setUniformValue("mvp_matrix", projectionMatrix * matrixView);
  // Отрисовка
  geometries->draw(&program);
}

void OGLW::initShaders() {
  // Компиляция вершинного шейдера
  if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
    close();

  // Компиляция фрагментного шейдера
  if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                       ":/fshader.glsl"))
    close();

  // Линковка шейдерной программы
  if (!program.link()) close();

  // Бинд шейдерной программы
  if (!program.bind()) close();
}

void OGLW::broadcast_file_name(QString fN) {
  geometries->get_file_name(fN);
  update();
}

void OGLW::broadcast_scale(float k) {
  geometries->get_scale(k);
  update();
}

void OGLW::broadcast_move_X(float k) {
  geometries->get_move_X(k);
  update();
}

void OGLW::broadcast_move_Y(float k) {
  geometries->get_move_Y(k);
  update();
}

void OGLW::broadcast_move_Z(float k) {
  geometries->get_move_Z(k);
  update();
}

void OGLW::broadcast_rotation_X(float k) {
  geometries->get_rotation_X(k);
  update();
}

void OGLW::broadcast_rotation_Y(float k) {
  geometries->get_rotation_Y(k);
  update();
}

void OGLW::broadcast_rotation_Z(float k) {
  geometries->get_rotation_Z(k);
  update();
}

void OGLW::broabcast_parametrs(QVector<float> p) {
  geometries->get_parametrs(p);
  bgRedNum = p[13];
  bgGreenNum = p[12];
  bgBlueNum = p[11];
  projectionType = (int)p[10];
  resizeGL(640, 480);
  update();
}
