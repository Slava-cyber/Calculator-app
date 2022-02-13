all: clean calc

debug:
	gcc calc.c `pkg-config --cflags --libs gtk+-3.0` -o ./a.out

calc: 
	gcc view.c controller.c calc.c `pkg-config --cflags --libs gtk+-3.0` -o calc
	./calc

test: 
	gcc test.c calc.c -g `pkg-config --cflags --libs gtk+-3.0` -o Test -lcheck
	./Test

# s21_test.o: test.c calc.c
# 	gcc test.c calc.c -lcheck -o test.o `pkg-config --cflags --libs gtk+-3.0`

clean: 
	rm -rf ./calc
	rm -f *.o Test *.out *.gcov *.gcda *.gcno *.info *.a
	rm -rf ./report