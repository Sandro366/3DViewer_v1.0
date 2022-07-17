#ifndef SRC_BACKEND_H_
#define SRC_BACKEND_H_
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//* Структура для хранения поверхностей (строки, начинающиеся на "f", в .obj)
typedef struct surfaceStruct {
  unsigned int* vertexOfSurface;
  unsigned int numberOfVertex;
} surface_t;

//* Структура для хранения всех данных объекта
typedef struct figureData {
  unsigned int countOfVertex;
  unsigned int countOfFacet;
  float** vertexes;  // матрица вершин (строки на "v")
  surface_t** surfaces;  // массив структур с данными о поверхностях
  unsigned int* facets;  // массив с номерами соединяемых вершин
  unsigned int
      countOnFasetsInArray;  // кол-во элементов во всех масивах surfaces
} data_t;

//? основные функции

// парсер obj-файла
int parcer(data_t* figure, char* fileName);
int pracer_line(data_t* figure, char* line);
float get_digit(size_t* index, char* line);
void clean(data_t* figure, float** array);

// функции сдвига
void move_x(data_t* figure, float mv);
void move_y(data_t* figure, float mv);
void move_z(data_t* figure, float mv);

// функции вращения
void rotation_x(data_t* figure, float angle);
void rotation_y(data_t* figure, float angle);
void rotation_z(data_t* figure, float angle);

// функция масштабирования
void scale(data_t* figure, float k);

void create_array_draw(data_t* figure);
void print(data_t* figure);
void vertexes_copy(float** value, float** figure, unsigned int a);

#endif  // SRC_BACKEND_H_
