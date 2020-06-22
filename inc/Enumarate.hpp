#ifndef ENUMARATE_CHECKERS_HPP
#define ENUMARATE_CHECKERS_HPP

////-------------------------------------------------------------
/**
 * \brief Kod gracza
 */
enum class Player : int8_t {
    Human   =  1,       ///< Gracz
    Nobody  =  0,       ///< Żaden z graczy
    Bot     = -1        ///< Bot
};

Player operator ! ( const Player& player );
Player operator & ( const Player& player );

////-------------------------------------------------------------
/**
 * \brief Kod pola planszy
 * \note Ważna kolejność: Inne ... Puste ... Gracz ... Bot
 */
enum class Field : int8_t {
    Empty  = 0,        ///< 0000 - puste pole
    Human  = 1 << 0,   ///< 0001 - gracz
    Bot    = 1 << 1,   ///< 0010 - bot
    Pawn   = 1 << 2,   ///< 0100 - pionek
    Lady   = 1 << 3,   ///< 1000 - damka
};

Field operator | ( const Field, const Field );
Field operator & ( const Field, const Field );

////-------------------------------------------------------------
/**
 * \brief Dane ruchu
 */
enum class Data : int8_t {
    No       = 0,
    Capture  = 1 << 0,   ///< Bicie - 1, ruch - 0
    isLady   = 1 << 1,   ///< Damka - 1, pionek - 0
    BotsMove = 1 << 2    ///< bot - 1, gracz - 0
};

Data operator | ( const Data a, const Data b );
Data operator & ( const Data a, const Data b );


#endif