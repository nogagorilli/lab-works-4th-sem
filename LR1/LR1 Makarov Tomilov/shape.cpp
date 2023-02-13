//========== Файл shape.cpp (прикладная программа) ==========
// Пополнение и использование библиотеки фигур
#include "pch.h"	//связь с ОС (пример для Visual C++2017)
#include "screen.h"
#include "shape.h"
// ПРИМЕР ДОБАВКИ: дополнительный фрагмент - полуокружность
class h_circle: public rectangle, public reflectable {
  bool reflected;
public:
	h_circle(point a, point b, bool r=true) : rectangle(a, b), reflected(r) { }
	void draw();
	void flip_horisontally( ) { }; // Отразить горизонтально (пустая функция)
	void flip_vertically( ) { reflected = !reflected; };	// Отразить вертикально
};


void h_circle :: draw() //Алгоритм Брезенхэма для окружностей
{   //(выдаются два сектора, указываемые значением reflected)
    int x0 = (sw.x + ne.x)/2, y0 = reflected ? sw.y : ne.y;
	int radius = (ne.x - sw.x)/2;
	int x = 0, y = radius, delta = 2 - 2 * radius, error = 0;
    while(y >= 0) { // Цикл рисования
	    if(reflected) { put_point(x0 + x, y0 + y*0.7); put_point(x0 - x, y0 + y*0.7); }
	    else { put_point(x0 + x, y0 - y*0.7); put_point(x0 - x, y0 - y*0.7); }
       error = 2 * (delta + y) - 1;
       if(delta < 0 && error <= 0) { ++x; delta += 2 * x + 1; continue; }
       error = 2 * (delta - x) - 1;
       if(delta > 0 && error > 0) { --y; delta += 1 - 2 * y; continue; }
       ++x; delta += 2 * (x - y);  --y;
	   }
}


// ПРИМЕР ДОБАВКИ: дополнительная функция присоединения…
void down(shape &p,  const shape &q)
{    point n = q.south( );
     point s = p.north( );
     p.move(n.x - s.x, n.y - s.y - 1);
}

void upRight(shape &p,  const shape &q){
    int deltaX = q.neast().x - p.swest().x;
    int deltaY = q.neast().y - p.swest().y;
    p.move(deltaX,deltaY);
}
void upLeft(shape &p,  const shape &q){
    int deltaX = q.nwest().x - p.seast().x;
    int deltaY = q.nwest().y - p.seast().y;
    p.move(deltaX,deltaY);
}

// Cборная пользовательская фигура - физиономия
class myshape : public rectangle { // Моя фигура ЯВЛЯЕТСЯ
     int w, h;			             //        прямоугольником
     line l_eye; // левый глаз – моя фигура СОДЕРЖИТ линию
     line r_eye; // правый глаз
     line mouth; // рот
  public:
     myshape(point, point);
     void draw( );
     void move(int, int);
     void resize(int) { }
};

myshape :: myshape(point a, point b)
	: rectangle(a, b),	//Инициализация базового класса
	  w(neast( ).x - swest( ).x + 1), // Инициализация данных
	  h(neast( ).y - swest( ).y + 1), // - строго в порядке объявления!
	  l_eye(point(swest( ).x + 2, swest( ).y + h * 3 / 4), 2),
	  r_eye(point(swest( ).x + w - 4, swest( ).y + h * 3 / 4), 2),
	  mouth(point(swest( ).x + 2, swest( ).y + h / 4), w - 4)
{ }
void myshape :: draw( )
{
	 rectangle :: draw( ); //Контур лица (глаза и нос рисуются сами!)
	 int a = (swest( ).x + neast( ).x) / 2;
	 int b = (swest( ).y + neast( ).y) / 2;
	 put_point(point(a, b)); // Нос – существует только на рисунке!
}


void myshape :: move(int a, int b)
{
	 rectangle :: move(a, b);
	 l_eye.move(a, b);  r_eye.move(a, b);
	 mouth.move(a, b);
}


class InclinedCross : public rectangle
{
public:
    InclinedCross(point swest, point neast);
    void draw() override;
};



InclinedCross::InclinedCross(point swest, point neast) : rectangle(swest,neast){}


void InclinedCross::draw(){
    put_line(neast().x,neast().y,swest().x,swest().y);
    put_line(seast().x,seast().y,nwest().x,nwest().y);
}

class EvenTriangle : public rectangle, virtual public reflectable
{
protected:
    point trBotLeft;
    point trBotRight;
    point trApex;
public:
    void draw() override;
    void flip_horisontally() override;
    void flip_vertically() override;
    void rotate_left() override;
    void rotate_right() override;
    void resize(int) override;
    void move(int,int) override;
    EvenTriangle(point, point);
};



