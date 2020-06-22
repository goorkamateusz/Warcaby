#ifndef INTERFACE_CHECKERS_HPP
#define INTERFACE_CHECKERS_HPP

#include "Board.hpp"
#include "SFML/Graphics.hpp"

////---------------------------------------------------------------------------------------
#define GUI_MARGIN  30              ///< Narginesy planszy
#define GUI_FIELD   55              ///< Wymiary planszy
#define GUI_FOOTER  100             ///< Wysokosc stopki
#define GUI_PAWN_RADIUS 22          ///< Promien pionka
////---------------------------------------------------------------------------------------
#define BACKGROUND_IMAGE_FILE "data/gui-background.png"         ///< Plik tla
#define FONT_FILE             "data/coolvetica-rg.ttf"          ///< Plik czcionki
////---------------------------------------------------------------------------------------
#define GUI_PAWN_MARGIN (GUI_FIELD/2)-GUI_PAWN_RADIUS           ///< Margines pionka
#define GUI_BOARD_END   (GUI_FIELD*8)+GUI_MARGIN                ///< Koniec planszy
#define GUI_WIDHT       (GUI_MARGIN*2)+(GUI_FIELD*8)            ///< Szerokosc okna
#define GUI_HEIGHT      (GUI_MARGIN*2)+(GUI_FIELD*8)+GUI_FOOTER ///< Wysokosc okna
////---------------------------------------------------------------------------------------
#define BUTTON_Y        GUI_WIDHT+GUI_FOOTER-BUTTON_H-10        ///< Wsp Y przyciskow
#define BUTTON_H        30                                      ///< Wysokosc przyciskow
#define BUTTON_X        (GUI_WIDHT-BUTTON_W)/2                  ///< Wsp X przycisku "zagraj jeszcz raz"
#define BUTTON_W        160                                     ///< Szerokosc przycisku "zagraj..."
#define BUTTON_UNDO_X   387         ///< Wsp X1 przycisku cofnij
#define BUTTON_UNDO_X2  477         ///< Wsp X2 przycisku cofnij

////---------------------------------------------------------------------------------------
/**
 * \brief Model pionka na planszy
 */
class GPawn : public sf::Drawable {
private:
    sf::CircleShape shape;              ///< Ksztalt pionka

public:
    ~GPawn() = default;

    /**
     * \brief Konstruktor
     */
    GPawn();

    /**
     * \brief Przesuwa pionek do danych wspolrzednych
     * \param vec - wsp pionka na planszy
     */
    inline void move( const Vector2D& vec )
    { shape.setPosition( (vec.X*GUI_FIELD)+GUI_MARGIN+GUI_PAWN_MARGIN, (vec.Y*GUI_FIELD)+GUI_MARGIN+GUI_PAWN_MARGIN ); }

    /**
     * \brief Ustawia wartosc pionka
     * \param val -
     */
    void field( const Field val );

    /**
     * \brief Wyświetla pionka
     * \param target -
     * \param state -
     */
    inline void draw( sf::RenderTarget& target, sf::RenderStates state ) const override
    { target.draw( shape ); }

};

////---------------------------------------------------------------------------------------

/**
 * \brief Interface graficzny użytkownika
 */
class Interface : public sf::Drawable {

    sf::RenderWindow *window;                  ///< Okno
    sf::Sprite  background;                    ///< Tlo okno
    sf::Texture texBackground;                 ///< Tekstura tla okna
    sf::Font    font;                          ///< Czcionka
    sf::Text    text;                          ///< Tekst stopki
    GPawn       pawnExample;                   ///< Przykład pionka
    Vector2D    selectedPawn{Vector2D(-1,-1)}; ///< Wsp wybranego pionka

////---------------------------------------------------------------------------
public:
    Interface() = delete;
    ~Interface() = default;

    /**
     * \brief Konstruktor
     * \param mode -
     * \param title - 
     * \param style -
     */
    Interface( sf::RenderWindow *window );

    /**
     * \brief Wyświetla całą planszę.
     */
    void display( Board* const board );

    /**
     * \brief Wyświetla całe okno
     * \param target -
     * \param state -
     */
    void draw( sf::RenderTarget& target, sf::RenderStates state ) const override;

    /**
     * \brief Wyswietla komunikat
     * \param window -
     * \param txt -
     */
    inline void setText( const char* txt ){ text.setString( txt ); }

    /**
     * \brief Obsługuje opcje wywołania programu
     * \param agrc -
     * \param argv -
     */
    void loadArgs( const int agrc, const char** argv );


////---------------------------------------------------------------------------
    /**
     * \brief Ustawia wspolrzedne wybranego pionka
     * \param vec -
     */
    inline void select( const Vector2D& vec ){ selectedPawn = vec; }

    /**
     * \brief Zwraca wsp wybranego pionka
     * \return Vector2D - wybrany pionek
     */
    inline Vector2D select() const { return selectedPawn; }

    /**
     * \brief Czy zostal wybrany pionek
     * \return true -
     * \return false -
     */
    inline bool isSelected() const { return !(selectedPawn == Vector2D(-1,-1)); }

    /**
     * \brief Odznacza pionka
     */
    inline void unselect(){ selectedPawn = Vector2D(-1,-1); }

////---------------------------------------------------------------------------
    /**
     * \brief Czy wspolrzedne naleza do planszy
     * \param pos - wsp
     * \return true -
     * \return false -
     */
    inline static bool BoardCo( const sf::Vector2i& pos )
    { return( ( pos.x > GUI_MARGIN && pos.x < GUI_BOARD_END ) && ( pos.y > GUI_MARGIN && pos.y < GUI_BOARD_END ) ); }

    /**
     * \brief Czy wsporzledne naleza do przycisku "Zagraj jeszcze raz"
     * \param pos -
     * \return true -
     * \return false -
     */
    inline static bool ButtonAgain( const sf::Vector2i& pos )
    { return( ( pos.x > BUTTON_X && pos.x < BUTTON_X+BUTTON_W ) && ( pos.y > BUTTON_Y && pos.y < BUTTON_Y+BUTTON_H ) ); }


    inline static bool ButtonUndo( const sf::Vector2i& pos )
    { return( ( pos.x > BUTTON_UNDO_X && pos.x < BUTTON_UNDO_X2 ) && ( pos.y > BUTTON_Y && pos.y < BUTTON_Y+BUTTON_H ) ); }

    /**
     * \brief Przetwarza wspólrzedne okna na współrzedne pionka
     * \param vec -
     * \return Vector2D -
     */
    inline static Vector2D coordinates( const sf::Vector2i vec )
    { return Vector2D( (vec.x-GUI_MARGIN)/GUI_FIELD, ((vec.y)-GUI_MARGIN)/GUI_FIELD ); }

    /**
     * \brief Przetwarza współrzedne pionka na współrzedne okna
     * \param vec -
     * \return sf::Vector2f -
     */
    inline static sf::Vector2f coordinates( const Vector2D& vec )
    { return sf::Vector2f((vec.X*GUI_FIELD)+GUI_MARGIN, (vec.Y*GUI_FIELD)+GUI_MARGIN ); }

    /**
     * \brief Wyświetla całą plaszę i pola kopi na `cout`
     */
    static void displayDebug( const Board& );

};


#endif