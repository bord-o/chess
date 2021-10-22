#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint64_t bboard;

typedef struct move {
    int to_square;
    int from_square;
    int flags;
} move;


struct ChessBoard {
    // piece bitboards
    bboard WhitePawns;
    bboard BlackPawns;
    bboard WhiteKnights;
    bboard BlackKnights;
    bboard WhiteBishops;
    bboard BlackBishops;
    bboard WhiteRooks;
    bboard BlackRooks;
    bboard WhiteQueens;
    bboard BlackQueens;
    bboard WhiteKings;
    bboard BlackKings;
    // color bitboards
    bboard WhitePieces;
    bboard BlackPieces;
    bboard Empty;
    bboard AllPieces;
};

// helper functions
void printBoard(bboard board);
int leastSignificantBit(bboard square);
int mostSignificantBit(bboard board);
bboard clearFilesToLeft(int index); // used in diagonal vector functions to prevent wrap
bboard clearFilesToRight(int index); // used in diagonal vector functions to prevent wrap

// pseudo-legal move generation
bboard computeKingMoves(bboard kingLoc, bboard ownPieces); //color agnostic; doesn't account for pins or checks
bboard computeKnightMoves(bboard knightLoc, bboard ownPieces); //color agnostic;
bboard computePawnMovesWhite(bboard pawnLoc, bboard ownPieces, bboard enemyPieces); 
bboard computePawnMovesBlack(bboard pawnLoc, bboard ownPieces, bboard enemyPieces);
bboard computeRookMoves(bboard rookLoc, bboard ownPieces, bboard enemyPieces); //relies on precomputed vectors and a blocker check
bboard computeBishopMoves(bboard bishopLoc, bboard ownPieces, bboard enemyPieces); //relies on precomputed vectors and a blocker check
bboard computeQueenMoves(bboard queenLoc, bboard ownPieces, bboard enemyPieces);

// function to get a list of squares from a bboard result of move computation
void computeMoveList(bboard pseudo_moves, bboard loc, move *moves, int *move_counter);

// function to iterate over all square on board, computing moves for each piece type and adding
// the move lists to the global move array
void computeWhitePseudo(struct ChessBoard *BoardState, move *moves, int *move_counter);
void computeBlackPseudo(struct ChessBoard *BoardState, move *moves, int *move_counter);

// todo: create functions for moving pieces, possibly a function for getting square's current value, and setting squares
void executeMove(struct ChessBoard *BoardState, move move); // does not check for move validation
int getPieceAtIndex(struct ChessBoard *BoardState, int index);
void setPieceAtIndex(struct ChessBoard *BoardState, int index, int pieceType);

// standard vectors
void computeNorthVec(bboard *vectors);
void computeSouthVec(bboard *vectors);
void computeEastVec(bboard *vectors);
void computeWestVec(bboard *vectors);
// diagonal vectors
void computeNEastVec(bboard *vectors);
void computeSEastVec(bboard *vectors);
void computeSWestVec(bboard *vectors);
void computeNWestVec(bboard *vectors);
