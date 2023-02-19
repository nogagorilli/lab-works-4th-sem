//========== Файл shape.cpp (прикладная программа) ==========
// Пополнение и использование библиотеки фигур
#include "pch.h"	//связь с ОС (пример для Visual C++2017)
#include "screen.h"
#include "shape.h"
// ПРИМЕР ДОБАВКИ: дополнительный фрагмент - полуокружность
class h_circle: public RectangularShape {
  bool reflected;
public:
	h_circle(point a, point b, bool r=true) : RectangularShape(a, b), reflected(r) { }
	void draw();
	void flip_horisontally( ) override { }; // Отразить горизонтально (пустая функция)
	void flip_vertically( ) override { reflected = !reflected; };	// Отразить вертикально
	void rotate_left() override {};
	void rotate_right() override{};
	void move(int x,int y) override;
	void resize(int dimension) override;
};
void h_circle::resize(int dimension){
    try{
        RectangularShape::resize(dimension);
    }catch(CantResize&){}
}

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
void h_circle::move(int x,int y){
    try{
        RectangularShape::move(x,y);
    }catch(CantMove&){}
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
class myshape : public RectangularShape { // Моя фигура ЯВЛЯЕТСЯ
     int w, h;   //        прямоугольником
     line l_eye; // левый глаз – моя фигура СОДЕРЖИТ линию
     line r_eye; // правый глаз
     line mouth; // рот
     rectangle face;
  public:
     myshape(point, point);
     void draw( );
     void move(int, int);
     void resize(int dimension){}
     void flip_horisontally() override{};
     void flip_vertically() override{};
};


myshape :: myshape(point a, point b)
	: RectangularShape(a, b),	//Инициализация базового класса
	  w(neast( ).x - swest( ).x + 1), // Инициализация данных
	  h(neast( ).y - swest( ).y + 1), // - строго в порядке объявления!
	  l_eye(point(swest( ).x + 2, swest( ).y + h * 3 / 4), 2),
	  r_eye(point(swest( ).x + w - 4, swest( ).y + h * 3 / 4), 2),
	  mouth(point(swest( ).x + 2, swest( ).y + h / 4), w - 4),
	  face(swest(),neast())
{ }
void myshape :: draw( )
{
	 face.draw( ); //Контур лица (глаза и нос рисуются сами!)
	 int a = (swest( ).x + neast( ).x) / 2;
	 int b = (swest( ).y + neast( ).y) / 2;
	 put_point(point(a, b)); // Нос – существует только на рисунке!
}


void myshape :: move(int a, int b)
{
    try{
         RectangularShape :: move(a, b);
         face.move(a,b);
         l_eye.move(a, b);
         r_eye.move(a, b);
         mouth.move(a, b);
    } catch(CantMove&){}
}


class InclinedCross : virtual public RectangularShape
{
public:
    InclinedCross(point swest, point neast);
    void flip_horisontally() override{};
    void flip_vertically() override{};
    void rotate_right() override;
    void rotate_left() override;
    void draw() override;
    void move(int x,int y) override;
    void resize(int dimension);
};
void InclinedCross::resize(int dimension){
    try{
        RectangularShape::resize(dimension);
    }catch(CantResize&){}
}


InclinedCross::InclinedCross(point swest, point neast) : RectangularShape(swest,neast){}


void InclinedCross::draw(){
    put_line(neast().x,neast().y,swest().x,swest().y);
    put_line(seast().x,seast().y,nwest().x,nwest().y);
}
void InclinedCross::rotate_left(){
    try{
        RectangularShape::rotate_left();
    }catch(CantRotate&){}
}

void InclinedCross::rotate_right(){
    try{
        RectangularShape::rotate_right();
    }catch(CantRotate&){}
}

void InclinedCross::move(int x,int y){
    try{
        RectangularShape::move(x,y);
    }catch(CantMove&){}
}


class EvenTriangle : virtual public RectangularShape
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
    float xCenter = center().x;
    float yCenter = center().y;
    try{
        RectangularShape::rotate_left();
        trApex = get_point_rotated_left(trApex,xCenter,yCenter);
        trBotLeft = get_point_rotated_left(trBotLeft,xCenter,yCenter);
        trBotRight = get_point_rotated_left(trBotRight,xCenter,yCenter);
    } catch(CantRotate&){}
}

