# Files

I use whole Stockfish base code not to worry about dependencies.
The sf sources are isolated in the stockfish_src folder.

My main.cpp is a copy/past from the begining of sf's to init classes and then I test my objects in it.

My objects are `AskablePosition`, `AskableMove` and `AskablePiece` split in .cpp and .h files.

# Test
First, compile stockfish.
`cd stockfish_src && make build ARCH=x86-64-modern && cd ..`
Then, compile the askable objects.
`make`

To test without getting stockfish output:
`./task 2>&1 > /dev/null` will test the objects with an hard coded FEN string.
Or you can past a custom FEN string as argument:
`./task "r3r1k1/pb1q2pp/1p1p1pn1/1N1P1Q1R/2PB2Pb/P7/1P2B3/5RK1 b - - 3 26" 2>&1 > /dev/null`
