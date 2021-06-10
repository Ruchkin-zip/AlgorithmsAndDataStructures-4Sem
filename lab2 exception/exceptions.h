#include <exception>
#include <string>
// ошибка: точка вне экрана
struct OffScreen : std::exception {
public:
    OffScreen(const std::string& s, point err) : msg_(s), point_(err) { } // ошибка при изменении фигуры
    virtual char const *what() const noexcept { return msg_.c_str(); }
    virtual point get_point() { return point_; }
    virtual std::string crnds() { return "("+std::to_string(point_.x)+", "+std::to_string(point_.y)+")"; }
private:
    std::string msg_; // сообщение ошибки
    point point_; // точка, вызвавшая ошибку
};
/*
struct CantBeMoved : std::exception {
    CantBeMoved(const std::string& s) : std::exception(s.c_str( )) { }
};

struct CantBeDrawn : std::exception {
    CantBeDrawn(const std::string& s) : std::exception(s.c_str( )) { }
};
*/
