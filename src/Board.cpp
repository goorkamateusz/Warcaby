#include "Board.hpp"
using namespace std;

////----------------------------------------------------------------------------------------------------------
Vector2D const Board::PossibleMoves[] = { Vector2D(1,-1), Vector2D(-1,-1), Vector2D(-1,1), Vector2D(1,1) };

////-----------------------------------------------------------------------------------------------------------
Player operator ! ( const Player& player )
{ return( (player==Player::Human)? Player::Bot : (player==Player::Bot)? Player::Human : Player::Nobody ); }

////-----------------------------------------------------------------------------------------------------------
Field operator | ( const Field a, const Field b ){ return (enum Field)( int8_t(a) | int8_t(b) ); }
Field operator & ( const Field a, const Field b ){ return (enum Field)( int8_t(a) & int8_t(b) ); }

////-----------------------------------------------------------------------------------------------------------
Data operator | ( const Data a, const Data b ){ return (enum Data)( int8_t(a) | int8_t(b) ); }
Data operator & ( const Data a, const Data b ){ return (enum Data)( int8_t(a) & int8_t(b) ); }

////-----------------------------------------------------------------------------------------------------------
/**
 * \retval false - jesli bledny ruch
 * \retval false - jesli mozliwe kolejne bicie
 * \retval true  - jesli zakonczono kolejke gracza
 * \post Modyfikuje plansze
 */
bool Board::makeHumanMove( const Vector2D& pawn, const Vector2D& move ){
    if( ! isEmpty( move ) ) return false;
    else {
        Vector2D dif = pawn-move;
        bool forced = forcedCapture();

        // Ruch
        if( ! forced ){
            for(short int i=possibleStart(pawn); i < possibleEnd(pawn); ++i ){
                if( dif == PossibleMoves[i] ){
                    //* Poprawny ruch
                    this->saveCopy();
                    // Wykonuje ruch
                    swap( field( pawn ), field( move ) );
                    if( move.Y == 0 ) field( move ) = Field::Human | Field::Lady;
                    return true;
                }
            }
        }

        // Bicie
        for(short int i=0; i < 4; ++i ){
            if( dif == PossibleMoves[i]*2 ){
                if( isBot( pawn + PossibleMoves[i] ) ){
                    //* Poprawne bicie
                    this->saveCopy();
                    // Wykonuje bicie
                    swap( field( pawn ), field( move ) );
                    field( pawn + PossibleMoves[i] ) = Field::Empty;

                    if( move.Y == 0 ) field( move ) = Field::Human | Field::Lady;

                    if( forcedCapture( Player::Human, move ) ) return false;
                    else return true;
                }
                else break;
            }
        }

        if( forced ) throw "_Nakaz bicia!";
    }
    return false;
}

////-----------------------------------------------------------------------------------------------------------
bool Board::noHumanMove() const {
     for(Vector2D pawn(0,0); pawn.Y < BSIZE; ++pawn.Y )
        for( pawn.X = pawn.Y%2; pawn.X < BSIZE; pawn.X += 2)
            if( this->player( pawn ) == Player::Human ){
                for( short int i=possibleStart(pawn); i<possibleEnd(pawn); ++i )
                    if( isEmpty(pawn+PossibleMoves[i] ) ) return false;

                for( short int i=0; i < 4; ++i )
                    if( isEmpty( pawn + PossibleMoves[i]*2 ) )
                        if( isEnemies( pawn+PossibleMoves[i], pawn ) ) return false;
            }
    return true;
}

////-----------------------------------------------------------------------------------------------------------
bool Board::forcedCapture( const Player player ){
    for(Vector2D pawn(0,0); pawn.Y < BSIZE; ++pawn.Y )
        for( pawn.X = pawn.Y%2; pawn.X < BSIZE; pawn.X += 2)
            if( this->player( pawn ) == player )
                for( short int i=0; i < 4; ++i )
                    if( isEmpty( pawn + PossibleMoves[i]*2 ) )
                        if( isEnemies( pawn+PossibleMoves[i], pawn ) ) return true;
    return false;
}

////-----------------------------------------------------------------------------------------------------------
bool Board::forcedCapture( const Player player, const Vector2D& pawn ){
    if( this->player( pawn ) == player )
        for( short int i=0; i < 4; ++i )
            if( isEmpty( pawn + PossibleMoves[i]*2 ) )
                if( isEnemies( pawn+PossibleMoves[i], pawn ) ) return true;
    return false;
}

////-----------------------------------------------------------------------------------------------------------
void Board::prepare(){
    int x = 0, y = 0, i = 0;

    for( y=0; y<BSIZE; ++y )
        for( x=0; x<BSIZE; ++x )
            board[x][y] = Field::Empty;

    x = 0; y = 0;

    while( y < BSIZE ){
        if     ( i < 12 ) board[ x ][ y ] = Field::Bot | Field::Pawn;
        else if( i < 20 ) board[ x ][ y ] = Field::Empty;
        else              board[ x ][ y ] = Field::Human | Field::Pawn;

        ++i;
        x += 2;
        if( x >= BSIZE ){
            ++y;
            x = y%2;
        }
    }
}

////-----------------------------------------------------------------------------------------------------------
void Board::saveCopy(){
    int x=0, y=0;
    for( y = 0; y<BSIZE; y += 2 )
        for( x = 0; x<BSIZE; x += 2 ) board[x+1][y] = board[ x ][y];
    for( y = 1; y<BSIZE; y += 2 )
        for( x = 1; x<BSIZE; x += 2 ) board[x-1][y] = board[ x ][y];
}

////-----------------------------------------------------------------------------------------------------------
void Board::loadCopy(){
    int x=0, y=0;
    for( y = 0; y<BSIZE; y += 2 )
        for( x = 0; x<BSIZE; x += 2 ) board[x][y] = board[x+1][y];
    for( y = 1; y<BSIZE; y += 2 )
        for( x = 1; x<BSIZE; x += 2 ) board[x][y] = board[x-1][y];
}