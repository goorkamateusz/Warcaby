#ifndef BOARD_CHECKERS_HPP
#define BOARD_CHECKERS_HPP

#include "AI_config.hpp"
#include "Move.hpp"
#include "Enumarate.hpp"
#include "Vector2D.hpp"
#include <functional>

class Interface;

////--------------------------------------------------------------------------
#define BSIZE 8                 ///< Wymiar planszy
////--------------------------------------------------------------------------

/**
 * \brief Modeluje plansze w grze w warcaby.
 */
class Board {
////--------------------------------------------------------------------------
private:
    static Vector2D const PossibleMoves[ 4 ];   ///< Możliwe ruchy

private:
    Field   board[ BSIZE ][ BSIZE ];            ///< Plansza

public:
    /** \brief Konstruktor, przygotowuje plansze */
    Board(){ prepare(); saveCopy(); }

    /**
     * \brief Przygotowuje plansze do gry, "rozstawia pionki"
     */
    void prepare();

    /**
     * \brief Wartość pola
     * \param vec - wsp pola
     * \return Field - wartość
     */
    inline Field& field( const Vector2D& vec )
    { return board[ vec.x() ][ vec.y() ]; }
    inline Field  field( const Vector2D& vec ) const
    { return board[ vec.x() ][ vec.y() ]; }

    /**
     * \brief Wartość pola
     * \param x - wsp X
     * \param y - wsp Y
     * \return Field - wartość
     */
    inline Field field( const unsigned int x, const unsigned int y ) const { return board[x][y]; }

////--------------------------------------------------------------------------
public:
    /**
     * \brief Zapisuje kopię planszy
     */
    void saveCopy();

    /**
     * \brief Wczytuje kopię planszy
     */
    void loadCopy();

////--------------------------------------------------------------------------------------------------------
public:
    /**
     * \brief Czyje jest pole o wsp. vec?
     * \param vec - współrzędne
     * \return Player - "właściciel"
     */
    inline Player player( const Vector2D& vec ) const {
        if( isField( vec ) ) {
            if( isBot( vec )    ) return Player::Bot;
            if( isHuman( vec )  ) return Player::Human;
        }
        return Player::Nobody;
    }

    /**
     * \brief Czy to pionki przeciwnikow?
     * \param a - wsp pionka A
     * \param b - wsp pionka B
     */
    inline bool isEnemies( const Vector2D& a, const Vector2D& b ) const {
        if( isField( a ) && isField( b ) )
            if( board[a.X][a.Y] != Field::Empty && board[b.X][b.Y] != Field::Empty )
                if( player( a ) != player( b ) ) return true;
        return false;
    }

    /**
     * \brief Czy pole gracza?
     * \param vec - współrzedne
     * \return true - gracz
     * \return false - nie gracz
     */
    inline bool isHuman( const Vector2D& vec ) const
    { return( board[ vec.x() ][ vec.y() ] & Field::Human ) != Field::Empty; }

    /**
     * \brief Czy pole bota?
     * \warning Nie sprawdza czy
     * \param vec - współrzedne
     * \return true - bot
     * \return false - nie bot (gracz lub puste)
     */
    inline bool isBot( const Vector2D& vec ) const
    { return( board[ vec.x() ][ vec.y() ] & Field::Bot ) != Field::Empty; }

    /**
     * \brief Czy pole jest puste?
     * \param vec - współrzędne
     * \return true - puste
     * \return false - nie puste
     */
    inline bool isEmpty( const Vector2D& vec ) const {
        if( isField( vec ) ) return( board[ vec.x() ][ vec.y() ] == Field::Empty );
        return false;
    }

    /**
     * \brief Czy współrzędne należą do planszy?
     * \param vec - współrzędne
     * \return true - należą
     * \return false -  nie należą
     */
    inline bool isField( const Vector2D& vec ) const
    { return (vec.x() >= 0 && vec.x() < BSIZE) && (vec.y() >= 0 && vec.y() < BSIZE); }

