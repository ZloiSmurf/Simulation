#include <SFML\Graphics.hpp>

const int HEIGHT_MAP = 25;//размер карты высота
const int WIDTH_MAP = 40;//размер карты ширина 
sf::String TileMap[HEIGHT_MAP] = {
"0000000000000000000000000000000000000000",
"0                                      0",
"0                                      0",
"0                                      0",
"0                                      0",
"0                                      0",
"0                                      0",
"0                                      0",
"0                                      0",
"0                                      0",
"0                                      0",
"0                                      0",
"0                                      0",
"0                                      0",
"0                                      0",
"0                                      0",
"0                                      0",
"0                                      0",
"0                                      0",
"0                                      0",
"0                                      0",
"0                                      0",
"0                                      0",
"0                                      0",
"0000000000000000000000000000000000000000",
};

void randomMapGenerate() {//рандомно расставляем траву

	int randomElementX = 0;//случайный элемент по горизонтали
	int randomElementY = 0;//случ эл-т по вертикали
	srand(time(0));//рандом	
	int countGrass = 1;//количество 

	while (countGrass > 0) {
		randomElementX = 1 + rand() % (WIDTH_MAP - 1);//рандомное по иксу от 1 до ширина карты-1, чтобы не получать числа бордюра карты
		randomElementY = 1 + rand() % (HEIGHT_MAP - 1);//по игреку так же

		if (TileMap[randomElementY][randomElementX] == ' ') {//если встретили символ пробел, 
			TileMap[randomElementY][randomElementX] = 'g'; 
			countGrass--;
		}
	}
}