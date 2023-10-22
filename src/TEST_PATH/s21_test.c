#include "s21_3d_viewer_test.h"

START_TEST(test_one) {
  data_t matrix = {0};
  matrix.matrix_3d.cols = 3;
  matrix.matrix_3d.rows = 0;
  matrix.count_of_facets = 0;
  matrix.count_of_vertexrs = 0;

  int value = SUCCESS;
  value = open_obj_file("/src/MODELS/false.obj", &matrix);
  ck_assert_int_eq(value, FAILURE);
  clear_memory(&matrix);
}
END_TEST

START_TEST(test_two) {
  data_t matrix = {0};
  matrix.matrix_3d.cols = 3;
  matrix.matrix_3d.rows = 0;
  matrix.count_of_facets = 0;
  matrix.count_of_vertexrs = 0;

  int value = SUCCESS;
  value = open_obj_file("obj_files/seat.obj", &matrix);
  ck_assert_int_eq(value, SUCCESS);
  clear_memory(&matrix);
}
END_TEST

Suite *suite_s21_3d(void) {
  Suite *s = suite_create("suite_3d");
  TCase *tc = tcase_create("case_3d");

  tcase_add_test(tc, test_one);
  tcase_add_test(tc, test_two);

  suite_add_tcase(s, tc);
  return s;
}
