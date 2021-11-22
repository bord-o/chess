#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "chess.h"

struct ChessBoard BoardState = {

    .WhitePawns = 0xff00,
    .BlackPawns = 0xff000000000000,
    .WhiteKnights = 0x42,
    .BlackKnights = 0x4200000000000000,
    .WhiteBishops = 0x24,
    .BlackBishops = 0x2400000000000000,
    .WhiteRooks = 0x81,
    .BlackRooks = 0x8100000000000000,
    .WhiteQueens = 0x8,
    .BlackQueens = 0x800000000000000,
    .WhiteKings = 0x10,
    .BlackKings = 0x1000000000000000,

    .WhitePieces = 0xffff,
    .BlackPieces = 0xffff000000000000,

    .Empty = 0xffffffff0000,
    .AllPieces = 0xffff00000000ffff,

    .side = 1
};

// defines for distiguishing piece types
#define WPAWN 1
#define WKNIGHT 2
#define WBISHOP 3
#define WROOK 4
#define WQUEEN 5
#define WKING 6

#define BPAWN -1
#define BKNIGHT -2
#define BBISHOP -3
#define BROOK -4
#define BQUEEN -5
#define BKING -6

//lookup tables for clearing and masking ranks and files
enum Ranks {
    RANK_1 = 0,
    RANK_2 = 1,
    RANK_3 = 2,
    RANK_4 = 3,
    RANK_5 = 4,
    RANK_6 = 5,
    RANK_7 = 6,
    RANK_8 = 7,
};

enum Files {
    FILE_A = 0,
    FILE_B = 1,
    FILE_C = 2,
    FILE_D = 3,
    FILE_E = 4,
    FILE_F = 5,
    FILE_G = 6,
    FILE_H = 7
};

enum Squares {
    A1 = (uint64_t)1,
    B1 = (uint64_t)1 << 1,
    C1 = (uint64_t)1 << 2,
    D1 = (uint64_t)1 << 3,
    E1 = (uint64_t)1 << 4,
    F1 = (uint64_t)1 << 5,
    G1 = (uint64_t)1 << 6,
    H1 = (uint64_t)1 << 7,

    A2 = (uint64_t)256 ,
    B2 = (uint64_t)256 << 1,
    C2 = (uint64_t)256 << 2,
    D2 = (uint64_t)256 << 3,
    E2 = (uint64_t)256 << 4,
    F2 = (uint64_t)256 << 5,
    G2 = (uint64_t)256 << 6,
    H2 = (uint64_t)256 << 7,

    A3 = (uint64_t)65536 ,
    B3 = (uint64_t)65536 << 1,
    C3 = (uint64_t)65536 << 2,
    D3 = (uint64_t)65536 << 3,
    E3 = (uint64_t)65536 << 4,
    F3 = (uint64_t)65536 << 5,
    G3 = (uint64_t)65536 << 6,
    H3 = (uint64_t)65536 << 7,

    A4 = (uint64_t)16777216 ,
    B4 = (uint64_t)16777216 << 1,
    C4 = (uint64_t)16777216 << 2,
    D4 = (uint64_t)16777216 << 3,
    E4 = (uint64_t)16777216 << 4,
    F4 = (uint64_t)16777216 << 5,
    G4 = (uint64_t)16777216 << 6,
    H4 = (uint64_t)16777216 << 7,

    A5 = (uint64_t)4294967296 ,
    B5 = (uint64_t)4294967296 << 1,
    C5 = (uint64_t)4294967296 << 2,
    D5 = (uint64_t)4294967296 << 3,
    E5 = (uint64_t)4294967296 << 4,
    F5 = (uint64_t)4294967296 << 5,
    G5 = (uint64_t)4294967296 << 6,
    H5 = (uint64_t)4294967296 << 7,
    
    A6 = (uint64_t)0x10000000000 ,
    B6 = (uint64_t)0x10000000000 << 1,
    C6 = (uint64_t)0x10000000000 << 2,
    D6 = (uint64_t)0x10000000000 << 3,
    E6 = (uint64_t)0x10000000000 << 4,
    F6 = (uint64_t)0x10000000000 << 5,
    G6 = (uint64_t)0x10000000000 << 6,
    H6 = (uint64_t)0x10000000000 << 7,

    A7 = (uint64_t)0x1000000000000 ,
    B7 = (uint64_t)0x1000000000000 << 1,
    C7 = (uint64_t)0x1000000000000 << 2,
    D7 = (uint64_t)0x1000000000000 << 3,
    E7 = (uint64_t)0x1000000000000 << 4,
    F7 = (uint64_t)0x1000000000000 << 5,
    G7 = (uint64_t)0x1000000000000 << 6,
    H7 = (uint64_t)0x1000000000000 << 7,

