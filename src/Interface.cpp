#include "Interface.hpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;
using namespace sf;

////--------------------------------------------------------------------------------
Interface::Interface( sf::RenderWindow *win ){

    /// - Otwieranie okna
    window = win;

    /// - Ładuje tlo pliku
    if( ! texBackground.loadFromFile( BACKGROUND_IMAGE_FILE ) )
        throw "EERR Blad otwierania pliku tla";

    /// - Ustawia tło
    background.setTexture( texBackground );
    background.setOrigin( 0, 0 );

    /// - Załadowanie czcionki
    if( ! font.loadFromFile( FONT_FILE ) )
        throw "EERR Blad otwierania pliku czcionki";

    /// - Zaladowanie tekstu
    text.setFont( font );
    text.setCharacterSize( 28 );
    text.setColor( Color(58,224,155) );
    text.setPosition( GUI_MARGIN, GUI_BOARD_END+GUI_MARGIN );

    text.setString( "Powodzenia!" );
}

////--------------------------------------------------------------------------------
void Interface::draw( sf::RenderTarget& target, sf::RenderStates state ) const {
    target.draw( background );
}


////--------------------------------------------------------------------------------
void Interface::display( Board* const board ){
    /// - Czyszczenie planszy
    window->clear();

    /// - Wyswietlanie planszy
    window->draw( *this );

    /// - Wyswietlanie pionkow
    for(Vector2D pawn(0,0); pawn.Y < BSIZE; ++pawn.Y )
        for( pawn.X = pawn.Y % 2; pawn.X < BSIZE; pawn.X += 2 ){

            if( board->isEmpty( pawn ) ) continue;

            if( pawn == selectedPawn ){ //jesli wybrany
                RectangleShape rect;
                rect.setFillColor( Color(58,224,155) );
                rect.setSize( Vector2f( GUI_FIELD, GUI_FIELD ) );
                rect.setPosition( coordinates( pawn ) );
                window->draw( rect );
            }

            pawnExample.move( pawn );
            pawnExample.field( board->field( pawn ) );
            window->draw( pawnExample );
        }

    window->draw( text );

    /// - Wyswietlanie
    window->display();
}


////--------------------------------------------------------------------------------
void Interface::loadArgs( const int argc, const char** argv ){
    char c = ' ';
    const char tab[] = "amd";

    if( argc == 2 )
        if( argv[1][0] == '-' ) c = argv[1][1];

wylosowano:
    switch( c ){
        case 'a':
            Move::captureHumanPawn = AGR_CAPTURE_HUMAN_PAWN;
            Move::captureHumanLady = AGR_CAPTURE_HUMAN_LADY;
            Move::captureBotPawn   = AGR_CAPTURE_BOT_PAWN;
            Move::captureBotLady   = AGR_CAPTURE_BOT_LADY;
            Move::deepScale        = AGR_DEEP_SCALE;
            Move::maxDeep          = AGR_MAX_DEEP;
            this->setText("Bot gra agresywanie");
            break;

        case 'm':
            Move::captureHumanPawn = MID_CAPTURE_HUMAN_PAWN;
            Move::captureHumanLady = MID_CAPTURE_HUMAN_LADY;
            Move::captureBotPawn   = MID_CAPTURE_BOT_PAWN;
            Move::captureBotLady   = MID_CAPTURE_BOT_LADY;
            Move::deepScale        = MID_DEEP_SCALE;
            Move::maxDeep          = MID_MAX_DEEP;
            this->setText("Bot gra zrownowazenie");
            break;

        case 'd':
            Move::captureHumanPawn = DEF_CAPTURE_HUMAN_PAWN;
            Move::captureHumanLady = DEF_CAPTURE_HUMAN_LADY;
            Move::captureBotPawn   = DEF_CAPTURE_BOT_PAWN;
            Move::captureBotLady   = DEF_CAPTURE_BOT_LADY;
            Move::deepScale        = DEF_DEEP_SCALE;
            Move::maxDeep          = DEF_MAX_DEEP;
            this->setText("Bot gra ostroznie");
            break;

        default:
            srand( time(NULL) );
            c = tab[ rand() % 3 ];
            goto wylosowano;
        }
}


////--------------------------------------------------------------------------------
////--------------------------------------------------------------------------------
////--------------------------------------------------------------------------------
GPawn::GPawn(){ shape.setRadius( GUI_PAWN_RADIUS ); }

////--------------------------------------------------------------------------------
void GPawn::field( const Field val ){
    if( ( val & Field::Bot ) != Field::Empty )
        shape.setFillColor( Color(17,152,155) );
    else
        shape.setFillColor( Color(51,102,153) );

    if( ( val & Field::Lady ) != Field::Empty ){
        shape.setOutlineThickness( 4 );
        shape.setOutlineColor( Color::Black );
    }
    else {
        shape.setOutlineThickness( 0 );
    }
}

////--------------------------------------------------------------------------------
////--------------------------------------------------------------------------------
////--------------------------------------------------------------------------------
void Interface::displayDebug( const Board& board ){

    cout << "  ";
    for( int x=0; x<BSIZE*4; ++x ) if( x % 4 == 2 ) cout << x/4; else cout << ' ';
    cout << "  |  ";
    for( int x=0; x<BSIZE*4; ++x ) if( x % 4 == 2 ) cout << x/4; else cout << ' ';
    cout << endl;

    for( int y=0; y<BSIZE; ++y ){
        // linia odstępu
        cout << "  ";
        for( int x=0; x<BSIZE*4; ++x ) if( x % 4 ) cout << '-'; else  cout << '+';
        cout << "  |  ";
        for( int x=0; x<BSIZE*4; ++x ) if( x % 4 ) cout << '-'; else  cout << '+';
        cout << '+' << endl;

        // linia danych
        cout << y << " ";
        for( int x=0; x<BSIZE; ++x ){
            cout << "| ";
            if( x % 2 == y % 2 ){
                if( board.field( x, y ) == Field::Empty ) cout << ' ';
                else {
                    if( board.isBot( Vector2D( x, y ) ) ){
                        if( board.isLady( Vector2D( x, y ) ) ) cout << "B";
                        else    cout << "b";
                    }
                    else {
                        if( board.isLady( Vector2D( x, y ) ) ) cout << "H";
                        else    cout << "h";
                    }

                }
            }
            else cout << ' ';
            cout << ' ';
        }

        cout << "  |  ";

        for( int x=0; x<BSIZE; ++x ){
            cout << "| ";
            if( board.field( x, y ) == Field::Empty ) cout << ' ';
            else cout << (int) board.field( x, y );
            cout << ' ';
        }
        cout << '|' << endl;

    }

    // linia na koniec
    cout << "  ";
    for( int x=0; x<BSIZE*4; ++x ) if( x % 4 ) cout << '-'; else  cout << '+';
    cout << "  |  ";
    for( int x=0; x<BSIZE*4; ++x ) if( x % 4 ) cout << '-'; else  cout << '+';
    cout << '+' << endl;
}

