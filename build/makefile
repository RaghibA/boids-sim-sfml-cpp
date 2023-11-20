all: link compile compile-exe exec

link:
	g++ -std=c++17 -c ../src/main.cpp ../src/Boid.cpp -I/opt/homebrew/Cellar/sfml/2.6.1/include  -I../headers

compile:
	g++ main.o Boid.o -o sim -L/opt/homebrew/Cellar/sfml/2.6.1/lib -lsfml-graphics -lsfml-window -lsfml-system

compile-exe:
	g++ main.o Boid.o -o sim.exe -L/opt/homebrew/Cellar/sfml/2.6.1/lib -lsfml-graphics -lsfml-window -lsfml-system

exec:
	./sim