    A8 = (uint64_t)0x100000000000000 ,
    B8 = (uint64_t)0x100000000000000 << 1,
    C8 = (uint64_t)0x100000000000000 << 2,
    D8 = (uint64_t)0x100000000000000 << 3,
    E8 = (uint64_t)0x100000000000000 << 4,
    F8 = (uint64_t)0x100000000000000 << 5,
    G8 = (uint64_t)0x100000000000000 << 6,
    H8 = (uint64_t)0x100000000000000 << 7,
};

// precalculated U64s for masking and clearing files
const bboard clearRank[8] = {
    0xffffffffffffff00,
    0xffffffffffff00ff,
    0xffffffffff00ffff,
    0xffffffff00ffffff,
    0xffffff00ffffffff,
    0xffff00ffffffffff,
    0xff00ffffffffffff,
    0x00ffffffffffffff,
};

const bboard clearFile[8] = {
    0xfefefefefefefefe,
    0xfdfdfdfdfdfdfdfd,
    0xfbfbfbfbfbfbfbfb,
    0xf7f7f7f7f7f7f7f7,
    0xefefefefefefefef,
    0xdfdfdfdfdfdfdfdf,
    0xbfbfbfbfbfbfbfbf,
    0x7f7f7f7f7f7f7f7f
};

const bboard maskRank[8] = {
    0xff,
    0xff00,
    0xff0000,
    0xff000000,
    0xff00000000,
    0xff0000000000,
    0xff000000000000,
    0xff00000000000000
};

const bboard maskFile[8] = {
    0x101010101010101,
    0x202020202020202,
    0x404040404040404,
    0x808080808080808,
    0x1010101010101010,
    0x2020202020202020,
    0x4040404040404040,
    0x8080808080808080
};
static bboard northVec[64];
static bboard southVec[64];
static bboard eastVec[64];
static bboard westVec[64];

static bboard neastVec[64];
static bboard seastVec[64];
static bboard swestVec[64];
static bboard nwestVec[64];


int main(void) {
    
    computeNorthVec(northVec);
    computeSouthVec(southVec);
    computeEastVec(eastVec);
    computeWestVec(westVec);

    computeNEastVec(neastVec);
    computeSEastVec(seastVec);
    computeSWestVec(swestVec);
    computeNWestVec(nwestVec);

    
    
   

    //printBoard(0x1000220000002008);
    //printf("least: %i -- most: %i\n", leastSignificantBit(0x1000220000002008), mostSignificantBit(0x1000220000002008));
    //printBoard(0x1000220000002008 & ~(bboard)pow(2,60));
    
    srand(time(NULL)); // get random seed for tests

    move p_moves[218]; //create and initialize the test move array to zero
    move l_moves[218];
    for (int i=0; i < 218; i++) {
        p_moves[i].from_square = 0;
        p_moves[i].to_square = 0;
        p_moves[i].flags = 0;

        l_moves[i].from_square = 0;
        l_moves[i].to_square = 0;
        l_moves[i].flags = 0;

    }
    int p_counter = 0;
    int l_counter = 0;

    move test_moves[] = {
        { .from_square = 1, .to_square = 18},
        { .from_square = 51, .to_square = 35},
        { .from_square = 18, .to_square = 35},
        { .from_square = 59, .to_square = 35}
    };

    //loop to test random move generation and validation...
    for (int i = 0; i < 300; i++) {

        if (BoardState.side == 1) {
            computeWhitePseudo(&BoardState, p_moves, &p_counter);
        } else {
            computeBlackPseudo(&BoardState, p_moves, &p_counter);
        }

        validateMoveList(&BoardState, p_moves, &p_counter, l_moves, &l_counter, 1);

        //rand() % (max_number + 1 - minimum_number) + minimum_number
        int x = rand() % (l_counter + 1 - 0) + 0;
        printf("%i: %i / %i\n", BoardState.side, x, l_counter);

        executeMove(&BoardState, l_moves[x]);

        // after executing move, zero the move buffer and counter
        BoardState.side = !BoardState.side;
        p_counter = 0;
        l_counter = 0;

        for (int i=0; i < 218; i++) {
            p_moves[i].from_square = 0;
            p_moves[i].to_square = 0;
            p_moves[i].flags = 0;

            l_moves[i].from_square = 0;
            l_moves[i].to_square = 0;
            l_moves[i].flags = 0;

        }
    }
    printBoard(BoardState.AllPieces);
    printBoard(BoardState.BlackPawns);
    printBoard(BoardState.WhitePawns);

    /*
    for (int i = 0; i < 4; i++) { 
        executeMove(&BoardState, test_moves[i]);
    }


    setPieceAtIndex(&BoardState, 1, 0);
    printBoard(BoardState.AllPieces);
    printBoard(BoardState.WhiteKnights);
    */

    return 0;
}
void executeMove(struct ChessBoard *BoardState, move move) {
    int to = move.to_square;
    int from = move.from_square;
    int frompiece = getPieceAtIndex(BoardState, from);
    setPieceAtIndex(BoardState, from, 0);
    setPieceAtIndex(BoardState, to, 0);
    setPieceAtIndex(BoardState, to, frompiece);

}

