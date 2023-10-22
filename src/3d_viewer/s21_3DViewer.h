#ifndef S21_3DVIEWER_HEADER_FILE
#define S21_3DVIEWER_HEADER_FILE

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum flag {
  FAILURE = 0,
  SUCCESS = 1,
};

enum type {
  VERTEXES = 2,
  FACETS = 3,
};

enum move {
  X = 4,
  Y = 5,
  Z = 6,
};

typedef struct matrix {
  double **matrix;
  int rows;
  int cols;
} matrix_t;

typedef struct facets {
  int *vertexrs;
  int numbers_of_vertexes_in_facets;
} polygon_t;

typedef struct min_max {
  double x_min_max[2];
  double y_min_max[2];
  double z_min_max[2];
} min_max_t;

typedef struct data {
  unsigned int count_of_vertexrs;
  unsigned int count_of_facets;
  matrix_t matrix_3d;
  polygon_t *polygon;
  min_max_t min_max_values;
} data_t;

int open_obj_file(const char *file_name, data_t *matrix);
void parsing_obj_file(char *str, data_t *matrix, int *flag_err);
void number_entry(char *str, data_t *matrix, int type, int *flag_err);
void realloc_memory_for_vertexes_and_add_values(data_t *matrix, double *values);
void realloc_memory_for_facets_and_add_values(data_t *matrix, double *values,
                                              int count, int *flag_err);
void figure_centering(data_t *matrix);
void check_min_max_values(data_t *matrix);
void move_figure_x_y_z(data_t *matrix, double value, int type);
double support_increase_reduction_figure(data_t *matrix, double val);
void increase_reduction_figure(data_t *matrix, double val);
void rotation_by_x_y_z(data_t *matrix, double angle, int type);
void increase_reduction_figure_a(data_t *matrix, double val);
void clear_memory(data_t *matrix);
// void rotation_by_y(data_t *matrix, double angle);

#endif // S21_3DVIEWER_HEADER_FILE
