#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;

void viewmap(float time) { //������� ��� ����������� ������ �� �����. ��������� ����� sfml


	if (Keyboard::isKeyPressed(Keyboard::D)) {
		view.move(0.1 * time, 0);//�������� ����� ������
	}

	if (Keyboard::isKeyPressed(Keyboard::S)) {
		view.move(0, 0.1 * time);//�������� ����� ����
	}

	if (Keyboard::isKeyPressed(Keyboard::A)) {
		view.move(-0.1 * time, 0);//�������� ����� �����
	}
	if (Keyboard::isKeyPressed(Keyboard::W)) {
		view.move(0, -0.1 * time);//�������� ����� ������
	}
}

void changeview() {


	if (Keyboard::isKeyPressed(Keyboard::Q)) {
		view.zoom(1.0100f); //������������, �����������
	}

	if (Keyboard::isKeyPressed(Keyboard::E)) {
		view.zoom(0.900f); //������������, ���������
	}

	if (Keyboard::isKeyPressed(Keyboard::I)) {
		view.setSize(640, 480);//������������� �������� ������ ������
	}
}