int getPieceAtIndex(struct ChessBoard *BoardState, int index){
    
    bboard current_square =  ((bboard) 1 << (bboard)index);

    if (current_square & BoardState->WhitePawns) {
        return WPAWN;
    }
    else if (current_square & BoardState->WhiteKnights) {
        return WKNIGHT;
    }
    else if (current_square & BoardState->WhiteBishops) {
        return WBISHOP;
    }
    else if (current_square & BoardState->WhiteRooks) {
        return WROOK;
    }
    else if (current_square & BoardState->WhiteQueens) {
        return WQUEEN;
    }
    else if (current_square & BoardState->WhiteKings) {
        return WKING;
    }

    else if (current_square & BoardState->BlackPawns) {
        return BPAWN;
    }
    else if (current_square & BoardState->BlackKnights) {
        return BKNIGHT;
    }
    else if (current_square & BoardState->BlackBishops) {
        return BBISHOP;
    }
    else if (current_square & BoardState->BlackRooks) {
        return BROOK;
    }
    else if (current_square & BoardState->BlackQueens) {
        return BQUEEN;
    }
    else if (current_square & BoardState->BlackKings) {
        return BKING;
    }
    else return 0;
}

void setPieceAtIndex(struct ChessBoard *BoardState, int index, int pieceType) {
    // todo
    bboard loc = (bboard)1 << (bboard)index; // get bitboard version of location
    switch (pieceType) {
        case 0:
            BoardState->Empty |= loc;
            BoardState->WhiteKnights &= ~(BoardState->WhiteKnights & BoardState->Empty);
            BoardState->WhitePawns&= ~(BoardState->WhitePawns& BoardState->Empty);
            BoardState->WhiteBishops&= ~(BoardState->WhiteBishops& BoardState->Empty);
            BoardState->WhiteRooks&= ~(BoardState->WhiteRooks& BoardState->Empty);
            BoardState->WhiteKings&= ~(BoardState->WhiteKings& BoardState->Empty);
            BoardState->WhiteQueens&= ~(BoardState->WhiteQueens& BoardState->Empty);

            BoardState->BlackKnights &= ~(BoardState->BlackKnights & BoardState->Empty);
            BoardState->BlackPawns&= ~(BoardState->BlackPawns& BoardState->Empty);
            BoardState->BlackBishops&= ~(BoardState->BlackBishops& BoardState->Empty);
            BoardState->BlackRooks&= ~(BoardState->BlackRooks& BoardState->Empty);
            BoardState->BlackKings&= ~(BoardState->BlackKings& BoardState->Empty);
            BoardState->BlackQueens&= ~(BoardState->BlackQueens& BoardState->Empty);
            break;
        case BPAWN:
            BoardState->BlackPawns |= loc;
            BoardState->Empty &= ~loc;
            BoardState->BlackPawns &= ~(BoardState->Empty & BoardState->BlackPawns);
            break;
        case BKNIGHT:
            BoardState->BlackKnights|= loc;
            BoardState->Empty &= ~loc;
            BoardState->BlackKnights &= ~(BoardState->Empty & BoardState->BlackKnights);
            break;
        case BBISHOP:
            BoardState->BlackBishops|= loc;
            BoardState->Empty &= ~loc;
            BoardState->BlackBishops &= ~(BoardState->Empty & BoardState->BlackBishops);
            break;
        case BROOK:
            BoardState->BlackRooks|= loc;
            BoardState->Empty &= ~loc;
            BoardState->BlackRooks &= ~(BoardState->Empty & BoardState->BlackRooks);
            break;
        case BQUEEN:
            BoardState->BlackQueens |= loc;
            BoardState->Empty &= ~loc;
            BoardState->BlackQueens &= ~(BoardState->Empty & BoardState->BlackQueens);
            break;
        case BKING:
            BoardState->BlackKings|= loc;
            BoardState->Empty &= ~loc;
            BoardState->BlackKings &= ~(BoardState->Empty & BoardState->BlackKings);
            break;

        case WPAWN:
            BoardState->WhitePawns|= loc;
            BoardState->Empty &= ~loc;
            BoardState->WhitePawns &= ~(BoardState->Empty & BoardState->WhitePawns);
            break;
        case WKNIGHT:
            BoardState->WhiteKnights|= loc;
            BoardState->Empty &= ~loc;
            BoardState->WhiteKnights &= ~(BoardState->Empty & BoardState->WhiteKnights);
            break;
        case WBISHOP:
            BoardState->WhiteBishops|= loc;
            BoardState->Empty &= ~loc;
            BoardState->WhiteBishops &= ~(BoardState->Empty & BoardState->WhiteBishops);
            break;
        case WROOK:
            BoardState->WhiteRooks|= loc;
            BoardState->Empty &= ~loc;
            BoardState->WhiteRooks &= ~(BoardState->Empty & BoardState->WhiteRooks);
            break;
        case WQUEEN:
            BoardState->WhiteQueens |= loc;
            BoardState->Empty &= ~loc;
            BoardState->WhiteQueens &= ~(BoardState->Empty & BoardState->WhiteQueens);
            break;
        case WKING:
            BoardState->WhiteKings|= loc;
            BoardState->Empty &= ~loc;
            BoardState->WhiteKings &= ~(BoardState->Empty & BoardState->WhiteKings);
            break;
    }
    BoardState->WhitePieces = BoardState->WhitePawns|BoardState->WhiteKnights|BoardState->WhiteBishops|BoardState->WhiteRooks|BoardState->WhiteQueens|BoardState->WhiteKings;
    //BoardState->WhitePieces &= ~(BoardState->Empty & BoardState->WhitePieces);
    BoardState->BlackPieces = (BoardState->BlackPawns|BoardState->BlackKnights|BoardState->BlackBishops|BoardState->BlackRooks|BoardState->BlackQueens|BoardState->BlackKings);
    //BoardState->BlackPieces &= ~(BoardState->Empty & BoardState->BlackPieces);

    BoardState->AllPieces = (BoardState->WhitePieces | BoardState->BlackPieces);
}