void EvenTriangle::draw(){
    put_line(trBotLeft.x,trBotLeft.y,trBotRight.x,trBotRight.y);
    put_line(trApex.x,trApex.y,trBotRight.x,trBotRight.y);
    put_line(trBotLeft.x,trBotLeft.y,trApex.x,trApex.y);
}

void EvenTriangle::flip_horisontally(){
    float xCenter = ((float)(west().x + east().x))/2;
    trBotLeft.x = std::round(xCenter - (((float)trBotLeft.x)-xCenter));
    trBotRight.x = std::round(xCenter - (((float)trBotRight.x)-xCenter));
    trApex.x = std::round(xCenter - (((float)trApex.x)-xCenter));
}

void EvenTriangle::flip_vertically(){
    float yCenter = ((float)(south().y + north().y))/2;
    trBotLeft.y = std::round(yCenter - (((float)trBotLeft.y)-yCenter));
    trBotRight.y = std::round(yCenter - (((float)trBotRight.y)-yCenter));
    trApex.y = std::round(yCenter - (((float)trApex.y)-yCenter));
}

void EvenTriangle::rotate_left(){
    float xCenter = swest().x;
    float yCenter = swest().y;

    trApex = get_point_rotated_left(trApex,xCenter,yCenter);
    trBotLeft = get_point_rotated_left(trBotLeft,xCenter,yCenter);
    trBotRight = get_point_rotated_left(trBotRight,xCenter,yCenter);
    rectangle::rotate_left();
}

void EvenTriangle::rotate_right(){
    float xCenter = seast().x;
    float yCenter = seast().y;
    trApex = get_point_rotated_right(trApex,xCenter,yCenter);
    trBotLeft = get_point_rotated_right(trBotLeft,xCenter,yCenter);
    trBotRight = get_point_rotated_right(trBotRight,xCenter,yCenter);
    rectangle::rotate_right();
}

void EvenTriangle::resize(int dimension){
    rectangle::resize(dimension);
    trApex.x += (trApex.x - sw.x) * (dimension - 1);
    trApex.y += (trApex.y - sw.y) * (dimension - 1);
    trBotLeft.x += (trBotLeft.x - sw.x) * (dimension - 1);
    trBotLeft.y += (trBotLeft.y - sw.y) * (dimension - 1);
    trBotRight.x += (trBotRight.x - sw.x) * (dimension - 1);
    trBotRight.y += (trBotRight.y - sw.y) * (dimension - 1);
}

void EvenTriangle::move(int x,int y){
    rectangle::move(x,y);
    trApex.x+=x;
    trApex.y+=y;
    trBotLeft.x+=x;
    trBotLeft.y+=y;
    trBotRight.x+=x;
    trBotRight.y+=y;
}

EvenTriangle::EvenTriangle(point sWest, point nEast): rectangle(sWest,nEast){
    trBotLeft = swest();
    trBotRight = seast();
    trApex = north();
}


class EvenTriangleWithInclinedCross : public EvenTriangle
{
public:
    void draw() override;
    EvenTriangleWithInclinedCross(point, point);
};

void EvenTriangleWithInclinedCross::draw(){
    EvenTriangle::draw();
    put_line(neast().x,neast().y,swest().x,swest().y);
    put_line(seast().x,seast().y,nwest().x,nwest().y);
}
EvenTriangleWithInclinedCross::EvenTriangleWithInclinedCross(point sWest, point nEast): EvenTriangle(sWest,nEast){}





int main( )
{   setlocale(LC_ALL, "Rus");
	screen_init( );
//== 1.Объявление набора фигур ==
	rectangle hat(point(0, 0), point(10, 12));
	line brim(point(0,15),17);
	myshape face(point(15,10), point(27,18));
	EvenTriangleWithInclinedCross beard(point(39,10), point(50,18));
	EvenTriangleWithInclinedCross leftHorn(point(39-30,10+10), point(50-30,20+5));
	EvenTriangleWithInclinedCross rightHorn(point(39-22,10+20), point(50-20,20+15));
	shape_refresh( );
	std::cout << "=== Generated... ===\n";
	std::cin.get(); //Смотреть исходный набор
//== 2.Подготовка к сборке ==
	hat.rotate_right( );
	brim.resize(2);
	face.resize(2);
	leftHorn.resize(2);
	beard.flip_vertically();
	leftHorn.rotate_left();
	rightHorn.rotate_right();

    shape_refresh( );
	std::cout << "=== Prepared... ===\n";
	std::cin.get(); //Смотреть результат поворотов/отражений
//== 3.Сборка изображения ==
//	face.move(0, -10); // Лицо - в исходное положение
	up(brim, face);
	up(hat, brim);
	down(beard, face);
	upRight(rightHorn,hat);
	upLeft(leftHorn,hat);
	shape_refresh( );
	std::cout << "=== Ready! ===\n";
	std::cin.get(); //Смотреть результат
	screen_destroy( );
	return 0;
}

