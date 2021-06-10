//========== ���� shape.cpp (���������� ���������) ==========
// ���������� � ������������� ���������� �����
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "screen.h"
#include "shape.h"


class circle : public shape {
protected:
    point c;
    int radius;
public:
    circle(point a, int r);
    void draw();
    point north() const { return point(c.x,c.y+radius); }
    point south() const { return point(c.x,c.y-radius); }
    point east() const { return point(c.x+radius,c.y); }
    point west() const { return point(c.x-radius,c.y); }
    point neast() const { return point(c.x+radius*0.7, c.y+radius*0.7); }
    point seast() const { return point(c.x+radius*0.7, c.y-radius*0.7); }
    point nwest() const { return point(c.x-radius*0.7, c.y+radius*0.7); }
    point swest() const { return point(c.x-radius*0.7, c.y-radius*0.7); }
    void move(int a , int b) { c.x += a; c.y += b; } //�����������
    void resize(int d) { (d>0) ? radius *= d : radius /= -d; } //��������� �������
private:
    circle(const circle &x) { std::cout << "copy constructor\n"; } // ����������� �����������
    circle& operator=(const circle &x) { }// �������� ������������ ������������
    circle(circle &&x) { std::cout << "move constructor\n"; } // ����������� �����������
    circle& operator=(circle &&x) { } // �������� ������������ ������������
};
circle::circle(point a, int r) : c(a), radius(r) {
    if((!on_screen(c.x+r, c.y)) || (!on_screen(c.x,c.y+r)) || (!on_screen(c.x-r,c.y)) || (!on_screen(c.x,c.y-r)))
        throw OffScreen("������ �" + std::to_string(id) + " ��� �������� ��������� ��� ������.", a);
}
void circle::draw() {
    char buff[YMAX][XMAX];
    // �������� ����������� ������ �� ���������
    /*for (int i=0; i<YMAX; i++)
        for (int j=0; j<XMAX; j++)
            buff[i][j] = screen[i][j];*/
    try {
        int x = 0, y = radius, delta = 1 - 2 * radius, error = 0;
        while (y >= 0) {
            put_point(c.x + x, c.y + y);
            put_point(c.x + x, c.y - y);
            put_point(c.x - x, c.y + y);
            put_point(c.x - x, c.y - y);
            error = 2 * (delta + y) - 1;
            if (delta < 0 && error <= 0) {
                ++x;
                delta += 2 * x + 1;
                continue;
            }
            error = 2 * (delta - x) - 1;
            if (delta > 0 && error > 0) {
                --y;
                delta += 1 - 2 * y;
                continue;
            }
            ++x;
            delta += 2 * (x - y);
            --y;
        }
    }
    catch (OffScreen &ex) {
        std::cout << "������ ������ ������ �" << id << " (" << ex.what() << ex.crnds() << ")\n\n";
        resize(-2);
        try {
            int x = 0, y = radius, delta = 1 - 2 * radius, error = 0;
            while (y >= 0) {
                put_point(c.x + x, c.y + y);
                put_point(c.x + x, c.y - y);
                put_point(c.x - x, c.y + y);
                put_point(c.x - x, c.y - y);
                error = 2 * (delta + y) - 1;
                if (delta < 0 && error <= 0) {
                    ++x;
                    delta += 2 * x + 1;
                    continue;
                }
                error = 2 * (delta - x) - 1;
                if (delta > 0 && error > 0) {
                    --y;
                    delta += 1 - 2 * y;
                    continue;
                }
                ++x;
                delta += 2 * (x - y);
                --y;
            }
            std::cout << "������ �" << id << " ���� ��������� � ��� ���� ��� ����������� ������\n";
        }
        catch (OffScreen) {
            resize(2);
            throw;
        }
        // ���������� ����������� ������ � �������� ���������
        /*for (int i=0; i<YMAX; i++)
            for (int j=0; j<XMAX; j++)
                screen[i][j] = buff[i][j];*/
    }
}

