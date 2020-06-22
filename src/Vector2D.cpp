#include "Vector2D.hpp"

std::ostream& operator << ( std::ostream& strm, const Vector2D& vec ){
    strm << "(" << (int)vec.x() << "," << (int)vec.y() << ")";
    return strm;
}