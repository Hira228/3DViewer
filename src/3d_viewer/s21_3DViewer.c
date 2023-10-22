#include "s21_3DViewer.h"

int open_obj_file(const char *file_name, data_t *matrix) {
  ssize_t read = 0;
  size_t len = 0;
  char *str = NULL;
  int return_status = SUCCESS;
  FILE *file = fopen(file_name, "r");
  int flag_err = 0;

  if (file) {
    while ((read = getline(&str, &len, file)) != -1) {
      parsing_obj_file(str, matrix, &flag_err);
    }
    if (!flag_err) {
      figure_centering(matrix);
      increase_reduction_figure(matrix, 0.5);
      free(str);
    }
  } else {
    return_status = FAILURE;
  }
  if (return_status == SUCCESS && flag_err) return_status = FAILURE;
  return return_status;
}

void parsing_obj_file(char *str, data_t *matrix, int *flag_err) {
  switch (str[0]) {
    case 'v':
      if (str[1] == ' ') {
        matrix->count_of_vertexrs++;
        number_entry(str, matrix, VERTEXES, flag_err);
      }
      break;

    case 'f':
      if (str[1] == ' ') {
        matrix->count_of_facets++;
        number_entry(str, matrix, FACETS, flag_err);
      }
      break;

    default:
      break;
  }
}

void number_entry(char *str, data_t *matrix, int type, int *flag_err) {
  int i = 0;
  int count = 1;
  char *number_str = malloc(sizeof(char) * 50);
  double *values = NULL;
  int index = 0;
  double val = 0;

  while (str[i] != '\0' && str[i] != '\n') {
    if (str[i] == '.' || str[i] == '-' || (str[i] >= '0' && str[i] <= '9') ||
        (str[i] == '-' && (i == 0 || str[i - 1] == ' '))) {
      number_str[index++] = str[i];
    } else if (str[i] == '/') {
      while (str[i + 1] != ' ' && str[i + 1] != '\n') {
        i++;
      }
    } else if (index > 0) {
      number_str[index] = '\0';
      val = atof(number_str);
      values = (double *)realloc(values, count * sizeof(double));
      values[count - 1] = val;
      free(number_str);
      number_str = malloc(sizeof(char) * 50);
      count++;
      index = 0;
    }
    i++;
  }
  if (index > 0) {
    number_str[index] = '\0';
    val = atof(number_str);
    values = (double *)realloc(values, count * sizeof(double));
    values[count - 1] = val;
    count++;
  }

  if (type == VERTEXES) {
    realloc_memory_for_vertexes_and_add_values(matrix, values);
  } else {
    realloc_memory_for_facets_and_add_values(matrix, values, count, flag_err);
  }
  free(values);
  free(number_str);
}

void realloc_memory_for_vertexes_and_add_values(data_t *matrix,
                                                double *values) {
  matrix->matrix_3d.rows++;
  matrix->matrix_3d.matrix = (double **)realloc(
      matrix->matrix_3d.matrix, matrix->matrix_3d.rows * sizeof(double *));
  if (values)
    matrix->matrix_3d.matrix[matrix->matrix_3d.rows - 1] =
        (double *)malloc(matrix->matrix_3d.cols * sizeof(double));
  for (int i = 0; i < matrix->matrix_3d.cols; i++) {
    matrix->matrix_3d.matrix[matrix->matrix_3d.rows - 1][i] = values[i];
  }
}

void realloc_memory_for_facets_and_add_values(data_t *matrix, double *values,
                                              int count, int *flag_err) {
  matrix->polygon = (polygon_t *)realloc(
      matrix->polygon, matrix->count_of_facets * sizeof(polygon_t));
  matrix->polygon[matrix->count_of_facets - 1].vertexrs =
      (int *)malloc((count - 1) * sizeof(int));
  matrix->polygon[matrix->count_of_facets - 1].numbers_of_vertexes_in_facets =
      --count;
  for (int i = 0; i < count; i++) {
    matrix->polygon[matrix->count_of_facets - 1].vertexrs[i] = (int)values[i];
    if (values[i] > matrix->count_of_vertexrs || values[i] <= 0) *flag_err = 1;
  }
}

