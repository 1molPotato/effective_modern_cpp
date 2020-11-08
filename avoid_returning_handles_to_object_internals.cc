#include <iostream>
#include <memory>

using namespace std;

class Point {
public:
    Point() = default;
    Point(int x, int y) : x_(x), y_(y) {}
    void setX(int x) { x_ = x; }
    void setY(int y) { y_ = y; }
    int X() const { return x_; }
    int Y() const { return y_; }
private:
    int x_;
    int y_;
};

struct RectData {
    RectData() = default;
    Point ulhc;
    Point lrhc;
};

class Rectangle {
public:
    Rectangle(int x1, int y1, int x2, int y2) {
        data_ = make_shared<RectData>();
        data_->ulhc.setX(x1);
        data_->ulhc.setY(y1);
        data_->lrhc.setX(x2);
        data_->lrhc.setY(y2);
    }
    const Point& upperLeft() const { return data_->ulhc; }
    const Point& lowerRight() const { return data_->lrhc; }

private:
    shared_ptr<RectData> data_;
};

const shared_ptr<Rectangle> makeRectPtr(int x1, int y1, int x2, int y2) {
    auto rect = make_shared<Rectangle>(x1, y1, x2, y2);
    return rect;
}

int main() {
    const auto& ul = makeRectPtr(0, 0, 100, 100)->upperLeft();  // it dangles
    cout << ul.X() << ", " << ul.Y() << endl;                   // the results might be correct because the memory hasn't been overwritten

    auto newRect = makeRectPtr(10, 10, 50, 50);                 // do something to allocate memory
    cout << ul.X() << ", " << ul.Y() << endl;                   // output: 10, 10
                                                                // which means 
    return 0;
}