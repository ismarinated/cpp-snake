OS = $(shell uname -s)

FLAGS = -Wall -Werror -Wextra -std=c++20
NCURSES_LIB = -lncurses
TEST_LIB =  -lgtest

ifeq ($(OS), Linux)
	TEST_LIB += -lpthread -lrt -lsubunit -lm
else ifeq ($(OS), Darwin)
	TEST_LIB += -lsubunit -lm
endif

SRC = gui/cli/*.c brick_game/tetris/*.c brick_game/snake/*.cc

all: install

install: clean
	mkdir build
	g++ $(FLAGS) $(SRC) $(NCURSES_LIB) -o build/brick_game

install_desktop:
	rm -rf build/desktop
	cmake -S gui/desktop -B ./build/desktop
	cmake --build ./build/desktop --target desktop_app -j 4

uninstall:
	rm -rf build

dvi:
	doxygen Doxyfile

dist:
	tar -czvf brickgame.tar.gz Makefile brick_game gui tests Doxyfile # diagram.png

test: clean clean_txt
	g++ $(FLAGS) tests/tests.cc brick_game/snake/*.cc -o tests/test $(TEST_LIB)
	./tests/test
	rm -f snake_score.txt

add_coverage_flag:
	$(eval FLAGS += --coverage)

gcov_report: add_coverage_flag test
	lcov --capture --directory . --output-file coverage.info
	lcov --extract coverage.info "*/src/brick_game/snake/*" --output-file coverage.info
	genhtml coverage.info --output-directory gcov_report
	rm -rf *.gcno *.gcda *.gcov *.html *.css *.info
	rm -rf tests/*.gcno tests/*.gcda

clean:
	rm -rf build docs
	rm -rf *.o *.a tests/test
	rm -rf *.gcno *.gcda *.gcov *.html *.css gcov_report *.info
	rm -rf tests/*.gcno tests/*.gcda
	rm -rf *gz

clean_txt:
	rm -f score.txt
	rm -f snake_score.txt

run:
	./build/brick_game

run_desktop:
	./build/desktop/desktop_app

check:
ifeq ($(OS),Linux)
	valgrind --tool=memcheck --leak-check=yes ./tests/test
	rm -f snake_score.txt
else
	leaks --atExit -- ./tests/test
	rm -f snake_score.txt
endif

clang-format:
	cp ../materials/linters/.clang-format ../src/.clang-format
	clang-format -n gui/desktop/*.cc gui/desktop/*.h gui/cli/*.c gui/cli/*.h brick_game/tetris/*.c brick_game/tetris/*.h brick_game/common/*.h brick_game/snake/*.h brick_game/snake/*.cc tests/*.cc tests/*.h
	rm -f .clang-format