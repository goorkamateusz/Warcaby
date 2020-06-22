#ifndef VECTOR_2D_HPP
#define VECTOR_2D_HPP

#include <iostream>
#include <cmath>

/**
 * \brief Klasa wektora dwu wymiarowego
 */
class Vector2D {
public:
    int8_t X;        ///< Współrzędna X wektora
    int8_t Y;        ///< Współrzędna Y wektora

public:
    /** \brief Konstruktor. */
    Vector2D() = default;

    /** \brief Konstruktor parametryczny. */
    Vector2D( int8_t x, int8_t y ){ X = x; Y = y; }

    /** \brief Konstruktor kopiujący. */
    Vector2D( const Vector2D& b ){ this->X = b.X; this->Y = b.Y;  }

    /** \brief Współrzedna x. */
    inline int8_t& x()       { return X; }
    /** \brief Współrzedna x. */
    inline int8_t  x() const { return X; }

    /** \brief Współrzedna y. */
    inline int8_t& y()       { return Y; }
    /** \brief Współrzedna y. */
    inline int8_t  y() const { return Y; }

    /** \brief Dlugosc wektora */
    float len() const { return sqrtf( (X*X) + (Y*Y) ); }

    /**
     * \brief Dodawanie
     * \param b - drugi składnik
     * \return Vector2D -
     */
    inline Vector2D operator + ( Vector2D b ) const
    { b += *this;   return b; }

    /**
     * \brief Odejmowanie
     * \param b - drugi składnik
     * \return Vector2D -
     */
    inline Vector2D operator - ( Vector2D b ) const
    { b -= *this;   return b; }

    /**
     * \brief Skrócony operator dodawania
     * \param b - elemeny o jaki powiększyć
     * \return Vector2D& -
     */
    inline Vector2D& operator += ( const Vector2D& b ){
        this->X += b.X;   this->Y += b.Y;
        return *this;
    }

    /**
     * \brief Skrócony operator odejmowania
     * \param b -
     * \return Vector2D& -
     */
    inline Vector2D& operator -= ( const Vector2D& b ){
        this->X -= b.X;   this->Y -= b.Y;
        return *this;
    }

    /**
     * \brief Mnożenie przez skalar
     * \param scalar -
     * \return Vector2D - 
     */
    inline Vector2D operator * ( const int scalar ) const {
        Vector2D b( *this );
        b.X *= scalar;
        b.Y *= scalar;
        return b;
    }

    /**
     * \brief Operator porownania
     * \param vec -
     * \return true -
     * \return false -
     */
    inline bool operator == ( const Vector2D& vec ) const
    { return( this->X == vec.X && this->Y == vec.Y); }

};

/**
 * \brief Drukuje wektor
 * \param strm -
 * \param vec -
 * \return std::ostream& -
 */
std::ostream& operator << ( std::ostream& strm, const Vector2D& vec );


#endif