// ������ �������: �������������� �������� - ��������������
class h_circle: public rectangle, public reflectable {
    bool reflected;
public:
    h_circle(point a, point b, bool r);
    void draw();
    void flip_horisontally( ) { }; // �������� ������������� (������ �������)
    void flip_vertically( ) { reflected = !reflected; }; // �������� �����������
};
h_circle::h_circle(point a, point b, bool r=true) : rectangle(a, b), reflected(r) {
    if((!on_screen(a.x, a.y)) || (!on_screen(b.x,b.y)))
        throw OffScreen("������ �" + std::to_string(id) + " ��� �������� ��������� ��� ������.", point((east().x-west().x)/2, (north().y-south().y)/2));
}
void h_circle :: draw() //�������� ���������� ��� �����������
{ //(�������� ��� �������, ����������� ��������� reflected)
    int x0 = (sw.x + ne.x)/2, y0 = reflected ? sw.y : ne.y;
    int radius = (ne.x - sw.x)/2;
    int x = 0, y = radius, delta = 2 - 2 * radius, error = 0;
    while(y >= 0) { // ���� ���������
        if(reflected) { put_point(x0 + x, y0 + y*0.7); put_point(x0 - x, y0 + y*0.7); }
        else { put_point(x0 + x, y0 - y*0.7); put_point(x0 - x, y0 - y*0.7); }
        error = 2 * (delta + y) - 1;
        if(delta < 0 && error <= 0) { ++x; delta += 2 * x + 1; continue; }
        error = 2 * (delta - x) - 1;
        if(delta > 0 && error > 0) { --y; delta += 1 - 2 * y; continue; }
        ++x; delta += 2 * (x - y); --y;
    }
}
// ������ �������: �������������� ������� ��������������
void down(shape &p, const shape &q) {
    point n = q.south( );
    point s = p.north( );
    p.move(n.x - s.x, n.y - s.y - 1);
}
void centering(shape &p, const shape &q) {
    int delta_x = (q.east().x + q.west().x)/2 - (p.east().x + p.west().x)/2, // (����� ������ q) - (����� ������ p), ���������� ���
        delta_y = (q.east().y + q.west().y)/2 - (p.east().y + p.west().y)/2; // (����� ������ q) - (����� ������ p), ���������� �����
    p.move(delta_x, delta_y);
}
void rightDown(shape &p, shape&q) {
    point n = q.seast();
    point s = p.nwest();
    p.move(n.x - s.x, n.y - s.y);
}

void leftDown(shape &p, shape &q){
    point n = q.swest();
    point s = p.neast();
    p.move(n.x - s.x - 1, n.y - s.y);
}
// C������ ���������������� ������ - ����������
class myshape : public rectangle { // ��� ������ ��������
    int w, h; // ���������������
    line l_eye; // ����� ���� � ��� ������ �������� �����
    line r_eye; // ������ ����
    line mouth; // ���
public:
    myshape(point, point);
    void draw( );
    void move(int, int);
    void resize(int) { }
};
myshape :: myshape(point a, point b)
   :rectangle(a, b), //������������� �������� ������
    w(neast( ).x - swest( ).x + 1), // ������������� ������
    h(neast( ).y - swest( ).y + 1), // - ������ � ������� ����������!
    l_eye(point(swest( ).x + 2, swest( ).y + h * 3 / 4), 2),
    r_eye(point(swest( ).x + w - 4, swest( ).y + h * 3 / 4), 2),
    mouth(point(swest( ).x + 2, swest( ).y + h / 4), w - 4)
{ }
void myshape :: draw( )
{
    rectangle :: draw( ); //������ ���� (����� � ��� �������� ����!)
    int a = (swest( ).x + neast( ).x) / 2;
    int b = (swest( ).y + neast( ).y) / 2;
    put_point(point(a, b)); // ��� � ���������� ������ �� �������!
}
void myshape :: move(int a, int b)
{
    rectangle :: move(a, b);
    l_eye.move(a, b); r_eye.move(a, b);
    mouth.move(a, b);
}
// ������, ���������� ������ � ������� ��� ��������/��������� (������� �������� 3x3)
class error_model : public rotatable, public reflectable {
private:
    point s;
public:
    error_model(point point_) : s(point_) {}
    point north() const{ return point(s.x, s.y + 1); }
    point south() const{ return point(s.x, s.y - 1); }
    point east() const{ return point (s.x + 1, s.y); }
    point west() const{ return point(s.x - 1, s.y); }
    point neast() const{ return point(s.x + 1, s.y + 1); }
    point seast() const{ return point(s.x + 1, s.y - 1); }
    point nwest() const{ return point(s.x - 1, s.y + 1); }
    point swest() const{ return point(s.x - 1, s.y - 1); }