void computeNWestVec(bboard *vectors) {
    bboard startVec = 0x2040810204080; //Starting at A1
    for (int i = 0; i < 64; i++) {
        vectors[i] = startVec & clearFilesToRight(i);
        startVec <<= 1;
    }
}

void computeSWestVec(bboard *vectors) {
    bboard startVec = 0x40201008040201;
    for (int i = 63; i > -1; i--) {
        vectors[i] = startVec & clearFilesToRight(i);
        startVec >>= 1;
    }
    
}

void computeSEastVec(bboard *vectors) {
    bboard startVec = 0x102040810204000; //starting from H8
    for (int i = 63; i > -1; i--) {
        vectors[i] = startVec & clearFilesToLeft(i);
        startVec >>= 1;
    }
}

void computeNEastVec(bboard *vectors) {
    bboard startVec = 0x8040201008040200; //starting from A1; diag vector
    for (int i = 0; i < 64; i++) {
        vectors[i] = startVec & clearFilesToLeft(i);
        startVec <<= 1;
    }
}

void computeWestVec(bboard *vectors) {
    bboard startVec = 0x7f00000000000000; // starting from H8
    int rank_to_clear = 6; // initially clear the 7th rank

    for (int i = 63; i > -1; i--) {
        if (rank_to_clear > -1) {
            vectors[i] = startVec & clearRank[rank_to_clear];
            if (i % 8 == 0) {
                rank_to_clear--;
            }
        }
        else {
            vectors[i] = startVec;
        }

        startVec >>= 1;
    }
}

void computeEastVec(bboard *vectors) {
    bboard startVec = 0xfe; //starting from A1
    int rank_to_clear = 1; // initially clear the second rank (zero indexed)

    for (int i = 0; i < 64; i++) {

        vectors[i] = startVec & clearRank[rank_to_clear];
        if ((i + 1) % 8 == 0) {
            rank_to_clear++;
        }
        startVec <<= 1;
    }
}

void computeSouthVec(bboard *vectors) {
    bboard startVec = 0x80808080808080; //south vector for H8
    for (int i = 63; i > -1; i--) {
        vectors[i] = startVec;
        startVec >>= 1;
    }
}
    
