#include "graphicengine.h"

GraphicEngine::GraphicEngine()
    : vertexBuf(QOpenGLBuffer::VertexBuffer),
      indexBuf(QOpenGLBuffer::IndexBuffer) {
  initializeOpenGLFunctions();
  scale_k = 1.0;
  value_move_X = 0.0;
  value_move_Y = 0.0;
  value_move_Z = 0.0;
  value_rotation_X = 0.0;
  value_rotation_Y = 0.0;
  value_rotation_Z = 0.0;
  lineRedNum = 0.0;
  lineGreenNum = 0.0;
  lineBluNum = 0.0;
  lineFat = 1.0;
  vertexRedNum = 0.0;
  vertexGreenNum = 0.0;
  vertexBlueNum = 0.0;
  vertexSize = 1.0;
  lineType = 0;
  vertexType = 0;
  figure = nullptr;
  defaultVertex = nullptr;
}

GraphicEngine::~GraphicEngine() {
  if (vertexBuf.isCreated() && indexBuf.isCreated()) {
    vertexBuf.destroy();
    indexBuf.destroy();
  }
  if (figure) clean(figure, defaultVertex);
  if (figure) free(figure);
}

void GraphicEngine::init_figure() {
  if (figure) clean(figure, defaultVertex);
  QByteArray tmp = fileName.toLocal8Bit();
  char* fN = tmp.data();
  figure = (data_t*)calloc(1, sizeof(data_t));
  parcer(figure, fN);
  defaultVertex = (float**)calloc(figure->countOfVertex, sizeof(float*));
  for (unsigned int i = 0; i < figure->countOfVertex; i++) {
    defaultVertex[i] = (float*)calloc(3, sizeof(float));
  }
  vertexes_copy(defaultVertex, figure->vertexes, figure->countOfVertex);
  create_array_draw(figure);
  add_in_buffer_F();
  affine_function();
}

void GraphicEngine::add_in_buffer_V() {
  if (vertexBuf.isCreated()) vertexBuf.destroy();
  vertexBuf.create();
  // Инициализируем массив вершин для отрисовки
  QVector3D vertices[figure->countOfVertex];
  for (unsigned int i = 0; i < figure->countOfVertex; i++) {
    vertices[i] = QVector3D(figure->vertexes[i][0], figure->vertexes[i][1],
                            figure->vertexes[i][2]);
  }
  vertexBuf.bind();
  vertexBuf.allocate(vertices, figure->countOfVertex * sizeof(QVector3D));
  vertexBuf.release();
}

void GraphicEngine::add_in_buffer_F() {
  indexBuf.create();
  indexBuf.bind();
  indexBuf.allocate(figure->facets,
                    (figure->countOnFasetsInArray * 2) * sizeof(unsigned int));
  indexBuf.release();
}

void GraphicEngine::draw(QOpenGLShaderProgram* program) {
  vertexBuf.bind();
  indexBuf.bind();
  int colorLocation = program->uniformLocation("color");
  int vertexLocation = program->attributeLocation("a_position");
  program->enableAttributeArray(vertexLocation);
  program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3,
                              sizeof(QVector3D));
  glLineWidth(lineFat);  // толщина линии
  if (lineType == 1) {
    glEnable(GL_LINE_STIPPLE);  // прерывистая линия
    glLineStipple(2, 255);      // маска
  }
  glUniform4f(colorLocation, lineRedNum, lineGreenNum, lineBluNum,
              1.0f);  // цвет
  glDrawElements(GL_LINES, indexBuf.size(), GL_UNSIGNED_INT, nullptr);
  if (lineType == 1) glDisable(GL_LINE_STIPPLE);

  glPointSize(vertexSize);  // размер точки
  glUniform4f(colorLocation, vertexRedNum, vertexGreenNum, vertexBlueNum,
              1.0f);  // цвет
  if (vertexType == 1) {
    glEnable(GL_POINT_SMOOTH);  // круглые вершины
  }
  if (vertexType)
    glDrawElements(GL_POINTS, indexBuf.size(), GL_UNSIGNED_INT, nullptr);
  if (vertexType == 1) glDisable(GL_POINT_SMOOTH);
}

void GraphicEngine::get_file_name(QString fN) {
  fileName = fN;
  init_figure();
}

void GraphicEngine::get_scale(float k) {
  scale_k = k;
  if (!fileName.isNull()) affine_function();
}

void GraphicEngine::get_move_X(float k) {
  value_move_X = k;
  if (!fileName.isNull()) affine_function();
}

void GraphicEngine::get_move_Y(float k) {
  value_move_Y = k;
  if (!fileName.isNull()) affine_function();
}

void GraphicEngine::get_move_Z(float k) {
  value_move_Z = k;
  if (!fileName.isNull()) affine_function();
}

void GraphicEngine::get_rotation_X(float k) {
  value_rotation_X = k;
  if (!fileName.isNull()) affine_function();
}

void GraphicEngine::get_rotation_Y(float k) {
  value_rotation_Y = k;
  if (!fileName.isNull()) affine_function();
}

void GraphicEngine::get_rotation_Z(float k) {
  value_rotation_Z = k;
  if (!fileName.isNull()) affine_function();
}

void GraphicEngine::affine_function() {
  vertexes_copy(figure->vertexes, defaultVertex, figure->countOfVertex);
  scale(figure, scale_k);
  rotation_x(figure, value_rotation_X);
  rotation_y(figure, value_rotation_Y);
  rotation_z(figure, value_rotation_Z);
  move_x(figure, value_move_X);
  move_y(figure, value_move_Y);
  move_z(figure, value_move_Z);
  add_in_buffer_V();
}

void GraphicEngine::get_parametrs(QVector<float> p) {
  lineRedNum = p[9];
  lineGreenNum = p[8];
  lineBluNum = p[7];
  lineFat = p[6];
  vertexRedNum = p[5];
  vertexGreenNum = p[4];
  vertexBlueNum = p[3];
  vertexSize = p[2];
  lineType = (int)p[1];
  vertexType = (int)p[0];
  affine_function();
}