    void draw();
    void move(int dx, int dy) { s.x += dx; s.y += dy; };
    // ��������� ��������� ������ �����������
    void resize(int) { }
    void rotate_left() { }
    void rotate_right() { }
    void flip_vertically() { }
    void flip_horisontally() { }
};
void error_model::draw() {
    error_sym = true;
    put_line(nwest(), seast());
    put_line(swest(), neast());
    error_sym = false;
}
int main( )
{
    setlocale(LC_ALL, "Rus");
    screen_init( );
    //== 1.���������� ������ ����� ==
    rotatable *hat;
    try {
        hat = new rectangle(point(0, 0), point(14, 5));
    }
    catch (OffScreen &ex) {
        std::cout << ex.what() << "\n";
        hat = new error_model(ex.get_point());
    }
    shape *brim;
    try {
        brim = new line(point(0,15),17);
    }
    catch (OffScreen &ex) {
        std::cout << ex.what() << "\n";
        brim = new error_model(ex.get_point());
    }
    shape *emblem;
    try {
        emblem = new circle(point(60, 15), 2);
    }
    catch (OffScreen &ex) {
        std::cout << ex.what() << "\n";
        emblem = new error_model(ex.get_point());
    }
    shape *whisker_left;
    try {
        whisker_left = new circle(point(70,9), 10);
    }
    catch (OffScreen &ex) {
        std::cout << ex.what() << "\n";
        whisker_left = new error_model(ex.get_point());
    }
    shape *whisker_right;
    try {
        whisker_right = new circle(point(70,20), 8);
    }
    catch (OffScreen &ex) {
        std::cout << ex.what() << "\n";
        whisker_right = new error_model(ex.get_point());
    }
    shape *face;
    try {
        face = new myshape(point(15,10), point(27,18));
    }
    catch (OffScreen &ex) {
        std::cout << ex.what() << "\n";
        face = new error_model(ex.get_point());
    }
    reflectable *beard;
    try {
        beard = new h_circle(point(40,10), point(50,20));
    }
    catch (OffScreen &ex) {
        std::cout << ex.what() << "\n";
        beard = new error_model(ex.get_point());
    }
    shape_refresh();
    std::cout << "=== Generated... ===\n";
    std::cin.get(); //�������� �������� �����

    //== 2.���������� � ������ ==
    hat->rotate_right();
    brim->resize(2);
    face->resize(2);
    beard->flip_vertically();
    whisker_left->resize(-5);
    whisker_right->resize(-4);
    shape_refresh( );
    std::cout << "=== Prepared... ===\n";
    std::cin.get(); //�������� ��������� ���������/���������

    //== 3.������ ����������� ==
    up(*brim, *face);
    up(*hat, *brim);
    down(*beard, *face);
    centering(*emblem, *hat);
    rightDown(*whisker_left, *face);
    leftDown(*whisker_right, *face);
    shape_refresh( );
    std::cout << "=== Ready! ===\n";
    std::cin.get(); //�������� ���������
    screen_destroy( );
    delete hat;
    delete brim;
    delete emblem;
    delete whisker_left;
    delete whisker_right;
    delete face;
    delete beard;

    return 0;
}