void computeNorthVec(bboard *vectors) {
    bboard startVec = 0x101010101010100; //north vector for A1
    for (int i = 0; i < 64; i++) {
        vectors[i] = startVec;
        startVec <<= 1; //shift the vector by one through all squares
    }
}

bboard computeQueenMoves(bboard queenLoc, bboard ownPieces, bboard enemyPieces) {
    bboard diags = computeBishopMoves(queenLoc, ownPieces, enemyPieces);
    bboard straights = computeRookMoves(queenLoc, ownPieces, enemyPieces);

    bboard allMoves = diags | straights;
    return allMoves;
}

bboard computeBishopMoves(bboard bishopLoc, bboard ownPieces, bboard enemyPieces) {
    bboard ne = neastVec[leastSignificantBit(bishopLoc)];
    bboard nw = nwestVec[leastSignificantBit(bishopLoc)];
    bboard sw = swestVec[leastSignificantBit(bishopLoc)];
    bboard se = seastVec[leastSignificantBit(bishopLoc)];

    // own piece calc
    bboard own_ne = ne & ownPieces;
    //last part blocks the bishop from attacking own piece
    bboard own_ne_blocked = neastVec[leastSignificantBit(own_ne)] | own_ne; 
    bboard own_nw = nw & ownPieces;
    bboard own_nw_blocked = nwestVec[leastSignificantBit(own_nw)] | own_nw;
    bboard own_sw= sw & ownPieces;
    bboard own_sw_blocked = swestVec[mostSignificantBit(own_sw)] | own_sw;
    bboard own_se= se & ownPieces;
    bboard own_se_blocked = seastVec[mostSignificantBit(own_se)] | own_se;

    bboard movesAfterBlockingOwn = (ne & ~own_ne_blocked) | (nw & ~own_nw_blocked)
        | (sw & ~own_sw_blocked) | (se & ~own_se_blocked);

    // enemy piece calc
    bboard enem_ne = ne & enemyPieces;
    bboard enem_ne_blocked = neastVec[leastSignificantBit(enem_ne)]; 
    bboard enem_nw = nw & enemyPieces;
    bboard enem_nw_blocked = nwestVec[leastSignificantBit(enem_nw)];
    bboard enem_sw= sw & enemyPieces;
    bboard enem_sw_blocked = swestVec[mostSignificantBit(enem_sw)]; 
    bboard enem_se= se & enemyPieces;
    bboard enem_se_blocked = seastVec[mostSignificantBit(enem_se)]; 

    bboard movesAfterBlockingEnemy = (ne & ~enem_ne_blocked) | (nw & ~enem_nw_blocked)
        | (sw & ~enem_sw_blocked) | (se & ~enem_se_blocked);

    bboard allMoves = movesAfterBlockingEnemy & movesAfterBlockingOwn;
    return allMoves;
}

bboard computeRookMoves(bboard rookLoc, bboard ownPieces, bboard enemyPieces) {
    //todo:
    // make functions to initialize rays for each piece and square, then impliment the
    // blocking logic in each piece's move compute function.
    //
    // for the rook I need north, south, east, and west rays for empty board
    // to get valid moves I use bitwise ops to eliminate squares that are behind friendly
    // and enemy pieces. I need to make sure that in the case of an enemy blocking piece,
    // I incude the enemy's square in the attacks of the rook.
    //
    // after makeing the ray initialization functions, I might hardcode the values like i did for file masks.
    bboard e = eastVec[leastSignificantBit(rookLoc)];
    bboard w = westVec[leastSignificantBit(rookLoc)];
    bboard n = northVec[leastSignificantBit(rookLoc)]; 
    bboard s = southVec[leastSignificantBit(rookLoc)];

    // own piece calculation
    bboard own_e = e & ownPieces;
    bboard e_blocked = eastVec[leastSignificantBit(own_e)] | own_e;
    bboard own_w = w & ownPieces;
    bboard w_blocked = westVec[mostSignificantBit(own_w)] | own_w;
    bboard own_n = n & ownPieces;
    bboard n_blocked = northVec[leastSignificantBit(own_n)]  | own_n;
    bboard own_s = s & ownPieces;
    bboard s_blocked = southVec[mostSignificantBit(own_s)] | own_s;

    bboard movesAfterBlockingOwn = (e & ~e_blocked) | (w & ~w_blocked)
        | (n & ~n_blocked) | (s & ~s_blocked);
    
    // enemy piece calculation
    bboard enem_e = e & enemyPieces;
    bboard enem_e_blocked = eastVec[leastSignificantBit(enem_e)];
    bboard enem_w = w & enemyPieces;
    bboard enem_w_blocked = westVec[mostSignificantBit(enem_w)]; 
    bboard enem_n = n & enemyPieces;
    bboard enem_n_blocked = northVec[leastSignificantBit(enem_n)];
    bboard enem_s = s & enemyPieces;
    bboard enem_s_blocked = southVec[mostSignificantBit(enem_s)]; 

    bboard movesAfterBlockingEnemy = (e & ~enem_e_blocked) | (w & ~enem_w_blocked)
        | (n & ~enem_n_blocked) | (s & ~enem_s_blocked);

    bboard allMoves = movesAfterBlockingEnemy & movesAfterBlockingOwn;
    return allMoves;
}


