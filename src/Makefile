all: clean calc

calc: 
	gcc view.c controller.c calc.c -o calc `pkg-config --cflags --libs gtk+-3.0`
	./calc

clean: 
	rm -rf ./calc