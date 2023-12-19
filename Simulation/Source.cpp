#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include <random>
#include "Map.h"
#include "view.h"
#include <vector>
#include <list>

using namespace sf;
using namespace std;

default_random_engine generator;
uniform_int_distribution<int> ID(1000, 9999);
uniform_int_distribution<int> direction(0, 3);
uniform_int_distribution<int> pol(0, 1);


const int M = 640; // ширина
const int N = 480; // высота

uniform_int_distribution<int> posX(100, M-50);
uniform_int_distribution<int> posY(100, N - 50);

class Animal {
public:
    float x, y, dx, dy, speed, moveTimer=0, dayTimer =0, age = 0, BirthTimer = 0; // координаты x, y; ускорение по x и y; скорость; возраст
    int sex, dir = 0; // пол, направление
    int w = 40, h = 40;
    int id;
    int satiety; // насыщение
    bool dead = false;
    string name;
   
    Animal(int Sex, int needFood, int Age = 0) {
        age = Age, sex = Sex, w = 40, h = 40;//высота, ширина, возраст, пол
        id = ID(generator), satiety = 100;
        x = posX(generator); y = posY(generator);//координата появления спрайта
          
        speed = 0.1;
        //sprite.setOrigin(w / 2, h / 2);
    }

    void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
    {
        for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
            for (int j = x / 32; j < (x + w) / 32; j++)
            {
                if (TileMap[i][j] == '0')//если элемент наш тайлик земли? то
                {
                    if (Dy > 0) { y = i * 32 - h;  dy = 0; }//столкновение с нижним краем карты
                    if (Dy < 0) { y = i * 32 + 32;  dy = 0; }//столкновение с верхними краем карты
                    if (Dx > 0) { x = j * 32 - w; }//с правым краем карты
                    if (Dx < 0) { x = j * 32 + 32; }// с левым краем карты
                }
            }
    }
    
    FloatRect getRect() {
        return FloatRect(x, y, w, h);
    }

};

class Hunter : public Animal{ //класс хищника
public:
    int NeedFood, MaxRepAge, MinRepAge, MaxAge;
    Image image; //сфмл изображение 
    Texture texture;//сфмл текстура
    Sprite sprite;//сфмл спрайт
    
    Hunter(int Sex, int needFood, float Age, int maxRepAge, int minRepAge, int maxAge) : Animal(Sex, needFood, Age) {
        name = "hunt";
        NeedFood = needFood;
        MaxAge = maxAge;
        MaxRepAge = maxRepAge;
        MinRepAge = minRepAge;
        if (Sex == 1)
            image.loadFromFile("Textures/HuntMale.png");
        else
            image.loadFromFile("Textures/HuntFemale.png");
        texture.loadFromImage(image);//закидываем наше изображение в текстуру
        sprite.setTexture(texture);//заливаем спрайт текстурой 
    }

    void Aging(float time) {
        dayTimer += time;
        if (dayTimer >= 7500) {
            age += 1;
            cout << "Хищник " << id << "\t Возраст: " << age << endl;
            if (age >= MaxAge)
            {
                dead = true;
            }
            dayTimer = 0;
        }
    }

    virtual void update(float time) //функция оживления объекта класса
    {
        BirthTimer += time;
        moveTimer += time;
        if (moveTimer >= 500) {
            dir = direction(generator);
            satiety -= 3;
            if (satiety <= 0) {
                dead = true;
            }
            moveTimer = 0;
        }
        if (!dead) {
            switch (dir)
            {
            case 0: dx = speed; dy = 0; break;//по иксу задаем положительную скорость, по игреку зануляем.
            case 1: dx = -speed; dy = 0; break;//по иксу задаем отрицательную скорость, по игреку зануляем.
            case 2: dx = 0; dy = speed; break;//по иксу задаем нулевое значение, по игреку положительное.
            case 3: dx = 0; dy = -speed; break;//по иксу задаем нулевое значение, по игреку отрицательное.
            }

            x += dx * time;
            checkCollisionWithMap(dx, 0);
            y += dy * time;
            checkCollisionWithMap(0, dy);

            Aging(time);

            sprite.setPosition(x, y);
        }
    }
};

