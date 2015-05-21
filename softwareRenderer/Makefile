final: dependency1 dependency2 dependency3 dependency4 dependency5
	g++ ./build/deprecated.o ./build/helpers.o ./build/model.o ./build/tgaimage.o ./build/main.o -o renderer
dependency1: prepare
	g++ -c model.cpp -o ./build/model.o
dependency2: prepare
	g++ -c tgaimage.cpp -o ./build/tgaimage.o
dependency3: prepare	
	g++ -c deprecated.cpp -o ./build/deprecated.o
dependency4: prepare
	g++ -c main.cpp -o ./build/main.o
dependency5: prepare
	g++ -c helpers.cpp -o ./build/helpers.o
prepare:
	rm -rf build
	mkdir build