//
// Created by Ido on 8/20/17.
//

#include "GameMoves.h"

GAME_MOVE *createGameMove(CHESS_GAME *game, int source_row, int source_col, int dest_row, int dest_col){
    if (game == NULL){
        return NULL;
    }
    GAME_MOVE *move = (GAME_MOVE *) malloc(sizeof(GAME_MOVE));
    if (move == NULL){
        return NULL;
    }
    move->sourceRowIndex = source_row;
    move->sourceColIndex = source_col;
    move->destRowIndex = dest_row;
    move->destColIndex = dest_col;
    move->sourceOriginalSymbol = matGet(game->gameBoard, source_row, source_col);
    move->destOriginalSymbol = matGet(game->gameBoard, dest_row, dest_col);
    return move;
}

GAME_MOVE* createGameMoveWithoutGame(int sourceRow, int sourceCol, int destRow, int destCol){
    GAME_MOVE *move = (GAME_MOVE *) malloc(sizeof(GAME_MOVE));
    if (move == NULL){
        return NULL;
    }
    move->sourceRowIndex = sourceRow;
    move->sourceColIndex = sourceCol;
    move->destRowIndex = destRow;
    move->destColIndex = destCol;
    move->sourceOriginalSymbol = '\0';
    move->destOriginalSymbol = '\0';
    return move;
}

GAME_MOVE *copyGameMove(GAME_MOVE *move) {
    GAME_MOVE *cpyMove = (GAME_MOVE *) malloc(sizeof(GAME_MOVE));
    if (cpyMove == NULL){
        return NULL;
    }
    cpyMove->sourceRowIndex = move->sourceRowIndex;
    cpyMove->sourceColIndex = move->sourceColIndex;
    cpyMove->destRowIndex = move->destRowIndex;
    cpyMove->destColIndex = move->destColIndex;
    cpyMove->sourceOriginalSymbol = move->sourceOriginalSymbol;
    cpyMove->destOriginalSymbol = move->destOriginalSymbol;
    return cpyMove;
}

void destroyGameMove(GAME_MOVE *move){
    if (move == NULL){
        return;
    }
    free(move);
}

void performMove(CHESS_GAME *game, GAME_MOVE *move) {
    MATRIX *board = game->gameBoard;
    char piece = matGet(board, move->sourceRowIndex, move->sourceColIndex); // get the piece on previous cell
    matSet(board, move->sourceRowIndex, move->sourceColIndex, '_'); // fill blank on previous cell
    matSet(board, move->destRowIndex, move->destColIndex, piece); // set piece on current cell
}

void revertMove(CHESS_GAME *game, GAME_MOVE *move) {
    GAME_MOVE *inverseMove = createGameMove(game, move->destRowIndex, move->destColIndex, move->sourceRowIndex, move->sourceColIndex);
    performMove(game, inverseMove);
    matSet(game->gameBoard, move->destRowIndex, move->destColIndex, move->destOriginalSymbol); // assign the original piece on original destination
    destroyGameMove(inverseMove);
}

bool isConquer(GAME_MOVE *move) {
    if (move == NULL) return false;
    if (move->destOriginalSymbol != '_') return true;
    return false;
}
