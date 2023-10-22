CC = gcc 
FLAGS =  -Wall -Werror -Wextra -std=c11# -fsanitize=address -g
CLEAN = clean
TESTS = TEST_PATH/
SRC_PATH = 3d_viewer/
SRCS = $(wildcard $(SRC_PATH)*.c)
OBJS = ${SRCS:.c = .o}
TEST_OBJS = ${SRCS_TEST:.c = .o}
SRCS_TEST = $(wildcard $(TESTS)*.c)
UNAME := $(shell uname)
TARGET = tests
GCOV = gcov_report
GCOV_FLAGS = -fprofile-arcs -ftest-coverage -lgcov 

ifeq ($(UNAME), Linux)
	TEST_FLAGS += -lcheck -lsubunit -lrt -lpthread -lm
else
	TEST_FLAGS += -lcheck -lm
endif

%.o: %.c
	${CC} -c ${FLAGS} $@ $<

${TESTS}%.o: ${TESTS}%.c
	${CC} -c ${FLAGS} $@ $< 

all: ${CLEAN} install ${TARGET} ${GCOV} dvi dist

${TARGET} : $(SRCS) ${SRCS_TEST}
	${CC} ${FLAGS} -o TESTS ${SRCS} ${SRCS_TEST} ${TEST_FLAGS} 
	./TESTS

install: 
	rm -rf Build_3D_VIEWER
	mkdir Build_3D_VIEWER
	cd Build_3D_VIEWER&&cmake ../3d_viewer
	cd Build_3D_VIEWER&&make

uninstall:
	rm -rf Build_3D_VIEWER

dist: |install
	tar --totals -cvf 3d_viewer.tar .

${GCOV}: build_file
	$(CC) $(FLAGS) $(GCOV_FLAGS) $(SRCS_TEST) $(SRCS) $(TEST_FLAGS) -o Build_test/${GCOV} 
	Build_test/./${GCOV} 
	lcov -o ${GCOV}.info -c -d .
	genhtml ${GCOV}.info -o html_report
	open html_report/index.html

dvi:
	cd 3d_viewer&&Doxygen .
	cd 3d_viewer&&open html/index.html

build_file:
	mkdir Build_test

$(CLEAN) :
	rm -rf 3d_viewer/html 3d_viewer/latex Build_3D_VIEWER Build_test TESTS *.a *.o $(TESTS).*o *.out app *.tar *.info html_report TEST/*.o *.gcno *.gcda