    /**
     * \brief Czy pole jest damką?
     * \param vec - współrzędne
     */
    inline bool isLady( const Vector2D& vec ) const
    { return( board[ vec.x() ][ vec.y() ] & Field::Lady )  != Field::Empty; }

////--------------------------------------------------------------------------------------------------------
public:
    /**
     * \brief Wykonuje ruch gracza
     * \param pawn - pionek (pozycja startowa)
     * \param move - pionek (pozycja koncowa)
     * \return true  - bot moze wykonac ruch
     * \return false - kontynuacja ruchu gracza
     */
    bool makeHumanMove( const Vector2D& pawn, const Vector2D& move );

    /**
     * \brief Czy gracz ma nakaz bicia?
     * \param player - gracz
     * \return true - nakaz bicia
     * \return false - brak nakazu
     */
    bool forcedCapture( const Player player = Player::Human );

    /**
     * \brief Czy gracz ma nakaz bicia danym pionkiem?
     * \param player - gracz
     * \param pawn - współrzedne pionka
     */
    bool forcedCapture( const Player player, const Vector2D& pawn  );

    /**
     * \brief Czy gracz nie moze wykonac ruchu
     * \return true - nie moze
     * \return false - moze
     */
    bool noHumanMove() const;

////--------------------------------------------------------------------------------------------------------
public:
    /**
     * \brief Wykonuje najlepszy ruch dla bota
     * \param gui - interface
     * \param deep - głębokość rekurencji
     */
    void makeBotsMove( Interface *const gui, uint8_t deep );

    /**
     * \brief Znajduje najlepszy ruch dla graca
     * \param[out] tree - drzewo ruchow
     * \param[in]  player - gracz
     */
    void findBestMove( mvTree* const tree, const Player player, uint8_t deep );

    /**
     * \brief Znajduje najlepszy ruch dla danego pionka
     * \param[out] tree - drzewo [out]
     * \param[in]  pawn - wsp pionka
     * \param[in]  deep - głębokość rekurencji
     */
    void findBestMove( mvTree* const tree, const Vector2D& pawn, uint8_t deep );

    /**
     * \brief Znajduje najlepsze bicie
     * \param[out] tree - drzewo [out]
     * \param[in]  pawn - wsp pionka
     * \param[in]  deep - głębokość rekurencji
     */
    void findBestCapture( mvTree* const tree, const Vector2D& pawn, uint8_t deep );

////--------------------------------------------------------------------------------------------------------
private:
    /**
     * \brief Poczatkowy indeks tablicy PossibleMove[]
     * \param vec - współrzedne
     * \return short int -
     */
    inline short int possibleStart( const Vector2D& vec ) const
    { return possibleStart( field( vec ) ); }

    /**
     * \brief Poczatkowy indeks tablicy PossibleMove[]
     * \param val - wartosc pionka
     * \return short int -
     */
    inline short int possibleStart( const Field val ) const {
        if(( val & Field::Lady ) != Field::Empty ) return 0;
        else return( (( val & Field::Human ) != Field::Empty )? 0 : 2 );
    }

    /**
     * \brief Końcowy indeks tablicy PossibleMove[]
     * \param vec - współrzedne
     * \return short int -
     */
    inline short int possibleEnd( const Vector2D& vec ) const
    { return possibleEnd( field( vec ) ); }

    /**
     * \brief Końcowy indeks tablicy PossibleMove[]
     * \param val - wartosc pionka
     * \return short int -
     */
    inline short int possibleEnd( const Field val ) const {
        if(( val & Field::Lady ) != Field::Empty ) return 4;
        else return( (( val & Field::Human ) != Field::Empty )? 2 : 4 );
    }


////--------------------------------------------------------------------------------------------------------
public:
    /**
     * \brief Wyjątek wykonania operacji nie mozliwej dla pustego pola
     */
    class err_no_for_empty{};

    /**
     * \brief Wyjątek wykonania operacji nie mozliwej dla gracza "Nobody"
     */
    class err_no_for_nobody{};

};

#endif