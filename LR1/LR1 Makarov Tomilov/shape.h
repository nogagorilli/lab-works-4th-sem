//======== ���� shape.h � ���������� ����� =========
#pragma once
#include <list>

//==1. ��������� ������ � ����� ������� �������� ==
char screen[YMAX] [XMAX];
enum color { black = '*', white = '.' };
void screen_init( )
{
  for (auto y = 0; y < YMAX; ++y)
    for (auto &x : screen[y])  x = white;
}


void screen_destroy( )
{
  for (auto y = 0; y < YMAX; ++y)
    for (auto &x : screen[y])  x = black;
}


bool on_screen(int a, int b) // �������� ��������� ����� �� �����
{ return 0 <= a && a < XMAX && 0 <= b && b < YMAX; }


void put_point(int a, int b)
{ if (on_screen(a,b)) screen[b] [a] = black; }


point get_point_rotated_right(point p, float xCenter,float yCenter){
    float x,y,deltaX,deltaY;
    deltaY = (((float)p.y)-yCenter);
    deltaX = (((float)p.x)-xCenter);
    x = p.x;
    y = p.y;
    x = x-deltaX;
    y = y-deltaY;
    x = x+deltaY;
    y = y - deltaX;

    return point(round(x),round(y));
}

point get_point_rotated_left(point p, float xCenter,float yCenter){
    float x,y,deltaX,deltaY;
    deltaY = (((float)p.y)-yCenter);
    deltaX = (((float)p.x)-xCenter);
    x = p.x;
    y = p.y;
    x = x-deltaX;
    y = y-deltaY;
    x = x-deltaY;
    y = y + deltaX;

    return point(round(x),round(y));
}

void put_line(int x0, int y0, int x1, int y1)
/* �������� ���������� ��� ������:
��������� ������� ������ �� (x0,y0) �� (x1,y1).
��������� ������: b(x-x0) + a(y-y0) = 0.
�������������� �������� abs(eps), ��� eps = 2*(b(x-x0)) + a(y-y0).  */
{
  int dx = 1;
  int a = x1 - x0;   if (a < 0) dx = -1, a = -a;
  int dy = 1;
  int b = y1 - y0;   if (b < 0) dy = -1, b = -b;
  int two_a = 2*a;
  int two_b = 2*b;
  int xcrit = -b + two_a;
  int eps = 0;
  for (;;) { //������������ ������ ����� �� ������
	  put_point(x0, y0);
	  if (x0 == x1 && y0 == y1) break;
	  if (eps <= xcrit) x0 += dx, eps += two_b;
	  if (eps >= a || a < b) y0 += dy, eps -= two_a;
  }
}


void screen_clear( ) { screen_init( ); } //������� ������


void screen_refresh( ) // ���������� ������
{
  for (int y = YMAX-1; 0 <= y; --y) { // � ������� ������ �� ������
    for (auto x : screen[y])    // �� ������ ������� �� �������
      std::cout << x;
    std::cout << '\n';
  }
}


//== 2. ���������� ����� ==
struct shape { // ����������� ������� ����� "������"
  static std::list<shape*> shapes;// ������ ����� (���� �� ��� ������!)
  shape( ) { shapes.push_back(this); } //������ �������������� � ������
  virtual point north( ) const = 0;	//����� ��� ��������
  virtual point south( ) const = 0;
  virtual point east( ) const = 0;
  virtual point west( ) const = 0;
  virtual point neast( ) const = 0;
  virtual point seast( ) const = 0;
  virtual point nwest( ) const = 0;
  virtual point swest( ) const = 0;
  virtual void draw( ) = 0;		//���������
  virtual void move(int, int) = 0;	//�����������
  virtual void resize(int) = 0;    	//��������� �������
};


std::list<shape*> shape::shapes;   // ���������� ������ �����


void shape_refresh( ) // ����������� ���� ����� �� ������
{
  screen_clear( );
  for (auto p : shape :: shapes) p->draw( ); //������������ ����������!!!
  screen_refresh( );
}


class rotatable : virtual public shape { //������, ��������� � ��������
public:
	virtual void rotate_left( ) = 0;	//��������� �����
	virtual void rotate_right( ) = 0;	//��������� ������
};


class reflectable : virtual public shape { // ������, ���������
public:					     // � ����������� ���������
	virtual void flip_horisontally( ) = 0;	// �������� �������������
	virtual void flip_vertically( ) = 0;	          // �������� �����������
};