bboard computePawnMovesBlack(bboard pawnLoc, bboard ownPieces, bboard enemyPieces) {
    bboard allPieces = ownPieces | enemyPieces;

    bboard oneSquare = (pawnLoc >> 8) & ~allPieces;
    bboard twoSquare = ((oneSquare & maskRank[RANK_6]) >> 8) & ~allPieces;

    bboard leftAttack = ((pawnLoc & clearFile[FILE_H]) >> 7) & enemyPieces;
    bboard rightAttack = ((pawnLoc & clearFile[FILE_A]) >> 9) & enemyPieces;

    bboard allMoves = oneSquare | twoSquare | leftAttack | rightAttack;
    return allMoves;
}

bboard computePawnMovesWhite(bboard pawnLoc, bboard ownPieces, bboard enemyPieces) {
    bboard allPieces = ownPieces | enemyPieces;

    bboard oneSquare = (pawnLoc << 8) & ~allPieces;
    bboard twoSquare = ((oneSquare & maskRank[RANK_3]) << 8) & ~allPieces;

    bboard leftAttack = ((pawnLoc & clearFile[FILE_A]) << 7) & enemyPieces;
    bboard rightAttack = ((pawnLoc & clearFile[FILE_H]) << 9) & enemyPieces;

    bboard allMoves = oneSquare | twoSquare | leftAttack | rightAttack;
    return allMoves;
}


bboard computeKnightMoves(bboard knightLoc, bboard ownPieces) {
    bboard spot1 = (knightLoc & clearFile[FILE_A] & clearFile[FILE_B]) << 6;
    bboard spot2 = (knightLoc & clearFile[FILE_A]) << 15;
    bboard spot3 = (knightLoc & clearFile[FILE_H]) << 17;
    bboard spot4 = (knightLoc & clearFile[FILE_H] & clearFile[FILE_G]) << 10;
    bboard spot5 = (knightLoc & clearFile[FILE_H] & clearFile[FILE_G]) >> 6;
    bboard spot6 = (knightLoc & clearFile[FILE_H]) >> 15;
    bboard spot7 = (knightLoc & clearFile[FILE_A]) >> 17;
    bboard spot8 = (knightLoc & clearFile[FILE_A] & clearFile[FILE_B]) >> 10;


    bboard allMoves = spot1 | spot2 | spot3 | spot4 | spot5 | spot6 | spot7 | spot8;
    bboard validMoves = allMoves & ~ownPieces;
    return validMoves;
}

bboard computeKingMoves(bboard kingLoc, bboard ownPieces) {
    
    bboard spot1 = (kingLoc & clearFile[FILE_A]) << 7;
    bboard spot2 = kingLoc << 8;
    bboard spot3 = (kingLoc & clearFile[FILE_H]) << 9;
    bboard spot4 = (kingLoc & clearFile[FILE_H]) << 1;
    bboard spot5 = (kingLoc & clearFile[FILE_H]) >> 7;
    bboard spot6 = kingLoc >> 8;
    bboard spot7 = (kingLoc & clearFile[FILE_A]) >> 9;
    bboard spot8 = (kingLoc & clearFile[FILE_A]) >> 1;
    
    bboard allMoves = spot1 | spot2 | spot3 | spot4 | spot5 | spot6 | spot7 | spot8;

    bboard validMoves = allMoves & ~ownPieces;

    return validMoves;

}

