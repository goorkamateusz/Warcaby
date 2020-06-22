#include "Board.hpp"
#include "Interface.hpp"
#include <iostream>

using namespace std;
using namespace sf;

/**
 * \brief Glowna funkcja programu.
 *
 * \param argc - ilosc argumentow
 * \param argv - wartosci argumentow
 * \return int - kod bledu
 * \retval 0 - brak bledu
 */
int main( const int argc, const char* argv[] ){

    /// - Przygotowanie planszy
    Board board;
    board.saveCopy();

    /// - Przygotowanie okna
    RenderWindow window( VideoMode( GUI_WIDHT, GUI_HEIGHT ),
                        "Warcaby | Gorka",
                        Style::Titlebar | Style::Close );
    Event appEvent;
    Interface gui( & window );
    gui.loadArgs( argc, argv );

    bool change = true;
    bool endOfGame = false;
    bool undoLock = true;
    short int undoCount = UNDO_LIMIT;

    //! ------------------------------------------------------------------------------
    /// - Petla gry
	while( window.isOpen() ){

        /// - Obsługa zdarzeń
		while( window.pollEvent( appEvent ) ){
			if( appEvent.type == sf::Event::Closed ) window.close();

			if( sf::Mouse::isButtonPressed(sf::Mouse::Left) ){

                // Nacisniecie na plansze
                if( Interface::BoardCo( sf::Mouse::getPosition(window) ) && ! endOfGame ){

                    Vector2D tmp;
                    if( ! gui.isSelected() ){                           //* wybor pionka
                        tmp = Interface::coordinates(sf::Mouse::getPosition(window));

                        if( board.isHuman( tmp ) ) gui.select( tmp );
                    }
                    else {                                              //* wybor kierunku ruchu
                        tmp = Interface::coordinates(sf::Mouse::getPosition(window));

                        try {
                            if( board.makeHumanMove( gui.select(), tmp ) ){
                                undoLock = false;
                                gui.setText(" ");
                                board.makeBotsMove( &gui, Move::maxDeep );

                                if( board.noHumanMove() )
                                    throw "EPRZEGRANA! :( Brak ruchow gracza!";
                            }
                        }
                        catch( const char* err ){                       //* Przechwytywanie komunitaktow
                            gui.setText( ++err );
                            if( err[0] == 'E' ) endOfGame = true;
                        }

                        gui.unselect();
                    }
                }
                else {
                    if( Interface::ButtonAgain( sf::Mouse::getPosition(window) ) ){
                        board.prepare();
                        gui.loadArgs( argc, argv );
                        endOfGame = false;
                        undoCount = UNDO_LIMIT;
                        undoLock = true;
                    }
                    else if( Interface::ButtonUndo( sf::Mouse::getPosition(window)) ){
                        if( undoCount > 0 ){
                            if( ! undoLock ){
                                board.loadCopy();
                                undoLock = true;
                                --undoCount;
                                if( undoCount == 1 ) gui.setText("Pozostala ostatnia mozliwosc\ncofniecia ruchu!");
                                else gui.setText("Cofnieto ruch!");
                            }
                            else if( undoCount < UNDO_LIMIT )
                                gui.setText("Nie mozna cofnac o wiecej niz 1 ruch\nwstecz...");
                        }
                        else gui.setText("Przekroczono limit cofniec!");
                    }
                }
                change = true;
			}
		}

        if( change ){
            gui.display( & board );
            sleep( sf::milliseconds(200) );
            change = false;
        }
	}

    //! ------------------------------------------------------------------------------

    return 0;
}