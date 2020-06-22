#include "Board.hpp"
#include "Interface.hpp"
#include <iostream>
using namespace std;

////---------------------------------------------------------------------------------------
////---------------------------------------------------------------------------------------
void Board::makeBotsMove( Interface *const gui, uint8_t deep ){

    mvTree *moves = new mvTree( Move() );   ///<< Drzewo ruchow

    /// - Znajduje wszystkie mozliwe ruchu, w ramach głębokości deep
    findBestMove( moves, Player::Bot, deep );

    #if DEBUG_TEST > 1
        cout << endl << endl << "ZAKONCZONO SYMULACJE" << endl;
        moves->display();
    #endif

    /// - Znajduje najlepszy ze znajelzionych ruch
    moves->bestBranch(
        []( const Move& mv ){ return mv.points(); },
        []( const Move& mv ){ return mv.maxim(); }
    );

    #if DEBUG_TEST > 0
        cout << endl << "NAJLEPSZA SCIEZKA" << endl;
        moves->display();
    #endif

    /// - Jeśli nie znaleziono ruchu wywołuje wyjątek "WYGRANA!"
    if( ! moves->haveChild() ){
        throw "EWYGRANA! :)";
        return;
    }

    /// - Wykonuje wybrany ruch
    myTree<Move> *firstborn = moves;
    Move domove;
    while( firstborn->haveChild() ){
        firstborn = firstborn->Firstborn();
        domove = firstborn->v();

        if(( domove.data & Data::BotsMove ) != Data::No ) break;       // koniec tury bota

        if(( domove.data & Data::Capture ) != Data::No ){              //* bicie
            swap( field( domove.co ), field( domove.co + domove.mv*2 ) );

            if( ( domove.co+domove.mv*2 ).Y == 7 )                     // ustawienie damki
                field(domove.co+domove.mv*2) = Field::Bot | Field::Lady;

            gui->display( this );                                      // opoznienie
            sleep( sf::milliseconds(400) );

            field( domove.co + domove.mv ) = Field::Empty;
        }
        else {                                                         //* ruch
            swap( field( domove.co ), field( domove.co + domove.mv ) );

            if( ( domove.co+domove.mv ).Y == 7 )                       // ustawienie damki
                field(domove.co+domove.mv) = Field::Bot | Field::Lady;
        }

        gui->display( this );
        sleep( sf::milliseconds(750) );
    }

    delete moves;
}

////---------------------------------------------------------------------------------------
////---------------------------------------------------------------------------------------
/**
 * \details
 * ```
 * znajdzNajlepszyRuch( drzewo, gracz, głębokość ):
 *  jeśli głębokość < 0:
 *      głebokość = głębokość - 1
 *      dla każdego pionka gracza
 *          znajdzNajlepszeBicie( drzewo, pionek, głębokość )
 *
 *      jeśli nie ma możliwości bicia
 *          dla każdego pionka gracza
 *              znajdzNajlepszyRuch( drzewo, pionek, głębokość )
 * ```
 */
void Board::findBestMove( mvTree* const tree, const Player player, uint8_t deep ){
    /// - Wykonuje symulacje dla każdego z pionkow gracza
    if( ! deep ) return;
    if( player == Player::Nobody ) throw err_no_for_nobody();

    --deep;

    #if DEBUG_TEST > 2
        for( int k=8; k>deep; --k ) cout << ".  ";
        cout << deep << " findBestMove gracz   | g:" << (int) player << endl;
    #endif

    /// - Najpierw sprawdza mozliwosci bicia
    for(Vector2D pawn(0,0); pawn.Y < BSIZE; ++pawn.Y )
        for( pawn.X = pawn.Y%2; pawn.X < BSIZE; pawn.X += 2){

            if( this->player( pawn ) == player )
                findBestCapture( tree, pawn, deep );
        }

    if( tree->haveChild() ) return;

    /// - Nastepnie mozliwosci ruchu
    for(Vector2D pawn(0,0); pawn.Y < BSIZE; ++pawn.Y )
        for( pawn.X = pawn.Y%2; pawn.X < BSIZE; pawn.X += 2){

            if( this->player( pawn ) == player ){
                findBestMove( tree, pawn, deep );
            }
        }
}