bboard clearFilesToRight(int index) {
    bboard cleared = 0xffffffffffffffff;

    if (index % 8 == 0) {
        cleared = cleared  & clearFile[FILE_B] 
            & clearFile[FILE_C] & clearFile[FILE_D] & clearFile[FILE_E]
            & clearFile[FILE_F] & clearFile[FILE_G] & clearFile[FILE_H];

    } else if (((index - 7) % 8) == 0) {
        cleared = cleared;
    } else if (((index - 6) % 8) == 0) {
        cleared = cleared & clearFile[FILE_H];
    } else if (((index - 5) % 8) == 0) {
        cleared = cleared & clearFile[FILE_G] 
            & clearFile[FILE_H];
    } else if (((index - 4) % 8) == 0) {
        cleared = cleared & clearFile[FILE_F] 
            & clearFile[FILE_G] & clearFile[FILE_H];
    } else if (((index - 3) % 8) == 0) {
        cleared = cleared & clearFile[FILE_F] & clearFile[FILE_G] 
            & clearFile[FILE_H] & clearFile[FILE_E];
    } else if (((index - 2) % 8) == 0) {
        cleared = cleared & clearFile[FILE_G] & clearFile[FILE_H] 
            & clearFile[FILE_D] & clearFile[FILE_E]
            & clearFile[FILE_F];
    } else if (((index - 1) % 8) == 0) {
        cleared = cleared & clearFile[FILE_C]
            & clearFile[FILE_D] & clearFile[FILE_E]
            & clearFile[FILE_F] & clearFile[FILE_G] & clearFile[FILE_H];
    }

    return cleared;
} 

bboard clearFilesToLeft(int index) {
    bboard cleared = 0xffffffffffffffff;

    if (index % 8 == 0) {
        return cleared;
    } else if (((index - 1) % 8) == 0) {
        cleared = cleared & clearFile[FILE_A];
    } else if (((index - 2) % 8) == 0) {
        cleared = cleared & clearFile[FILE_A] & clearFile[FILE_B];
    } else if (((index - 3) % 8) == 0) {
        cleared = cleared & clearFile[FILE_A] & clearFile[FILE_B] 
            & clearFile[FILE_C];
    } else if (((index - 4) % 8) == 0) {
        cleared = cleared & clearFile[FILE_A] & clearFile[FILE_B] 
            & clearFile[FILE_C] & clearFile[FILE_D];
    } else if (((index - 5) % 8) == 0) {
        cleared = cleared & clearFile[FILE_A] & clearFile[FILE_B] 
            & clearFile[FILE_C] & clearFile[FILE_D] & clearFile[FILE_E];
    } else if (((index - 6) % 8) == 0) {
        cleared = cleared & clearFile[FILE_A] & clearFile[FILE_B] 
            & clearFile[FILE_C] & clearFile[FILE_D] & clearFile[FILE_E]
            & clearFile[FILE_F];
    } else if (((index - 7) % 8) == 0) {
        cleared = cleared & clearFile[FILE_A] & clearFile[FILE_B] 
            & clearFile[FILE_C] & clearFile[FILE_D] & clearFile[FILE_E]
            & clearFile[FILE_F] & clearFile[FILE_G];
    }

    return cleared;
}

void computeWhitePseudo(struct ChessBoard *BoardState, move *moves, int *move_counter) {
    
    for (int i=0; i < 63; i++) {
        
        int piece_type = getPieceAtIndex(BoardState, i);
        bboard current_square =  ((bboard) 1 << (bboard)i);
        switch (piece_type) {
            case WPAWN:
                computeMoveList(computePawnMovesWhite(current_square, BoardState->WhitePieces, BoardState->BlackPieces), 
                    current_square, moves, move_counter);
                break;
            case WKNIGHT:
                computeMoveList(computeKnightMoves(current_square, BoardState->WhitePieces), 
                    current_square, moves, move_counter);
                break;
            case WBISHOP:
                computeMoveList(computeBishopMoves(current_square, BoardState->WhitePieces, BoardState->BlackPieces), 
                    current_square, moves, move_counter);
                break;
            case WROOK:
                computeMoveList(computeRookMoves(current_square, BoardState->WhitePieces, BoardState->BlackPieces), 
                    current_square, moves, move_counter);
                break;
            case WQUEEN:
                computeMoveList(computeQueenMoves(current_square, BoardState->WhitePieces, BoardState->BlackPieces), 
                    current_square, moves, move_counter);
                break;
            case WKING:
                computeMoveList(computeKingMoves(current_square, BoardState->WhitePieces), 
                    current_square, moves, move_counter);
                break;
        }
    }
}

