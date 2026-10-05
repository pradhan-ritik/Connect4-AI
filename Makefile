default:
	g++ -o ai src/*.cpp -Wall -Wextra

performance:
	g++ -o ai src/*.cpp -O3

debug:
	g++ -g -o ai src/*.cpp -Wall -Wextra
