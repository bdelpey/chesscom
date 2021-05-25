#include <sstream>

#include "stockfish_src/endgame.h"
#include "stockfish_src/search.h"
#include "stockfish_src/thread.h"
#include "stockfish_src/uci.h"

#include "askablePosition.h"

using namespace Stockfish;

const char* StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

const char* TestFEN = "r5k1/1bq1br1p/p1p1Q1p1/1p1n4/5P2/1P1B3R/PBPP2PP/R5K1 w - - 1 19";

void testBestNMoves(AskablePosition& pos, size_t numberOfMoves) {
    cerr << "=== TESTING BestNMoves ===" << endl;
    for (const auto&m: pos.getNBestMoves(numberOfMoves)) {
        m.debug();
    }
}

void testThreats(AskablePosition& pos, int ctThreshold) { 
    cerr << "=== TESTING Threats ===" << endl;
    for (const auto& m: pos.getThreatMoves(ctThreshold)) {
        m.debug();
    }
}

void testMoves(AskablePosition& pos) {
    cerr << "=== TESTING Moves ===" << endl;
    for (const auto& m: pos.legalMoves) {
        m.debug();
    }
}

void testPieces(AskablePosition& pos) {
    cerr << "=== TESTING Pieces ===" << endl;
    for (const auto&p: pos.allPieces) {
        p.debug();
    }
}

int main(int argc, char *argv[]) {
    std::cout << engine_info() << std::endl;

    CommandLine::init(argc, argv);
    UCI::init(Options);
    Tune::init();
    PSQT::init();
    Bitboards::init();
    Position::init();
    Bitbases::init();
    Endgames::init();
    Threads.set(size_t(Options["Threads"]));
    Search::clear(); // After threads are up
    Eval::NNUE::init();

    AskablePosition pos;
    if (argc > 1) {
        pos.setup(string(argv[1]));
    } else {
        pos.setup(string(TestFEN));
    }
    cerr << pos << endl;;

    cerr << "reversed fen: " << pos.getReverse() << endl; // other side to play
    cerr << "legalMovesCount: " << pos.legalMoves.size() << endl;
    cerr << "we have " << pos.ourPieces.size() << " pieces" << endl;
    cerr << "opp has " << pos.theirPieces.size() << " pieces" << endl;

    // test AskablePosition
    testBestNMoves(pos, 3);
    testThreats(pos, 100);

    // test AskableMove
    testMoves(pos);
    
    // test AskablePiece
    testPieces(pos);

    Threads.set(0);
    return 0;
}
