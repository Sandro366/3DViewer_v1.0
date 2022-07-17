#include "backend.h"

/*
  ! Основные задачи:
  ? 1) Загрузка модели из файла .obj (поддержка списка вершин и поверхностей) +
  ? 2) Перемещение модели на заданное расстояние относительно осей X, Y, Z +
  ? 3) Поворот модели на заданный угол относительно своих осей X, Y, Z +
  ? 4) Масштабирование модели на заданное значение +

  ! для удобства:
  * переменные называем в стиле camelCase
  * функции, типы данных (например при объявлении структур) в стиле snake_case
  * не забываем про комментарии
*/

int parcer(data_t* figure, char* fileName) {
  FILE* filePtr;
  int flagErr = 0;
  if ((filePtr = fopen(fileName, "r")) == NULL) {
    flagErr = 1;
  }
  if (!flagErr) {
    char* line = (char*)calloc(255, sizeof(char));
    while ((fgets(line, 255, filePtr)) != NULL) {
      if ((line[0] == 'v' || line[0] == 'f') &&
          line[1] == ' ') {  // парсим строку
        if (line[0] == 'v') {
          figure->countOfVertex += 1;
          int countRow = figure->countOfVertex - 1;
          // выделяем память под новую строку в матрице vertexes
          float** tmpPtr = (float**)realloc(
              figure->vertexes, (figure->countOfVertex) * sizeof(float*));
          if (tmpPtr) {
            figure->vertexes = tmpPtr;
            //  выделяем память под элементы в строке
            figure->vertexes[countRow] = (float*)calloc(3, sizeof(float));
            if (!figure->vertexes[countRow]) flagErr = 1;
          } else {
            flagErr = 1;
          }
        } else if (line[0] == 'f') {
          figure->countOfFacet += 1;
          int countSurf = figure->countOfFacet - 1;
          // выделяем память под массив структур surface
          surface_t** tmpPtr = (surface_t**)realloc(
              figure->surfaces, (figure->countOfFacet) * sizeof(surface_t*));
          if (tmpPtr) {
            figure->surfaces = tmpPtr;
            // выделяем память под экземпляры структур в массиве
            figure->surfaces[countSurf] =
                (surface_t*)calloc(1, sizeof(surface_t));
          } else {
            flagErr = 1;
          }
        }
        flagErr = pracer_line(figure, line);
        if (flagErr) break;
      } else {
        continue;
      }
      memset(line, '\0', 254);
    }
    free(line);
  }
  fclose(filePtr);
  return flagErr;
}

int pracer_line(data_t* figure, char* line) {
  int flagErr = 0;
  if (line) {
    size_t index = 0;  // индекс строки
    char ch = '0';
    char ch_pr = '0';
    int countIndex = 0;  // счетчик индексов в массиве куда записываем числа
    while ((ch = line[index]) != '\0') {
      if (((ch >= '0' && ch <= '9') || ch == '-') && ch_pr == ' ') {
        float num = get_digit(&index, line);  // считанное из файла число
        if (line[0] == 'v') {  //  кладем число в матрицу vertex
          int countRow = figure->countOfVertex - 1;
          figure->vertexes[countRow][countIndex++] = num;
        } else if (line[0] == 'f') {  // кладем число в массив facets
          int countSurf = figure->countOfFacet - 1;
          figure->surfaces[countSurf]->numberOfVertex += 1;
          figure->countOnFasetsInArray += 1;
          // выделяем память под массив чисел в структуре surface
          unsigned int* tmpPtr = (unsigned int*)realloc(
              figure->surfaces[countSurf]->vertexOfSurface,
              (figure->surfaces[countSurf]->numberOfVertex) *
                  sizeof(unsigned int));
          if (tmpPtr) {
            figure->surfaces[countSurf]->vertexOfSurface = tmpPtr;
            figure->surfaces[countSurf]->vertexOfSurface[countIndex++] =
                (unsigned int)num;
          } else {
            flagErr = 1;
          }
        }
        if (flagErr) break;
      } else {
        index++;
      }
      ch_pr = ch;
    }
  } else {
    flagErr = 1;
  }
  return flagErr;
}

float get_digit(size_t* index, char* line) {
  float number = 0;
  char numTmp[255] = "";
  int i = 0;
  while ((line[*index] >= '0' && line[*index] <= '9') || line[*index] == '.' ||
         line[*index] == '-') {
    numTmp[i] = line[*index];
    i++;
    *index = *index + 1;
  }
  number = strtof(numTmp, NULL);
  return number;
}

//* освобождение памяти
void clean(data_t* figure, float** array) {
  if (figure) {
    int countF = figure->countOfFacet;
    int countV = figure->countOfVertex;
    for (int i = 0; i < countV; i++) {
      free(figure->vertexes[i]);
    }
    free(figure->vertexes);
    for (int i = 0; i < countF; i++) {
      free(figure->surfaces[i]->vertexOfSurface);
      free(figure->surfaces[i]);
    }
    free(figure->surfaces);
    if (figure->facets != NULL) free(figure->facets);
    if (array) {
      for (int i = 0; i < countV; i++) {
        free(array[i]);
      }
      free(array);
    }
  }
}

