#include "stockfish_src/uci.h"
#include "stockfish_src/thread.h"
#include "stockfish_src/search.h"
#include "stockfish_src/position.h"

#include "askableMove.h"
#include "askablePiece.h"
#include "askablePosition.h"

using namespace Stockfish;

AskableMove::AskableMove(Move m, AskablePosition* posFrom) {
    this->sfMove = m;
    this->from = posFrom;
}

void AskableMove::debug() const {
    cerr << UCI::move(this->sfMove, false);

    if (color_of(from->piece_on(from_sq(sfMove))) == from->side_to_move()) {cerr << " ctLost: " << this->centipawnsLost(); }
    cerr << " capturing hanging piece: " << this->captureHangingPiece();
    cerr << " isSEEPositiveCapture: " << this->isSEEPositiveCapture() << endl;
}

int AskableMove::centipawnsLost() const {
    /*
     * caveat : doesn't work well with castling moves.
     */
    Options["MultiPV"] = to_string(1);

    AskableMove bestMove = from->getNBestMoves(1)[0];

    if (bestMove.sfMove == this->sfMove) {
        return 0;
    }
    int bestScore = from->this_thread()->rootMoves[0].score;

    Search::LimitsType limits;
    limits.searchmoves.push_back(this->sfMove);
    from->limitedSearch(limits);

    return bestScore - from->this_thread()->rootMoves[0].score;
}

bool AskableMove::captureHangingPiece() const {
    if (from->capture(sfMove)) {
        AskablePiece p = AskablePiece(from->piece_on(to_sq(sfMove)), to_sq(sfMove), from);
        return p.isHanging();
    }
    return false;
}

bool AskableMove::isSEEPositiveCapture() const {
    /*
     * Simply use Stockfish `Position` method with value 1 because Stockfish method
     * returns true if greater or equal and use 0 as default threshold.
     */
	return from->see_ge(this->sfMove, Value(1));
}

/*
bool AskableMove::allowOppSEEPositiveCapture() {
}
bool AskableMove::allowOppGoodMove() {
}
bool AskableMove::allowOppGoodMoveUndefending() {
}
*/
