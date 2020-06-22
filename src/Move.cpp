#include <Move.hpp>
#include <myTree.hpp>

////-----------------------------------------------------------------------
template<> class myTree<Move>;

////-----------------------------------------------------------------------
short int Move::captureHumanPawn = MID_CAPTURE_HUMAN_PAWN;
short int Move::captureHumanLady = MID_CAPTURE_HUMAN_LADY;
short int Move::captureBotPawn   = MID_CAPTURE_BOT_PAWN;
short int Move::captureBotLady   = MID_CAPTURE_BOT_LADY;
float     Move::deepScale        = MID_DEEP_SCALE;
uint8_t   Move::maxDeep          = MID_MAX_DEEP;

////-----------------------------------------------------------------------
short int Move::points() const {
    if( ( data & Data::Capture ) != Data::No ){         // jesli bicie
        if( ( data & Data::BotsMove ) != Data::No ) {   // jesli ruch bota
            if( ( data & Data::isLady ) != Data::No )  return captureBotLady * (1+deepScale*deep);
            else                                       return captureBotPawn * (1+deepScale*deep);
        }
        else {                                          // jesli ruch czlowieka
            if( ( data & Data::isLady ) != Data::No )  return captureHumanLady * (1+deepScale*deep);
            else                                       return captureHumanPawn * (1+deepScale*deep);
        }
    }

    return 0;
}

////-----------------------------------------------------------------------
Move::Move( const Vector2D& co_, const Vector2D& mv_, const Field val, uint8_t deep_, const bool cap ){
    co = co_;
    mv = mv_;
    if( cap ){
        data =      (( val & Field::Bot  )!=Field::Empty ? Data::BotsMove : Data::No )
                |   (( val & Field::Lady )!=Field::Empty ? Data::isLady   : Data::No )
                |   (( cap ) ? Data::Capture : Data::No );
    }
    else {
        data =      (( val & Field::Bot  )!=Field::Empty ? Data::No     : Data::BotsMove )
                |   (( val & Field::Lady )!=Field::Empty ? Data::isLady : Data::No )
                |   (( cap ) ? Data::Capture : Data::No );
    }
    deep = deep_;
}

////-----------------------------------------------------------------------
Move::Move(){
    co = Vector2D(0,0);
    mv = Vector2D(0,0);
    data = Data::No;
}

////-----------------------------------------------------------------------
std::ostream& operator << ( std::ostream& strm, const Move& mv ){
    strm << mv.co << " + " << mv.mv << " "
         << (( mv.data & Data::BotsMove  )!=Data::No   ? "Hum. " : "Bot  " )
         << (( mv.data & Data::isLady )!=Data::No   ? "Lady " : "Pawn " )
         << (( mv.data & Data::Capture )!=Data::No  ? "Capt." : "Move " )
         << " -> " << mv.points();

    return strm;
}