#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint64_t bboard;

typedef struct move {
    uint8_t to_square;
    uint8_t from_square;
    uint8_t flags;
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

// todo: create function to get a list of squares from a bboard result of move computation
void computeMoveList(bboard pseudo_moves, move *moves, uint8_t *move_counter);

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
