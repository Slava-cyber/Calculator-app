all: clean calc

debug:
	gcc calc.c `pkg-config --cflags --libs gtk+-3.0` -o ./a.out
#	CK_FORK=no leaks --atExit -- ./a.out
	./a.out

calc: 
	gcc view.c controller.c calc.c `pkg-config --cflags --libs gtk+-3.0` -o calc
	./calc

test: 
	gcc test.c calc.c -g `pkg-config --cflags --libs gtk+-3.0` -o Test -lcheck
#	CK_FORK=no leaks --atExit -- ./Test
	./Test

install: 
	gcc view.c controller.c calc.c `pkg-config --cflags --libs gtk+-3.0` -o calc
	mkdir -p app
	install ./calc app

uninstall:
	rm -rf app

dvi:
	open documentation.html

dist: install clean
	tar -czf SmartCalc.tgz ./*

gcov_report: s21_calc_gcov.o s21_test_gcov.o
	gcc -fprofile-arcs -ftest-coverage -lcheck s21_test_gcov.o s21_calc_gcov.o -o Test
	./Test
	gcov s21_calc_gcov
	gcov s21_test_gcov
	lcov -t "s21_calc" -o s21_calc.info -c -d .
	genhtml -o report s21_calc.info

s21_test_gcov.o: test.c
	gcc -fprofile-arcs -ftest-coverage -c test.c `pkg-config --cflags --libs gtk+-3.0` -o s21_test_gcov.o 

s21_calc_gcov.o: calc.c
	gcc -c -fprofile-arcs -ftest-coverage calc.c `pkg-config --cflags --libs gtk+-3.0` -o s21_calc_gcov.o 

clean: 
	rm -rf ./calc
	rm -f *.o Test *.out *.gcov *.gcda *.gcno *.info *.a
	rm -rf ./report
	rm -rf ./Test.dSYM

check:
	cp ../materials/linters/CPPLINT.cfg CPPLINT.cfg
	python3 ../materials/linters/cpplint.py --extension=c *.c *.h
	CK_FORK=no leaks --atExit -- ./Test
