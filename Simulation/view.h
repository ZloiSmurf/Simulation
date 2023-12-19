#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;

void viewmap(float time) { //функция для перемещения камеры по карте. принимает время sfml


	if (Keyboard::isKeyPressed(Keyboard::D)) {
		view.move(0.1 * time, 0);//скроллим карту вправо
	}

	if (Keyboard::isKeyPressed(Keyboard::S)) {
		view.move(0, 0.1 * time);//скроллим карту вниз
	}

	if (Keyboard::isKeyPressed(Keyboard::A)) {
		view.move(-0.1 * time, 0);//скроллим карту влево
	}
	if (Keyboard::isKeyPressed(Keyboard::W)) {
		view.move(0, -0.1 * time);//скроллим карту вправо
	}
}

void changeview() {


	if (Keyboard::isKeyPressed(Keyboard::Q)) {
		view.zoom(1.0100f); //масштабируем, приблежение
	}

	if (Keyboard::isKeyPressed(Keyboard::E)) {
		view.zoom(0.900f); //масштабируем, отдаление
	}

	if (Keyboard::isKeyPressed(Keyboard::I)) {
		view.setSize(640, 480);//устанавливает исходный размер камеры
	}
}