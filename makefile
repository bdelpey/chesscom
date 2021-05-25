task: objs
	g++ -o task askablePosition.o askablePiece.o askableMove.o stockfish_src/benchmark.o stockfish_src/bitbase.o stockfish_src/bitboard.o stockfish_src/endgame.o stockfish_src/evaluate.o main.o stockfish_src/material.o stockfish_src/misc.o stockfish_src/movegen.o stockfish_src/movepick.o stockfish_src/pawns.o stockfish_src/position.o stockfish_src/psqt.o stockfish_src/search.o stockfish_src/thread.o stockfish_src/timeman.o stockfish_src/tt.o stockfish_src/uci.o stockfish_src/ucioption.o stockfish_src/tune.o stockfish_src/tbprobe.o stockfish_src/evaluate_nnue.o stockfish_src/half_ka_v2.o -m64 -Wl,--no-as-needed -lpthread -Wall -Wcast-qual -fno-exceptions -std=c++17 -pedantic -Wextra -Wshadow -m64 -DUSE_PTHREADS -DNDEBUG -O3 -DIS_64BIT -msse -DUSE_SSE2 -msse2 -flto -flto=jobserver
#objs: build_stockfish
objs:
	g++ -Wall -Wcast-qual -fno-exceptions -std=c++17  -pedantic -Wextra -Wshadow -m64 -DUSE_PTHREADS -DNDEBUG -O3 -DIS_64BIT -msse -DUSE_SSE2 -msse2 -flto -c *.cpp
build_stockfish:
	cd stockfish_src ;make clean ;make -j build ARCH=x86-64
clean:
	rm *.o task
