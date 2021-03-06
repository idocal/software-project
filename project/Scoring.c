//
// Created by Ido on 8/25/17.
//

#include "Scoring.h"

int pieceScore(char piece) {
    switch(piece) {
        case 'm' :
            return 1;

        case 'M' :
            return -1;

        case 'b' :
            return 3;

        case 'B' :
            return -3;

        case 'r' :
            return 5;

        case 'R' :
            return -5;

        case 'n' :
            return 3;

        case 'N' :
            return -3;

        case 'q' :
            return 9;

        case 'Q' :
            return -9;

        case 'k' :
            return 100;

        case 'K' :
            return -100;

        default :
            return 0; // should be blank
    }
}

int expertPieceLegend(char piece) {
    if (piece == 'r' || piece == 'R') return 5;
    if (piece == 'n' || piece == 'N' || piece == 'b' || piece == 'B') return 5;
    if (piece == 'q' || piece == 'Q') return 9;
    if (piece == 'k' || piece == 'K') return 15;
    return 0; // have to add this in order to compile the code under Werror flag
}


int score(CHESS_GAME *game) {
    MATRIX *board = game->gameBoard;
    int score = 0;
    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            score += pieceScore(matGet(board, i, j));
        }
    }
    return score;
}



int pawnStructure(CHESS_GAME *game) {
    int defendedPawns = 0;

    for (int x = 0; x < nRows; x++) {
        for (int y = 0; y < nCols; y++) {
            char piece = matGet(game->gameBoard, x, y);
            if (piece == 'm') { // black pawn
                int adjRow = x + 1;
                int adjRight = y + 1;
                int adjLeft = y - 1;

                if (!isOutOfBounds(adjRow, adjLeft)) defendedPawns -= 1;
                if (!isOutOfBounds(adjRow, adjRight)) defendedPawns -= 1;

            }
            else if (piece == 'M') { // white pawn
                int adjRow = x - 1;
                int adjRight = y + 1;
                int adjLeft = y - 1;

                if (!isOutOfBounds(adjRow, adjLeft)) defendedPawns += 1;
                if (!isOutOfBounds(adjRow, adjRight)) defendedPawns += 1;
            }
        }
    }

    return defendedPawns;
}

int isCheckScore(CHESS_GAME *game) {
    char player = game->currentPlayer;
    int sign = (player == 1) ? 1 : -1;
    return sign * isGameCheck(game, game->currentPlayer);
}

int expertScore(CHESS_GAME *game) {
    int expertModeScore = 0;

    // combine naive scoring function with expert scoring function. give a different weight to each scoring function
    expertModeScore += 50 * score(game); // original score function gets weight 50
    expertModeScore += 25 * isCheckScore(game); // The current player is switched before the move, so increment if the player is checked
    expertModeScore += 5 * pawnStructure(game); // defended pawns are a goal when all other moves lead to the same score
    return expertModeScore;
}
