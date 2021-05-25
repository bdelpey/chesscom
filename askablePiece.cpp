#include "stockfish_src/uci.h"

#include "askablePiece.h"

AskablePiece::AskablePiece(Piece p, Square s, AskablePosition* pos) {
    /*
     * Set legalMoves of piece in constructor.
     */
    piece = p;
    color = color_of(p);
    currentSq = s;
    currentPos = pos;
    if (color == currentPos->side_to_move()) {
        for (const auto &o: currentPos->legalMoves) {
            if (from_sq(o.sfMove) == currentSq) {
                this->legalMoves.push_back(o);
            }
        }
    } else {
        for (const auto &m: currentPos->oppLegalMoves) {
            if (from_sq(m.sfMove) == currentSq) {
                this->legalMoves.push_back(m);
            }
        }
    }
}

string pieceToString(Piece p) {
    if (p == 1 or p == 9) {
        return string("pawn");
    } else if (p == 2 or p == 10) {
        return string("knight");
    } else if (p == 3 or p == 11) {
        return string("bishop");
    } else if (p == 4 or p == 12) {
        return string("rook");
    } else if (p == 5 or p == 13) {
        return string("queen");
    } else if (p == 6 or p == 14) {
        return string("king");
    }
    return string("none");
}

void AskablePiece::debug() const {
    cerr << (color ? "black " : "white ") << pieceToString(piece) << " " << UCI::square(currentSq) << " ";
    cerr << "legalMovesCount: " << legalMovesCount() << " ";
    cerr << "targetsCount: " << targetsCount() << " ";
    cerr << "isDefended: " << isDefended() << " ";
    cerr << "isPinned: " << isPinned() << " ";
    cerr << "isHanging: " << isHanging() << " ";
    cerr << "targetsLegalMovesCount: " << targetsLegalMovesCount() << endl;
}

size_t AskablePiece::legalMovesCount() const {
    return legalMoves.size();
}

size_t AskablePiece::targetsCount() const { // how many pieces it can captures
    size_t tarCount = 0;
    for (const auto& m: legalMoves) {
        if (currentPos->capture(m.sfMove)) {
            tarCount++;
        }
    }
    return tarCount;
}

bool AskablePiece::isPinned() const {
    /*
     * Use `Stockfish::Position::blockers_for_king`;
     */
    return currentPos->blockers_for_king(color) & currentSq;
}

bool AskablePiece::isDefended() const {
    /*
     * Intersection of attackers to current Square and our pieces;
     */
    return currentPos->attackers_to(currentSq) & currentPos->pieces(color);
}

bool AskablePiece::isHanging() const {
    /*
     * Is not defended and one or more opponent pieces attacks it.
     */
    return !isDefended() && currentPos->attackers_to(currentSq) & currentPos->pieces(~color);
}

size_t AskablePiece::targetsLegalMovesCount() const { // how many legal moves are there for all the pieces it can capture
    /*
     * Iter on all captures and sum targets legal moves.
     */

    vector<AskablePiece> considered = (color == currentPos->side_to_move() ? currentPos->theirPieces : currentPos->ourPieces);
    size_t count = 0;
    for (const auto& m: legalMoves) {
        if (currentPos->capture(m.sfMove)) {
            for (const auto& p: considered) {
                if (p.currentSq == to_sq(m.sfMove)) {
                    count += p.legalMovesCount();
                }
            }
        }
    }
    return count;
}