void check_min_max_values(data_t *matrix) {
  matrix->min_max_values.x_min_max[0] =
      matrix->matrix_3d.matrix[matrix->matrix_3d.rows - 1][0];
  matrix->min_max_values.y_min_max[0] =
      matrix->matrix_3d.matrix[matrix->matrix_3d.rows - 1][1];
  matrix->min_max_values.z_min_max[0] =
      matrix->matrix_3d.matrix[matrix->matrix_3d.rows - 1][2];
  matrix->min_max_values.x_min_max[1] =
      matrix->matrix_3d.matrix[matrix->matrix_3d.rows - 1][0];
  matrix->min_max_values.y_min_max[1] =
      matrix->matrix_3d.matrix[matrix->matrix_3d.rows - 1][1];
  matrix->min_max_values.z_min_max[1] =
      matrix->matrix_3d.matrix[matrix->matrix_3d.rows - 1][2];
  for (int i = 0; i < matrix->matrix_3d.rows; i++) {
    for (int j = 0; j < matrix->matrix_3d.cols; j++) {
      if (j == 0 &&
          matrix->matrix_3d.matrix[i][j] > matrix->min_max_values.x_min_max[0])
        matrix->min_max_values.x_min_max[0] = matrix->matrix_3d.matrix[i][j];
      if (j == 0 &&
          matrix->matrix_3d.matrix[i][j] < matrix->min_max_values.x_min_max[1])
        matrix->min_max_values.x_min_max[1] = matrix->matrix_3d.matrix[i][j];
      if (j == 1 &&
          matrix->matrix_3d.matrix[i][j] > matrix->min_max_values.y_min_max[0])
        matrix->min_max_values.y_min_max[0] = matrix->matrix_3d.matrix[i][j];
      if (j == 1 &&
          matrix->matrix_3d.matrix[i][j] < matrix->min_max_values.y_min_max[1])
        matrix->min_max_values.y_min_max[1] = matrix->matrix_3d.matrix[i][j];
      if (j == 2 &&
          matrix->matrix_3d.matrix[i][j] > matrix->min_max_values.z_min_max[0])
        matrix->min_max_values.z_min_max[0] = matrix->matrix_3d.matrix[i][j];
      if (j == 2 &&
          matrix->matrix_3d.matrix[i][j] < matrix->min_max_values.z_min_max[1])
        matrix->min_max_values.z_min_max[1] = matrix->matrix_3d.matrix[i][j];
    }
  }
}

void figure_centering(data_t *matrix) {
  check_min_max_values(matrix);

  double centerX = matrix->min_max_values.x_min_max[1] +
                   (matrix->min_max_values.x_min_max[0] -
                    matrix->min_max_values.x_min_max[1]) /
                       2;
  double centerY = matrix->min_max_values.y_min_max[1] +
                   (matrix->min_max_values.y_min_max[0] -
                    matrix->min_max_values.y_min_max[1]) /
                       2;
  double centerZ = matrix->min_max_values.z_min_max[1] +
                   (matrix->min_max_values.z_min_max[0] -
                    matrix->min_max_values.z_min_max[1]) /
                       2;

  for (int i = 0; i < matrix->matrix_3d.rows; i++) {
    for (int j = 0; j < matrix->matrix_3d.cols; j++) {
      if (j == 0) matrix->matrix_3d.matrix[i][j] -= centerX;
      if (j == 1) matrix->matrix_3d.matrix[i][j] -= centerY;
      if (j == 2) matrix->matrix_3d.matrix[i][j] -= centerZ;
    }
  }
}

void move_figure_x_y_z(data_t *matrix, double value,
                       int type) {  // ЧТОБЫ ФИГУРА ДВИГАЛАСЬ НУЖНО ПЕРЕДАВАТЬ
                                    // !!!РАЗНОСТЬ НОВОГО И СТАРОГО АРГУМЕНТА!!!
  for (int i = 0; i < matrix->matrix_3d.rows; i++) {
    for (int j = 0; j < matrix->matrix_3d.cols; j++) {
      if (type == X && j == 0) matrix->matrix_3d.matrix[i][j] += value;
      if (type == Y && j == 1) matrix->matrix_3d.matrix[i][j] += value;
      if (type == Z && j == 2) matrix->matrix_3d.matrix[i][j] += value;
    }
  }
}

