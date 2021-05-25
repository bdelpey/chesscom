#pragma once

#include "askablePosition.h"
#include "askablePiece.h"

using namespace Stockfish;

class AskablePosition;
class AskablePiece;

class AskableMove {
	public:
	    Move sfMove;
		AskablePosition* from;
		double score;

		AskableMove(Move, AskablePosition*);
		void debug() const;

		// Suggested functions
		int centipawnsLost() const;
		bool captureHangingPiece() const;
		bool isSEEPositiveCapture() const;

		/*
		bool allowOppSEEPositiveCapture();
		bool allowOppGoodMove();
		bool allowOppGoodMoveUndefending();
		*/
};
