#ifndef OGLW_H
#define OGLW_H

#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QWidget>

#include "graphicengine.h"

class OGLW : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  OGLW(QWidget *parent = nullptr);
  virtual ~OGLW();
  void broadcast_file_name(QString fileName);
  void broadcast_scale(float k);
  void broadcast_move_X(float k);
  void broadcast_move_Y(float k);
  void broadcast_move_Z(float k);
  void broadcast_rotation_X(float k);
  void broadcast_rotation_Y(float k);
  void broadcast_rotation_Z(float k);
  void broabcast_parametrs(QVector<float> p);
  int projectionType;
  GraphicEngine *geometries = nullptr;

 protected:
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();
  void initShaders();

 private:
  QOpenGLShaderProgram program;
  QMatrix4x4 projectionMatrix;
  float bgRedNum;
  float bgGreenNum;
  float bgBlueNum;
};

#endif  // OGLW_H
