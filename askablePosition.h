#pragma once

#include "stockfish_src/position.h"
#include "stockfish_src/search.h"

#include "askableMove.h"
#include "askablePiece.h"

using namespace std;
using namespace Stockfish;

class AskableMove;
class AskablePiece;

class AskablePosition : public Position {
	public:
		vector<AskablePiece> allPieces;
		vector<AskablePiece> ourPieces;
		vector<AskablePiece> theirPieces;
		vector<AskableMove> legalMoves;
		vector<AskableMove> oppLegalMoves;

		void setup(const string& fen);

        string getReverse() const;
        void limitedSearch(Search::LimitsType limits = Search::LimitsType());

		// Suggested functions
		vector<AskableMove> getNBestMoves(size_t N);
		vector<AskableMove> getThreatMoves(int ctThreshold);
};

