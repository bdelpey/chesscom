#include "stockfish_src/thread.h"
#include "stockfish_src/uci.h"


#include "askablePosition.h"
#include "askableMove.h"

using namespace std;
using namespace Stockfish;

double to_cp(Value v) { return double(v) / PawnValueEg; }

void AskablePosition::setup(const string& fen) {
    /*
     * Set all legal moves of the position.
     * Set all pieces of the position.
     */
    StateListPtr states(new std::deque<StateInfo>(1));
	this->set(fen, false, &states->back(), Threads.main());

    for (const auto& m : MoveList<LEGAL>(*this)) {
        legalMoves.push_back(AskableMove(m.move, this));
    }
    StateInfo state;
    this->do_null_move(state);
    for (const auto& m : MoveList<LEGAL>(*this)) {
        oppLegalMoves.push_back(AskableMove(m.move, this));
    }
    this->undo_null_move();

    for (Square i = SQ_A1; i < SQUARE_NB; ++i) {
        if (piece_on(i)) {
            AskablePiece p(piece_on(i), i, this);
            allPieces.push_back(p);
            if (p.color == this->side_to_move()) {
                ourPieces.push_back(p);
            } else {
                theirPieces.push_back(p);
            }
        }
    }
}

string AskablePosition::getReverse() const {
    string reversedPos = this->fen();
    if (reversedPos.find(" w ") != string::npos) {
        reversedPos.replace(reversedPos.find(" w "), 3, " b ");
    } else {
        reversedPos.replace(reversedPos.find(" b "), 3, " w ");
    }
    return reversedPos;
}

void AskablePosition::limitedSearch(Search::LimitsType limits) {
    limits.depth = 14;
    limits.startTime = now();
    string current = this->fen();
    bool ponderMode = false;
    Search::clear();

    StateListPtr states(new std::deque<StateInfo>(1));
    this->set(current, false, &states->back(), Threads.main());

    Threads.start_thinking(*this, states, limits, ponderMode);
    Threads.main()->wait_for_search_finished();

    // reseting halfmove component of fen string
    states = StateListPtr(new std::deque<StateInfo>(1));
    this->set(current, false, &states->back(), Threads.main());
}

vector<AskableMove> AskablePosition::getNBestMoves(size_t N) {
    /*
     * The idea here is to take advantage of the MultiPV option of Stockfish.
     * Set this option to the number N of best moves we want and start a search.
     * Returns of vector of `AskableMove`.
     */
    Options["MultiPV"] = to_string(N);
    this->limitedSearch();

    vector<AskableMove> NBestMoves;
    for (size_t i = 0; i < N; ++i) {
        Move m = Threads.main()->rootMoves[i].pv[0];
        NBestMoves.push_back(AskableMove(m, this));
    }
    return NBestMoves;
}

vector<AskableMove> AskablePosition::getThreatMoves(int ctThreshold) {
    /*
     * The caller pass the centipawns threshold as argument.
     * The idea here is to change side to play before a search and return opponents
     * moves which give an advantage larger than the threshold from current evaluation.
     */

    vector<AskableMove> threats;
    if (checkers()) {
        return threats; // checkers need to be deal with. Maybe return checkers to highlight them ?
    }

    // get current position eval
    Options["MultiPV"] = to_string(1);
    this->limitedSearch();
    int refScore = this->this_thread()->rootMoves[0].score;

    // get 3 best opp moves
    Options["MultiPV"] = to_string(3);
    string reversed = this->getReverse();
    string current = this->fen();
    StateListPtr states(new std::deque<StateInfo>(1));
    this->set(reversed, false, &states->back(), Threads.main());
    this->limitedSearch();

    // return those which enable the opponent to take the lead
    for (size_t i = 0; i < 3; ++i) {
        if (this->this_thread()->rootMoves[i].score > -refScore + ctThreshold) { // opposite refScore because we change maximising player
            Move m = this->this_thread()->rootMoves[i].pv[0];
            threats.push_back(AskableMove(m, this));
        }
    }
    // reset pos
    states = StateListPtr (new std::deque<StateInfo>(1));
    this->set(current, false, &states->back(), Threads.main());

    return threats;
}
