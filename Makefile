all: clean build

build:
	gcc -O3 -o pi_2accel_berryIMU.o -l rt pi_2accel_berryIMU.c -lm

clean-all: clean clean-data

clean:
	-rm pi_2accel_berryIMU.o

clean-data:
	-rm *.csv

configure:
	curl http://ozzmaker.com/i2c/
