default:
	g++ -o ai src/*.cpp -Wall -Wextra

performance:
	g++ -o ai src/*.cpp -O3 -DNDEBUG

debug:
	g++ -g -o ai src/*.cpp -Wall -Wextra