////---------------------------------------------------------------------------------------
////---------------------------------------------------------------------------------------
/**
 * \details
 * ```
 * znajdzNajlepszyRuch( drzewo, pionek, głębokość ):
 *  jeśli głębokość > 0:
 *      głębokość = głębokość - 1
 *
 *      dla dla każdej możliwości ruchu pionka:
 *          jeśli pole jest puste:
 *              wykonaj( ruch )
 *              drzewo.dodajMożliwość( ruch )
 *              znajdzNajlepszyRuch( drzewo, wróg(gracz), głębokość )
 *              cofnij( ruch )
 * ```
 */
void Board::findBestMove( mvTree* const tree, const Vector2D& pawn, uint8_t deep ){

    if( ! deep ) return;                                /// Przerywa jeśli koniec rekurencji
    if( isEmpty( pawn ) ) throw err_no_for_empty();     /// Wyjatek: err_no_for_empty()

    --deep;

    #if DEBUG_TEST > 2
        for( int k=8; k>deep; --k ) cout << ".  ";
        cout << deep << " findBestMove pionek | p: " << pawn << ", g:" << (int) player( pawn ) << endl;
    #endif

    /// Wykonuje symulacje dla danego pionka
    for( short int i=possibleStart( pawn ); i < possibleEnd( pawn ); ++i ){

        if( isEmpty( pawn + PossibleMoves[i] ) ){

            swap( field( pawn ), field( pawn + PossibleMoves[i] ) );    //* wykonaj ruch

            findBestMove(                                           //! Sprawdza odpowiedz wroga
                tree->addChild( Move(
                                pawn, PossibleMoves[i],
                                field( pawn + PossibleMoves[i] ),
                                deep, false
                            )),
                ! player( pawn + PossibleMoves[i] ),
                deep
            );

            swap( field( pawn ), field( pawn + PossibleMoves[i] ) );    //* cofnij ruch
        }
    }
}

////---------------------------------------------------------------------------------------
////---------------------------------------------------------------------------------------
/**
 * \details
 * ```
 * znajdzNajlepszeBicie( drzewo, pionek, głębokość ):
 *  dla dla każdej możliwości bicia pionka:
 *      drzewo.dodajMożliwość( ruch )
 *      wykonaj( ruch )
 *      znajdźNajlepszeBicie( drzewo, pionek, głębokość )
 *      jeśli zakończono bicie:
 *            znajdzNajlepszyRuch( drzewo, wróg(gracz), głębokość )
 *      cofnij( ruch )
 * ```
 * \post Nie zmniejsza deep! (ciag bic nie moze byc przerwany przez koniec rekurencji)
 */
void Board::findBestCapture( mvTree* const tree, const Vector2D& pawn, uint8_t deep ){

    #if DEBUG_TEST > 2
        for( int k=8; k>deep; --k ) cout << ".  ";
        cout << deep << " findBestCapture     | p: " << pawn << ", g:" << (int) player( pawn ) << endl;
    #endif

    Vector2D enemy;

    for(short int i=0; i < 4; ++i ){
        enemy = pawn + PossibleMoves[i];

        if( isEnemies( pawn, enemy ) ){                               //* Jeśli graniczy z wrogiem
            if( isEmpty( enemy + PossibleMoves[i] ) ){                //* Pole za wrogiem jest puste

                myTree<Move>* newTree = tree->addChild( Move(         //* Dodaje możliwy ruch
                                pawn, PossibleMoves[i],
                                field( enemy ),
                                deep, true                // true - bicie
                            ));

                const Field enemyValue = field( enemy );              //* Wykonanie ruchu
                field( enemy ) = Field::Empty;
                swap( field( pawn ), field( enemy + PossibleMoves[i] ) );

                /// - Sprawdza czy mozna kontynuowac bicie
                findBestCapture(    newTree,
                                    enemy + PossibleMoves[i],
                                    deep
                                );

                if( ! newTree->haveChild() ){
                    /// - Sprawdza odpowiedz gracza
                    findBestMove(   newTree,
                                    ! player( enemy + PossibleMoves[i] ),
                                    deep
                                );
                }

                field( enemy ) = enemyValue;                          //* Cofa ruch
                swap( field( pawn ), field( enemy + PossibleMoves[i] ) );

            }
        }
    }
}

////---------------------------------------------------------------------------------------