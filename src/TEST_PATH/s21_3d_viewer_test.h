#ifndef SRC_S21_3D_VIEWER
#define SRC_S21_3D_VIEWER

#include <check.h>
#include <unistd.h>

#include "../3d_viewer/s21_3DViewer.h"

Suite *suite_s21_3d(void);

void run_tests(void);
void run_testcase(Suite *testcase);

#endif  //