class line : public shape {
/* ������� ������ ["w", "e"].
   north( ) ���������� ����� "���� ������ ������� � ��� ������
   �� �����, ��� ����� ��� �������� �����", � �. �. */
protected:
	point w, e;
public:
  line(point a, point b) : w(a), e(b) { };
  line(point a, int L) : w(point(a.x + L - 1, a.y)), e(a) {  };
  point north( ) const { return point((w.x+e.x)/2, e.y<w.y? w.y : e.y); }
  point south( ) const { return point((w.x+e.x)/2, e.y<w.y? e.y : w.y); }
  point east( ) const { return point(e.x<w.x? w.x : e.x, (w.y+e.y)/2); }
  point west( ) const { return point(e.x<w.x? e.x : w.x, (w.y+e.y)/2); }
  point neast( ) const { return point(w.x<e.x? e.x : w.x, e.y<w.y? w.y : e.y); }
  point seast( ) const { return point(w.x<e.x? e.x : w.x, e.y<w.y? e.y : w.y); }
  point nwest( ) const { return point(w.x<e.x? w.x : e.x, e.y<w.y? w.y : e.y); }
  point swest( ) const { return point(w.x<e.x? w.x : e.x, e.y<w.y? e.y : w.y); }
  void move(int a, int b) 	{ w.x += a; w.y += b; e.x += a; e.y += b; }
  void draw( ) { put_line(w, e); }
  void resize(int d) // ���������� ����� ����� � (d) ���
  { e.x += (e.x - w.x) * (d - 1); e.y += (e.y - w.y) * (d - 1); }
};


// �������������
class rectangle : public rotatable {
/* nw ------ n ------ ne
   |		       |
   |		       |
   w	   c            e
   |		       |
   |		       |
   sw ------- s ------ se */
protected:
  point sw, ne;
public:
  rectangle(point a, point b) :  sw(a), ne(b) { }
  point north( ) const { return point((sw.x + ne.x) / 2, ne.y); }
  point south( ) const { return point((sw.x + ne.x) / 2, sw.y); }
  point east( ) const { return point(ne.x, (sw.y + ne.y) / 2); }
  point west( ) const { return point(sw.x, (sw.y + ne.y) / 2); }
  point neast( ) const { return ne; }
  point seast( ) const { return point(ne.x, sw.y); }
  point nwest( ) const { return point(sw.x, ne.y); }
  point swest( ) const { return sw; }
  void rotate_right() // ������� ������ ������������ se
	{
	    float xCenter,yCenter;
	    point swNew,neNew;
	    xCenter = seast().x;
	    yCenter = seast().y;
	    neNew = get_point_rotated_right(ne,xCenter,yCenter);
	    swNew = get_point_rotated_right(sw,xCenter,yCenter);
	    ne = point(std::max(neNew.x,swNew.x),std::max(neNew.y,swNew.y));
	    sw = point(std::min(neNew.x,swNew.x),std::min(neNew.y,swNew.y));

    }
  void rotate_left() // ������� ����� ������������ sw
	{
	    float xCenter,yCenter;
	    point swNew,neNew;
	    xCenter = swest().x;
	    yCenter = swest().y;
	    neNew = get_point_rotated_left(ne,xCenter,yCenter);
	    swNew = get_point_rotated_left(sw,xCenter,yCenter);
	    ne = point(std::max(neNew.x,swNew.x),std::max(neNew.y,swNew.y));
	    sw = point(std::min(neNew.x,swNew.x),std::min(neNew.y,swNew.y));

	}
  void move(int a, int b)
	{ sw.x += a; sw.y += b; ne.x += a; ne.y += b; }
    void resize(int d)
  { ne.x += (ne.x - sw.x) * (d - 1); ne.y += (ne.y - sw.y) * (d - 1); }
  void draw( )
  {
    put_line(nwest( ), ne);   put_line(ne, seast( ));
    put_line(seast( ), sw);   put_line(sw, nwest( ));
  }
};


void up(shape& p, const shape& q) // ��������� p ��� q
{	//��� ������� �������, �� ���� ������! ������������ ����������!!
  point n = q.north( );
  point s = p.south( );
  p.move(n.x - s.x, n.y - s.y + 1);
}