//* сдвиг по оси Х
void move_x(data_t* figure, float mv) {
  for (unsigned i = 0; i < figure->countOfVertex; i++) {
    figure->vertexes[i][0] += mv;
  }
}

//* сдвиг по оси Y
void move_y(data_t* figure, float mv) {
  for (unsigned i = 0; i < figure->countOfVertex; i++) {
    figure->vertexes[i][1] += mv;
  }
}

//* сдвиг по оси Z
void move_z(data_t* figure, float mv) {
  for (unsigned i = 0; i < figure->countOfVertex; i++) {
    figure->vertexes[i][2] += mv;
  }
}

//* мастшабирование
void scale(data_t* figure, float k) {
  if (k > 0) {
    for (unsigned r = 0; r < figure->countOfVertex; r++) {
      for (unsigned c = 0; c < 3; c++) {
        figure->vertexes[r][c] *= k;
      }
    }
  }
}

//* поворот по оси Х
// x'=x;
// y':=y*cos(a)+z*sin(a);
// z':=-y*sin(a)+z*cos(a);
void rotation_x(data_t* figure, float angle) {
  for (unsigned i = 0; i < figure->countOfVertex; i++) {
    float yTmp = figure->vertexes[i][1];
    float zTmp = figure->vertexes[i][2];
    figure->vertexes[i][1] = yTmp * cos(angle) + zTmp * sin(angle);
    figure->vertexes[i][2] = -yTmp * sin(angle) + zTmp * cos(angle);
  }
}

//* поворот по оси Y
// x'=x*cos(a)-z*sin(a);
// y'=y;
// z'=x*sin(a)+z*cos(a);
void rotation_y(data_t* figure, float angle) {
  for (unsigned i = 0; i < figure->countOfVertex; i++) {
    float xTmp = figure->vertexes[i][0];
    float zTmp = figure->vertexes[i][2];
    figure->vertexes[i][0] = xTmp * cos(angle) - zTmp * sin(angle);
    figure->vertexes[i][2] = xTmp * sin(angle) + zTmp * cos(angle);
  }
}

//* поворот по оси Z
// x'=x*cos(a)+y*sin(a);
// y'=-x*sin(a)+y*cos(a);
// z'=z;
void rotation_z(data_t* figure, float angle) {
  for (unsigned i = 0; i < figure->countOfVertex; i++) {
    float xTmp = figure->vertexes[i][0];
    float yTmp = figure->vertexes[i][1];
    figure->vertexes[i][0] = xTmp * cos(angle) + yTmp * sin(angle);
    figure->vertexes[i][1] = -xTmp * sin(angle) + yTmp * cos(angle);
  }
}

//* формирование массива для соединения вершин
void create_array_draw(data_t* figure) {
  figure->facets = (unsigned int*)calloc(figure->countOnFasetsInArray * 2,
                                         sizeof(unsigned int));
  unsigned int k = 0;
  for (unsigned int c = 0; c < figure->countOfFacet; c++) {
    for (unsigned int i = 0; i < figure->surfaces[c]->numberOfVertex; i++) {
      if (i == 0) {
        k += 2;
        figure->facets[k - 2] = figure->surfaces[c]->vertexOfSurface[i] - 1;
      } else {
        figure->facets[k - 1] = figure->surfaces[c]->vertexOfSurface[i] - 1;
        figure->facets[k] = figure->surfaces[c]->vertexOfSurface[i] - 1;
        k += 2;
      }
    }
    figure->facets[k - 1] = figure->surfaces[c]->vertexOfSurface[0] - 1;
  }
}

void print(data_t* figure) {
  printf("vertex:\n");
  for (unsigned r = 0; r < figure->countOfVertex; r++) {
    for (int c = 0; c < 3; c++) {
      printf("%lf ", figure->vertexes[r][c]);
    }
    printf("\n");
  }
  printf("facet:\n");
  for (unsigned k = 0; k < figure->countOfFacet; k++) {
    for (unsigned int i = 0; i < figure->surfaces[k]->numberOfVertex; i++) {
      printf("%u ", figure->surfaces[k]->vertexOfSurface[i]);
    }
    printf("\n");
  }
  for (unsigned int i = 0; i < figure->countOnFasetsInArray * 2; i++) {
    printf("%u ", figure->facets[i]);
  }
}

void vertexes_copy(float** value, float** figure, unsigned int a) {
  for (unsigned r = 0; r < a; r++) {
    for (unsigned c = 0; c < 3; c++) {
      value[r][c] = figure[r][c];
    }
  }
}
