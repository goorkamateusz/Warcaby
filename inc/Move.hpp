#ifndef MOVE_LIST_CHECKERS_HPP
#define MOVE_LIST_CHECKERS_HPP

#include "AI_config.hpp"
#include "Vector2D.hpp"
#include "Enumarate.hpp"
#include "myTree.hpp"

#include <iostream>

/**
 * \brief Przechowuje informacje o ruchu jaki należy wykonać.
 */
class Move {
public:
    static uint8_t   maxDeep;            ///< Głębokość rekurendji
    static short int captureHumanPawn;   ///< Punkty za zbicie pionka gracza ( > 0 )
    static short int captureHumanLady;   ///< Punkty za zbicie damki gracza  ( > 0 )
    static short int captureBotPawn;     ///< Punkty za zbicie pionka bota   ( < 0 )
    static short int captureBotLady;     ///< Punkty za zbicie damki bota    ( < 0 )
    static float     deepScale;          ///< Ulamek udzialu glebokosci rekurencji w wyliczaniu wagi

public:
    Vector2D    co;         ///< Współrzedne pola
    Vector2D    mv;         ///< Wektor ruchu
    Data        data;       ///< Dane ruchu
    uint8_t     deep;       ///< Głębokość rekurencji, na której został wykonany ruch

public:
    friend std::ostream& operator << ( std::ostream& strm, const Move& mv );

    /**
     * \brief Konstruktor
     * \warning Używać tylko dla nie używanych gałęźi drzewa!
     */
    Move();

    /**
     * \brief Konstruktor parametryczny
     * \param co_ - wsp pionka
     * \param mv_ - wektor ruchu
     * \param deep - glebokosc rekurencji
     * \param val - wartość pola startowego
     * \param cap - czy bicie
     */
    Move( const Vector2D& co_, const Vector2D& mv_, const Field val, uint8_t deep, const bool cap );

    /**
     * \brief Zwraca punktowanie ruchu
     * \return short int -
     */
    short int points() const;

    /**
     * \brief Czy maksymalizować obliczenia dla danego ruchu
     * \return true - tak, jeśli to ruch bota
     * \return false - nie, jeśli to ruch gracza
     */
    inline bool maxim() const{ return( ( data & Data::BotsMove ) == Data::No ); }

    /**
     * \brief Losuje nastawy bota
     */
    static void randomBot();
};

/**
 * \brief Drukowanie klasy Move
 * \param strm -
 * \param mv -
 * \return std::ostream& -
 */
std::ostream& operator << ( std::ostream& strm, const Move& mv );


/** \brief Alians */
typedef myTree<Move> mvTree;

#endif