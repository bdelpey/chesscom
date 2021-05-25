#pragma once

#include "askablePosition.h"
#include "askableMove.h"

using namespace Stockfish;
using namespace std;

class AskablePosition;
class AskableMove;

class AskablePiece {
	public:
		AskablePosition* currentPos;
		Square currentSq;
		vector<AskableMove> legalMoves;
		Piece piece;
		Color color;

		AskablePiece(Piece piece, Square sq, AskablePosition* currentPos);

		bool isDefended() const;

		void debug() const;

		// Suggested functions
		size_t legalMovesCount() const;
		size_t targetsCount() const; // how many pieces it can captures
		bool isPinned() const;
		bool isHanging() const;
		size_t targetsLegalMovesCount() const; // how many legal moves are there for all the pieces it can capture
};
