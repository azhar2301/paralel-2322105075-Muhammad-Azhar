CXX = g++
CXXFLAGS = -O2 -std=c++17

all: serial.exe pthreads.exe openmp.exe compare.exe

serial.exe: src/main_serial.cpp
	$(CXX) $(CXXFLAGS) src/main_serial.cpp -o serial.exe

pthreads.exe: src/main_pthreads.cpp
	$(CXX) $(CXXFLAGS) -pthread src/main_pthreads.cpp -o pthreads.exe

openmp.exe: src/main_openmp.cpp
	$(CXX) $(CXXFLAGS) -fopenmp src/main_openmp.cpp -o openmp.exe

compare.exe: src/compare.cpp
	$(CXX) $(CXXFLAGS) src/compare.cpp -o compare.exe

clean:
	rm -f serial.exe pthreads.exe openmp.exe compare.exe


run: all
	./serial.exe
	./pthreads.exe 1
	./pthreads.exe 2
	./pthreads.exe 4
	./pthreads.exe 8
	./openmp.exe 1
	./openmp.exe 2
	./openmp.exe 4
	./openmp.exe 8
	.\compare.exe output/sobel_serial.pgm output/sobel_pthreads_1.pgm
	.\compare.exe output/sobel_serial.pgm output/sobel_pthreads_2.pgm
	.\compare.exe output/sobel_serial.pgm output/sobel_pthreads_4.pgm
	.\compare.exe output/sobel_serial.pgm output/sobel_pthreads_8.pgm
	.\compare.exe output/sobel_serial.pgm output/sobel_openmp_1.pgm
	.\compare.exe output/sobel_serial.pgm output/sobel_openmp_2.pgm
	.\compare.exe output/sobel_serial.pgm output/sobel_openmp_4.pgm
	.\compare.exe output/sobel_serial.pgm output/sobel_openmp_8.pgm