void EvenTriangle::rotate_right(){
    float xCenter = center().x;
    float yCenter = center().y;
    try{
        RectangularShape::rotate_right();
        trApex = get_point_rotated_right(trApex,xCenter,yCenter);
        trBotLeft = get_point_rotated_right(trBotLeft,xCenter,yCenter);
        trBotRight = get_point_rotated_right(trBotRight,xCenter,yCenter);
    }catch(CantRotate&) {}
}

void EvenTriangle::resize(int dimension){
    try{
    RectangularShape::resize(dimension);
    trApex.x += (trApex.x - sw.x) * (dimension - 1);
    trApex.y += (trApex.y - sw.y) * (dimension - 1);
    trBotLeft.x += (trBotLeft.x - sw.x) * (dimension - 1);
    trBotLeft.y += (trBotLeft.y - sw.y) * (dimension - 1);
    trBotRight.x += (trBotRight.x - sw.x) * (dimension - 1);
    trBotRight.y += (trBotRight.y - sw.y) * (dimension - 1);
    }catch(CantResize&){}
}

void EvenTriangle::move(int x,int y){
    try{
        RectangularShape::move(x,y);
        trApex.x+=x;
        trApex.y+=y;
        trBotLeft.x+=x;
        trBotLeft.y+=y;
        trBotRight.x+=x;
        trBotRight.y+=y;
    }catch(CantMove&){}
}

EvenTriangle::EvenTriangle(point sWest, point nEast): RectangularShape(sWest,nEast){
    trBotLeft = swest();
    trBotRight = seast();
    trApex = north();
}


class EvenTriangleWithInclinedCross : virtual public RectangularShape
{
private:
    EvenTriangle tr;
    InclinedCross cr;
public:
    void draw() override;
    EvenTriangleWithInclinedCross(point, point);
    void flip_horisontally()override;
    void flip_vertically()override;
    void move(int x,int y) override;
    void resize(int dimension) override;
    void rotate_right() override;
    void rotate_left() override;


};

void EvenTriangleWithInclinedCross::draw(){
    tr.draw();
    cr.draw();
}

EvenTriangleWithInclinedCross::EvenTriangleWithInclinedCross(point sWest, point nEast):RectangularShape(sWest,nEast),tr(sWest,nEast),cr(sWest,nEast){}

void EvenTriangleWithInclinedCross::flip_horisontally(){
    tr.flip_horisontally();
    cr.flip_horisontally();
}

void EvenTriangleWithInclinedCross::flip_vertically(){
    tr.flip_vertically();
    cr.flip_vertically();
}
void EvenTriangleWithInclinedCross::move(int x,int y){
    try{
        RectangularShape::move(x,y);
        tr.move(x,y);
        cr.move(x,y);
    }catch(CantMove&){}
}

void EvenTriangleWithInclinedCross::resize(int dimension){
    try{
        RectangularShape::resize(dimension);
        tr.resize(dimension);
        cr.resize(dimension);
    }catch(CantResize&){}
}

void EvenTriangleWithInclinedCross::rotate_left(){
    try{
        RectangularShape::rotate_left();
        tr.rotate_left();
        cr.rotate_left();
    }catch(CantRotate&){}
}
void EvenTriangleWithInclinedCross::rotate_right(){
    try{
        RectangularShape::rotate_right();
        tr.rotate_right();
        cr.rotate_right();
    }catch(CantRotate&){}
}




int main( )
{   setlocale(LC_ALL, "Rus");
	screen_init( );
//== 1.Объявление набора фигур ==
	rectangle hat(point(0, 0), point(10, 12));
	line brim(point(0,15),17);
	myshape face(point(15,10), point(27,18));
	EvenTriangleWithInclinedCross beard(point(39,10), point(50,18));
	EvenTriangleWithInclinedCross leftHorn(point(39-30,10+10), point(50-30,20+5));
	EvenTriangleWithInclinedCross rightHorn(point(39-22,10+20), point(50-22,20+15));
	shape_refresh( );
	std::cout << "=== Generated... ===\n";
	std::cin.get(); //Смотреть исходный набор
//== 2.Подготовка к сборке ==
    face.move(20,5);
    hat.move(20,5);
	hat.rotate_right( );
	brim.resize(2);
	face.resize(2);

	beard.flip_vertically();
	leftHorn.rotate_left();
	rightHorn.rotate_right();
	leftHorn.resize(2);
	rightHorn.resize(3);

    shape_refresh( );
	std::cout << "=== Prepared... ===\n";
	std::cin.get(); //Смотреть результат поворотов/отражений
//== 3.Сборка изображения ==
	//face.move(0, -10); // Лицо - в исходное положение
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

