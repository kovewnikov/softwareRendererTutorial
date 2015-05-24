final: dependency1 dependency2 dependency3 dependency4 dependency5 dependency6
	g++ ./build/geometry.o ./build/model.o ./build/helpers.o ./build/deprecated.o ./build/tgaimage.o ./build/main.o -o renderer
dependency1: prepare
	g++ -c geometry.cpp -o ./build/geometry.o
dependency2: prepare
	g++ -c model.cpp -o ./build/model.o
dependency3: prepare
	g++ -c helpers.cpp -o ./build/helpers.o
dependency4: prepare
	g++ -c tgaimage.cpp -o ./build/tgaimage.o
dependency5: prepare	
	g++ -c deprecated.cpp -o ./build/deprecated.o
dependency6: prepare
	g++ -c main.cpp -o ./build/main.o


prepare:
	rm -rf build
	mkdir build