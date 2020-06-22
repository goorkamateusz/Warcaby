/**
 * # Konfiguracja zachowań bota:
 * wzór:
 *  $$ wagaRuchu * (1 + deepScale * deep) $$
 */
#ifndef AI_CONFIG_HPP
#define AI_CONFIG_HPP

////--------------------------------------------------------------------------
#define DEF_CAPTURE_HUMAN_PAWN   10     ///< waga zbicia pionka gracza
#define DEF_CAPTURE_HUMAN_LADY   14     ///< waga zbica damki gracza
#define DEF_CAPTURE_BOT_PAWN    -12     ///< waga zbicia pionka bota
#define DEF_CAPTURE_BOT_LADY    -15     ///< waga zbicia damki gracza
#define DEF_DEEP_SCALE           0.6    ///< ulamek wplywu glebokosci rekurencji
#define DEF_MAX_DEEP   8                ///< Głębokośc rekurencji
////--------------------------------------------------------------------------
#define MID_CAPTURE_HUMAN_PAWN   10     ///< waga zbicia pionka gracza
#define MID_CAPTURE_HUMAN_LADY   13     ///< waga zbica damki gracza
#define MID_CAPTURE_BOT_PAWN    -8      ///< waga zbicia pionka bota
#define MID_CAPTURE_BOT_LADY    -12     ///< waga zbicia damki gracza
#define MID_DEEP_SCALE           0.4    ///< ulamek wplywu glebokosci rekurencji
#define MID_MAX_DEEP   6                ///< Głębokośc rekurencji
////--------------------------------------------------------------------------
#define AGR_CAPTURE_HUMAN_PAWN   10     ///< waga zbicia pionka gracza
#define AGR_CAPTURE_HUMAN_LADY   12     ///< waga zbica damki gracza
#define AGR_CAPTURE_BOT_PAWN    -5      ///< waga zbicia pionka bota
#define AGR_CAPTURE_BOT_LADY    -7      ///< waga zbicia damki gracza
#define AGR_DEEP_SCALE           0.1    ///< ulamek wplywu glebokosci rekurencji
#define AGR_MAX_DEEP   4                ///< Głębokośc rekurencji

////--------------------------------------------------------------------------
#define UNDO_LIMIT 3                ///< Limit cofnięć na gre!

////--------------------------------------------------------------------------
#define DEBUG_TEST 0               ///< Stopien komunikatu o ruchach (0-5)


#endif
