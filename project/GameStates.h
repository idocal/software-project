//
// Created by Ido on 8/19/17.
//

#include "GameMoves.h"
#include "PieceRules.h"
#include "stdbool.h"

#ifndef PROJECT_GAMESTATES_H
#define PROJECT_GAMESTATES_H

/**
 * Detect whether the game is in "Check" mode.
 *
 * @param game
 * @return true iff current player is being Checked.
 */
bool isCheck(CHESS_GAME *game);

/**
 * Detect whether the game is in "Checkmate" mode.
 *
 * @param game
 * @return true iff current player has lost.
 */
bool isCheckMate(CHESS_GAME *game);

/**
 * Detect whether the game is in "Tie" mode.
 *
 * @param game
 * @return true iff the game is tied.
 */
bool isTie(CHESS_GAME *game);

/**
 * Uses the above function to update the game state UPON performing a move.
 * This function should be used before the turn to detect what state is the game at.
 *
 * @param game
 */
void updateGameState(CHESS_GAME *game);

#endif //PROJECT_GAMESTATES_H
