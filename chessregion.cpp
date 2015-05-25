#include "chessregion.h"

ChessRegion::ChessRegion()
{
    for (int i = 0; i <MAX_BOUND; ++i){
        for (int j = 0; j <MAX_BOUND; ++j){
            hasPiece[i][j] = NON_PIECE;
        }
    }
}

ChessRegion::~ChessRegion()
{

}

