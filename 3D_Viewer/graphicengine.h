#ifndef GRAPHICENGINE_H
#define GRAPHICENGINE_H
#define GL_SILENCE_DEPRECATION
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QVector3D>

extern "C" {
#include "backend.h"
}

class GraphicEngine : protected QOpenGLFunctions {
 public:
  GraphicEngine();
  virtual ~GraphicEngine();
  void draw(QOpenGLShaderProgram* program);
  void get_file_name(QString fN);
  void get_scale(float k);
  void get_move_X(float k);
  void get_move_Y(float k);
  void get_move_Z(float k);
  void get_rotation_X(float k);
  void get_rotation_Y(float k);
  void get_rotation_Z(float k);
  void get_parametrs(QVector<float> p);
  data_t* figure;

 private:
  void init_figure();
  void add_in_buffer_V();
  void add_in_buffer_F();
  void affine_function();
  QString fileName;
  QOpenGLBuffer vertexBuf;
  QOpenGLBuffer indexBuf;
  float** defaultVertex;
  float scale_k;
  float value_move_X;
  float value_move_Y;
  float value_move_Z;
  float value_rotation_X;
  float value_rotation_Y;
  float value_rotation_Z;
  float lineRedNum;
  float lineGreenNum;
  float lineBluNum;
  float lineFat;
  float vertexRedNum;
  float vertexGreenNum;
  float vertexBlueNum;
  float vertexSize;
  int lineType;
  int vertexType;
};
#endif  // GRAPHICENGINE_H