class Herbvirous :public Animal{ //класс травоядного
public:
public:
    int NeedFood, MaxRepAge, MinRepAge, MaxAge;
    Image image; //сфмл изображение 
    Texture texture;//сфмл текстура
    Sprite sprite;//сфмл спрайт
    Herbvirous(int Sex, int needFood, float Age, int maxRepAge, int minRepAge, int maxAge) : Animal(Sex, needFood, Age) {
        NeedFood = needFood;
        MaxAge = maxAge;
        MaxRepAge = maxRepAge;
        MinRepAge = minRepAge;
        name = "herb";
        if (Sex == 1)
            image.loadFromFile("Textures/HerbMale.png");
        else
            image.loadFromFile("Textures/HerbFemale.png");
        texture.loadFromImage(image);//закидываем наше изображение в текстуру
        sprite.setTexture(texture);//заливаем спрайт текстурой 
    }
   
    void checkGrass() {
        for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по тайликам, контактирующим с игроком, то есть по всем квадратикам размера 32*32, 
            for (int j = x / 32; j < (x + w) / 32; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается
            {
                if (TileMap[i][j] == 'g') { //если символ равен 'g' (трава)            
                    satiety += 100 / NeedFood;
                    if (satiety > 100)
                        satiety = 100;
                    TileMap[i][j] = ' ';//убираем траву
                }
            }
    }

    void Aging(float time) {
        dayTimer += time;
        if (dayTimer >= 7500) {
          age += 1;
          cout << "Травоядное " << id << "\t Возраст: " << age << endl;
          if (age >= MaxAge)
          {
              dead = true;
          }
          dayTimer = 0;
        }
    }

    virtual void update(float time) //функция оживления объекта класса.
    {
        BirthTimer += time;
        moveTimer += time;
        if (moveTimer >= 500) {
            dir = direction(generator);
            satiety -= 3;
            if (satiety <= 0) {
                dead = true;
            }
            moveTimer = 0;
        }
        if (!dead) {
            switch (dir)
            {
            case 0: dx = speed; dy = 0; break;//по иксу задаем положительную скорость, по игреку зануляем.
            case 1: dx = -speed; dy = 0; break;//по иксу задаем отрицательную скорость, по игреку зануляем. 
            case 2: dx = 0; dy = speed; break;//по иксу задаем нулевое значение, по игреку положительное. 
            case 3: dx = 0; dy = -speed; break;//по иксу задаем нулевое значение, по игреку отрицательное.
            }

            x += dx * time;
            checkCollisionWithMap(dx, 0);
            y += dy * time;
            checkCollisionWithMap(0, dy);

            sprite.setPosition(x, y);
            checkGrass();
            Aging(time);
        }
 
    }
};


int main()
{
    setlocale(LC_ALL, "rus");
    
    cout << "\t\tПрограмма \"Симуляция жизни\"" << endl;
    cout << "\t\t Добро пожаловать!" << endl << endl;
    
    RenderWindow window(VideoMode(M, N), "Simulation");
    
    Image map_image;//объект изображения для карты
    map_image.loadFromFile("Textures/tiles.png");//загружаем файл для карты
    Texture map;//текстура карты
    map.loadFromImage(map_image);//заряжаем текстуру картинкой
    Sprite s_map;//создаём спрайт для карты
    s_map.setTexture(map);//заливаем текстуру спрайтом
  
    list<Hunter*> HuntersList;//создаю список хищников
    list<Hunter*>::iterator it;//итератор чтобы проходить по эл-там списка
    list<Hunter*>::iterator it2;

    list<Herbvirous*> HerbsList;//создаю список травоядных
    list<Herbvirous*>::iterator itt;//итератор чтобы проходить по эл-там списка
    list<Herbvirous*>::iterator itt2;

    /////////////////// Данные о хищниках ///////////////////////////////////////////////////////////////
    cout << "Введите начальное кол-во хищников: ";
    int startAmountHunt;
    cin >> startAmountHunt;
    if (startAmountHunt > 0) {
        cout << "Введите информацию о начальной популяции: " << endl;
        int needFood, maxAge, maxRepAge, minRepAge;
        cout << "Введите необходимое кол-во пищи (сколько нужно травоядных в месяц): ";
        cin >> needFood;
        cout << "Максимальный возраст: ";
        cin >> maxAge;
        cout << "Минимальный репродуктивный возраст: ";
        cin >> minRepAge;
        cout << "Максимальный репродуктивный возраст: ";
        cin >> maxRepAge;
        
        cout << "Введите данные о каждом животном (пол(1-мужской, 0-женский) и возраст)" << endl;
        
        for (int i = 0; i < startAmountHunt; i++) {
            int sex = 0, age = 0;
            cout << "Данные о животном №" << i + 1 << ":" << endl;
            cin >> sex >> age;
            HuntersList.push_back(new Hunter(sex, needFood, age, maxRepAge, minRepAge, maxAge));
        }
    }

    /////////////////// Данные о травоядных ///////////////////////////////////////////////////////////////
    cout << "Введите начальное кол-во травоядных: ";
    int startAmountHerb;
    cin >> startAmountHerb;
    if (startAmountHerb > 0) {
        cout << "Введите информацию о начальной популяции: " << endl;
        int needFood, maxAge, maxRepAge, minRepAge;
        cout << "Введите необходимое кол-во пищи(сколько нужно травы в месяц): ";
        cin >> needFood;
        cout << "Максимальный возраст: ";
        cin >> maxAge;
        cout << "Минимальный репродуктивный возраст: ";
        cin >> minRepAge;
        cout << "Максимальный репродуктивный возраст: ";
        cin >> maxRepAge;

        cout << "Введите данные о каждом животном (пол(1-мужской, 0-женский) и возраст)" << endl;

        for (int i = 0; i < startAmountHerb; i++) {
            int sex = 0, age = 0;
            cout << "Данные о животном №" << i + 1 << ":" << endl;
            cin >> sex >> age;
            HerbsList.push_back(new Herbvirous(sex, needFood, age, maxRepAge, minRepAge, maxAge));
        }
    }

    float GrassSpeed;
    cout << "Введите скорость вырастания травы(травы/сек): ";
    cin >> GrassSpeed;
    view.reset(sf::FloatRect(0, 0, 640, 480));

    
    Clock clock;
    float createObjectForMapTimer = 0;
    float yeartimer = 0;
    float BirthTimer = 0;
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 800;

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        yeartimer += time;
        if (yeartimer >= 7500) {
            cout << "\n Данные о популяциях: \n";
            cout << HuntersList.size() << " Хищников\n" << HerbsList.size() << " Травоядных\n";
            yeartimer = 0;
        }

        for (it = HuntersList.begin(); it != HuntersList.end();)
        {
            Hunter *b = *it;//для удобства, чтобы не писать (*it)->
            b->update(time);//вызываем ф-цию update для всех объектов
            if (b->dead == true) { it = HuntersList.erase(it); delete b; }// если этот объект мертв, то удаляем его
            else it++;
        }

        for (itt = HerbsList.begin(); itt != HerbsList.end();)//говорим что проходимся от начала до конца
        {
            Herbvirous* b = *itt;//для удобства, чтобы не писать (*it)->
            b->update(time);//вызываем ф-цию update для всех объектов
            if (b->dead == true) { itt = HerbsList.erase(itt); delete b; }// если этот объект мертв, то удаляем его
            else itt++;
        }

        for (it = HuntersList.begin(); it != HuntersList.end(); it++)
            for (itt = HerbsList.begin(); itt != HerbsList.end(); itt++) {
                if ((*it)->getRect() != (*itt)->getRect())//при этом это должны быть разные прямоугольники
                    if (((*it)->getRect().intersects((*itt)->getRect())))//если столкнулись два объекта и они враги
                    {
                        (*itt)->dead = true;
                        (*it)->satiety += 100 / (*it)->NeedFood;
                    }
            }
        
        

        for (it = HuntersList.begin(); it != HuntersList.end(); it++)//проходимся по эл-там списка
            for (it2 = HuntersList.begin(); it2 != HuntersList.end(); it2++) {
                if ((*it)->getRect() != (*it2)->getRect())//при этом это должны быть разные прямоугольники
                    if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->sex != ((*it2)->sex)) && ((*it)->age >= (*it)->MinRepAge) && ((*it)->age < (*it)->MaxRepAge) && ((*it2)->age >= (*it2)->MinRepAge) && ((*it2)->age < (*it2)->MaxRepAge) && ((*it2)->BirthTimer >= 15000) && ((*it)->BirthTimer >= 15000))//если столкнулись два хищника разного пола
                    {
                        (*it2)->BirthTimer = 0;
                        (*it)->BirthTimer = 0;
                        HuntersList.push_back(new Hunter(pol(generator), (*it)->NeedFood, 0, (*it)->MaxRepAge, (*it)->MinRepAge, (*it)->MaxAge));
                    }
            }

        for (itt = HerbsList.begin(); itt != HerbsList.end(); itt++)//проходимся по эл-там списка
            for (itt2 = HerbsList.begin(); itt2 != HerbsList.end(); itt2++) {
                if ((*itt)->getRect() != (*itt2)->getRect())//при этом это должны быть разные прямоугольники
                    if (((*itt)->getRect().intersects((*itt2)->getRect())) && ((*itt)->sex != ((*itt2)->sex)) && ((*itt)->age >= (*itt)->MinRepAge) && ((*itt)->age < (*itt)->MaxRepAge) && ((*itt2)->age >= (*itt2)->MinRepAge) && ((*itt2)->age < (*itt2)->MaxRepAge) && ((*itt2)->BirthTimer >= 15000) && ((*itt)->BirthTimer >= 15000))//если столкнулись два хищника разного пола
                    {
                        (*itt2)->BirthTimer = 0;
                        (*itt)->BirthTimer = 0;
                        HerbsList.push_back(new Herbvirous(pol(generator), (*itt)->NeedFood, 0, (*itt)->MaxRepAge, (*itt)->MinRepAge, (*itt)->MaxAge));
                    }
            }

        
        

        viewmap(time); //функция скроллинга карты, передаем ей время sfml
        changeview();
        window.setView(view);
        
        createObjectForMapTimer += time;//наращиваем таймер
        if (createObjectForMapTimer > 1 / GrassSpeed * 1000) {
            randomMapGenerate();//генерация случ травы
            createObjectForMapTimer = 0;//обнуляем таймер
        }

        window.clear(Color(250, 220, 100, 0));

        /////////////////////////////Рисуем карту/////////////////////
        for (int i = 0; i < HEIGHT_MAP; i++)
            for (int j = 0; j < WIDTH_MAP; j++)
            {
                if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32));
                if (TileMap[i][j] == '0')  s_map.setTextureRect(IntRect(32, 0, 32, 32));
                if ((TileMap[i][j] == 'g')) s_map.setTextureRect(IntRect(64, 0, 32, 32));
                s_map.setPosition(j * 32, i * 32);

                window.draw(s_map);
            } 

        for (it = HuntersList.begin(); it != HuntersList.end(); it++) {
            window.draw((*it)->sprite);
        }

        for (itt = HerbsList.begin(); itt != HerbsList.end(); itt++) {
            window.draw((*itt)->sprite);
        }

        window.display();
    }

    return 0;
}