double support_increase_reduction_figure(data_t *matrix, double val) {
  double diff_X =
      matrix->min_max_values.x_min_max[0] - matrix->min_max_values.x_min_max[1];
  double diff_Y =
      matrix->min_max_values.y_min_max[0] - matrix->min_max_values.y_min_max[1];
  double diff_Z =
      matrix->min_max_values.z_min_max[0] - matrix->min_max_values.z_min_max[1];

  double max_diff = diff_X;
  if (diff_Y > max_diff) max_diff = diff_Y;
  if (diff_Z > max_diff) max_diff = diff_Z;

  double scal = (val - (val * (-1))) / max_diff;

  return scal;
}

void increase_reduction_figure(
    data_t *matrix,
    double val) {  // ЧТОБЫ ФИГУРА УМЕНЬШИЛАСЬ ИЛИ УВЕЛИЧИЛАСЬ НУЖНО ПЕРЕДАВАТЬ
                   // !!!ЧАСТНОЕ ОТ НОВОГО И СТАРОГО АРГУМЕНТА!!!
  for (int i = 0; i < matrix->matrix_3d.rows; i++) {
    for (int j = 0; j < matrix->matrix_3d.cols; j++) {
      if (j == 0)
        matrix->matrix_3d.matrix[i][j] *=
            support_increase_reduction_figure(matrix, val);
      if (j == 1)
        matrix->matrix_3d.matrix[i][j] *=
            support_increase_reduction_figure(matrix, val);
      if (j == 2)
        matrix->matrix_3d.matrix[i][j] *=
            support_increase_reduction_figure(matrix, val);
    }
  }
}

void increase_reduction_figure_a(
    data_t *matrix,
    double val) {  // ЧТОБЫ ФИГУРА УМЕНЬШИЛАСЬ ИЛИ УВЕЛИЧИЛАСЬ НУЖНО ПЕРЕДАВАТЬ
                   // !!!ЧАСТНОЕ ОТ НОВОГО И СТАРОГО АРГУМЕНТА!!!
  for (int i = 0; i < matrix->matrix_3d.rows; i++) {
    for (int j = 0; j < matrix->matrix_3d.cols; j++) {
      if (j == 0) matrix->matrix_3d.matrix[i][j] *= val;
      if (j == 1) matrix->matrix_3d.matrix[i][j] *= val;
      if (j == 2) matrix->matrix_3d.matrix[i][j] *= val;
    }
  }
}

void rotation_by_x_y_z(data_t *matrix, double angle, int type) {
  for (int i = 0; i < matrix->matrix_3d.rows; i++) {
    double temp_x = matrix->matrix_3d.matrix[i][0];
    double temp_y = matrix->matrix_3d.matrix[i][1];
    double temp_z = matrix->matrix_3d.matrix[i][2];

    if (type == X) {
      matrix->matrix_3d.matrix[i][1] =
          cos(angle) * temp_y - sin(angle) * temp_z;
      matrix->matrix_3d.matrix[i][2] =
          sin(angle) * temp_y + cos(angle) * temp_z;
    } else if (type == Y) {
      matrix->matrix_3d.matrix[i][0] =
          cos(angle) * temp_x + sin(angle) * temp_z;
      matrix->matrix_3d.matrix[i][2] =
          -sin(angle) * temp_x + cos(angle) * temp_z;
    } else if (type == Z) {
      matrix->matrix_3d.matrix[i][0] =
          cos(angle) * temp_x - sin(angle) * temp_y;
      matrix->matrix_3d.matrix[i][1] =
          sin(angle) * temp_x + cos(angle) * temp_y;
    }
  }
}

void clear_memory(data_t *matrix) {
  for (int i = 0; i < matrix->matrix_3d.rows; i++) {
    free(matrix->matrix_3d.matrix[i]);
  }

  free(matrix->matrix_3d.matrix);

  for (unsigned int i = 0; i < matrix->count_of_facets; i++) {
    free(matrix->polygon[i].vertexrs);
  }

  free(matrix->polygon);
}