void computeBlackPseudo(struct ChessBoard *BoardState, move *moves, int *move_counter) {
    for (int i=0; i < 63; i++) {

        int piece_type = getPieceAtIndex(BoardState, i);
        bboard current_square =  ((bboard) 1 << (bboard) i);

        switch (piece_type) {
        
            case BPAWN:
                computeMoveList(computePawnMovesBlack(current_square, BoardState->BlackPieces, BoardState->WhitePieces), 
                    current_square, moves, move_counter);
                break;
            case BKNIGHT:
                computeMoveList(computeKnightMoves(current_square, BoardState->BlackPieces), 
                    current_square, moves, move_counter);
                break;
            case BBISHOP:
                computeMoveList(computeBishopMoves(current_square, BoardState->BlackPieces, BoardState->WhitePieces), 
                    current_square, moves, move_counter);
                break;
            case BROOK:
                computeMoveList(computeRookMoves(current_square, BoardState->BlackPieces, BoardState->WhitePieces), 
                    current_square, moves, move_counter);
                break;
            case BQUEEN:
                computeMoveList(computeQueenMoves(current_square, BoardState->BlackPieces, BoardState->WhitePieces), 
                    current_square, moves, move_counter);
                break;
            case BKING:
                computeMoveList(computeKingMoves(current_square, BoardState->BlackPieces), 
                    current_square, moves, move_counter);
                break;

        }
    }
}

void validateMoveList(struct ChessBoard *InitialBoard, move *pseudo_move_list, int *p_counter, move *legal_move_list, int *l_counter, int side) {
    for (int i = 0; i < *p_counter; i++) {

        int t_counter = 0;
        int king_index = 0;
        int reject_move = 0;
        move p_move = pseudo_move_list[i];
        struct ChessBoard TestBoard = *InitialBoard;

        move test_moves[218];
            for (int i=0; i < 218; i++) {
                test_moves[i].from_square = 0;
                test_moves[i].to_square = 0;
                test_moves[i].flags = 0;

            }
        executeMove(&TestBoard, p_move);
        //printBoard(TestBoard.AllPieces);
        // need to compute pseudo moves for black at test position and see if any black pieces can take white king...
        // for brevity im going to assume theres only one king per side
        // this loop runs when its whites turn on the initial board
        if (InitialBoard->side) {
            computeBlackPseudo(&TestBoard, test_moves, &t_counter);
            king_index = leastSignificantBit(TestBoard.WhiteKings);
        } 
        else {
            computeWhitePseudo(&TestBoard, test_moves, &t_counter);
            king_index = leastSignificantBit(TestBoard.BlackKings);
        }

        for (int j = 0; j < t_counter; j++) {
            if (test_moves[j].to_square == king_index) {
                // king can be attacked, reject move by breaking loop
                reject_move = 1;
                break;
            }
        }

        if (!reject_move) {
            legal_move_list[*l_counter] = pseudo_move_list[i];
            (*l_counter)++;
        }
    }
}

void computeMoveList(bboard pseudo_moves, bboard loc, move *moves, int *move_counter) {
    bboard moveBoard = pseudo_moves;
    while(moveBoard != 0) {
        moves[*move_counter].from_square = leastSignificantBit(loc);
        moves[*move_counter].to_square = leastSignificantBit(moveBoard);

        //printf("%i", move_counter);
        moveBoard = moveBoard & ~((bboard) 1 << leastSignificantBit(moveBoard));

        (*move_counter)++;
    }
}

int mostSignificantBit(bboard board) {
    // returns the index of the square that represents the MSB
    bboard decrementBoard = board;
    int index = 0;
    for (int i = 0; i < 65; i++) {
        if (decrementBoard == 0) {
            index = i - 1;
            break;
        }
        decrementBoard >>= 1;
    }
    return index;
}

int leastSignificantBit(bboard square) {
    bboard decrementSquare = square;
    int index = 0;
    for (int i = 0; i < 64; i++) {

        if (decrementSquare == 0) {
            index = 64-i;
            break;
        }
        decrementSquare <<= 1;
    }
    if (index > 63) {
        index = -1; // just to keep this function consistant with LSB function
    }

    return index;
}

void printBoard(bboard board) {
    char outBits[64];

    for (int i = 63; i > -1; i--) {
        char oneOrZero;
        oneOrZero = (board % 2) ? '1': '0';
        outBits[i] = oneOrZero; 

        board = board >> 1;
    }
    for (int i = 8; i <=64; i=i+8) {
        for (int j = 0; j < 8 ; j++) {
            printf("%c ", outBits[i - j - 1]);
            if ( (i+j+1) % 8 == 0) {
                printf("\n");
            }
        }
        
    }
    printf("\n");
    /* This prints a reversed layout

    for (int i = 0; i <= 63; i++) {
        printf("%c  ", outBits[i]);
        if ((i+1) % 8 == 0) {
            printf("\n");
        }
    }
    */
}