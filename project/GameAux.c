//
// Created by Ido on 8/13/17.
//

#include "GameAux.h"

int applyGameState(CHESS_GAME *);
int AIMove(CHESS_MATCH *, MOVES_STACK *);
int AIFirstMove(CHESS_MATCH *match);

int evaluateSettingStateCommand(CHESS_MATCH **matchPtr, SETTING_STATE_COMMAND *cmd) {
    SETTING_STATE_COMMAND_NAME name = cmd->command_name;
    int arg = cmd->commandArgument;
    char *addressArg = cmd->addressForLoadCommand;
    CHESS_MATCH *match = *matchPtr;

    switch(name) {
        case INVALID_COMMAND : {
            printf(INVALID_COMMAND_ERROR);
            return 0;
        }

        case GAME_MODE : {
            if (arg == 1 || arg == 2) {
                match->gameMode = arg;
                (arg == 1) ? printf(SET_ONE_PLAYER_COMMAND) : printf(SET_TWO_PLAYERS_COMMAND);
            } else {
                printf(WRONG_GAME_MODE_COMMAND);
            }
            return 0;
        }

        case DIFFICULTY : {
            if (match->gameMode == 2){
                printf(INVALID_COMMAND_ERROR);
            }
            if (match->gameMode == 1) {
                if (arg >= 1 && arg <= 5) match->level = arg;
                else printf(WRONG_DIFFICULTY_LEVEL_ERROR);
            }
            return 0;
        }

        case USER_COLOR : {
            if (match->gameMode == 2) {
                printf(INVALID_COMMAND_ERROR);
            } else{
                if (arg == 0 || arg == 1)
                    match->userColor = arg;
                else
                    printf(WRONG_USER_COLOR);
            }
            return 0;
        }

        case LOAD : {
            CHESS_MATCH *newMatch = parseXMLGameFile(addressArg);
            if (newMatch == NULL){
                printf(LOAD_FILE_ERROR);
                return 0;
            }
            CHESS_MATCH *destroyMatch = match;
            *matchPtr = newMatch;
            destroyChessMatch(destroyMatch);
            return 0;
        }

        case DEFAULT : {
            resetMatchSettings(match);
            initGameBoard(match->game);
            return 0;
        }

        case PRINT_SETTING : {
            printMatchSettings(match);
            return 0;
        }

        case QUIT : {
            printf(EXIT_MESSAGE);
            return 2;
        }

        case START : {
            return 1;
        }

        default : return -1; // this should not happen. If it does - there is a bug in the command parsing module
    }
}

bool initiateChessMatch(CHESS_MATCH **match){
    int status = 0;
    while (status == 0){
        SETTING_STATE_COMMAND *cmd = parseUserSettingCommand();
        status = evaluateSettingStateCommand(match, cmd);
        destroyStateCommand(cmd);
    }
    bool retValue = (status == 1) ? true : false;
    return retValue;
}

void printUndoMessage(CHESS_GAME *game, GAME_MOVE *move) {
    char player = game->currentPlayer;
    int sourceX = move->sourceRowIndex;
    int sourceY = move->sourceColIndex;
    int destX = move->destRowIndex;
    int destY = move->destColIndex;
    sourceX += '1'; destX += '1';
    sourceY += 'A'; destY += 'A';

    printf("Undo move for player %s : <%c,%c> -> <%c,%c>\n", color(player), destX, destY, sourceX, sourceY);
}

void handleUndoMove(CHESS_GAME *game, MOVES_STACK *stack) {
    GAME_MOVE *prevMove = pop(stack);
    revertMove(game, prevMove);
    printUndoMessage(game, prevMove);
    destroyGameMove(prevMove);
}

void handleGetMovesCommand(int x, int y, CHESS_GAME *game){
    MATRIX *possibleMoves = getPossibleMoves(game, x, y);
    for (int i = 0; i < nRows; ++i) {
        for (int j = 0; j < nCols; ++j) {
            if (matGet(possibleMoves, i, j) == 1) { // if move is a possible one
                char iChar = (char) (i + '1');
                char jChar = (char) (j + 'A');
                char destPiece = matGet(game->gameBoard, i, j);
                bool capture = (destPiece != '_') ? true : false; // since cell can not be player's

                CHESS_GAME *cpyGame = copyChessGame(game);
                GAME_MOVE *move = createGameMove(game, x, y, i, j); // move <x,y> -> <i,j>
                performMove(cpyGame, move);
                bool threat = isThreatened(cpyGame, i, j);
                destroyChessGame(cpyGame);
                destroyGameMove(move);

                if (capture && threat){
                    printf("<%c,%c>*^\n", iChar, jChar); // <x,y>*^
                } else if (capture){
                    printf("<%c,%c>^\n", iChar, jChar); // <x,y>^
                } else if (threat){
                    printf("<%c,%c>*\n", iChar, jChar); // <x,y>*
                } else{
                    printf("<%c,%c>\n", iChar, jChar); // <x,y>
                }

            }
        }
    }
    matDestroy(possibleMoves);
}

int evaluateGameStateCommand(CHESS_MATCH *match, GAME_STATE_COMMAND *cmd, MOVES_STACK *stack) {
    CHESS_GAME *game = match->game;
    GAME_STATE_COMMAND_NAME name = cmd->command_name;
    GAME_MOVE *move = cmd->move;
    int mode = match->gameMode;
    char *filename = cmd->filename;
    int x = cmd->x;
    int y = cmd->y;
    char player = game->currentPlayer;

    if (move != NULL) { // update move original symbols for revert
        move->sourceOriginalSymbol = matGet(game->gameBoard, move->sourceRowIndex, move->sourceColIndex);
        move->destOriginalSymbol = matGet(game->gameBoard, move->destRowIndex, move->destColIndex);
    }

    switch (name) {
        case INVALID_GAME_COMMAND : {
            printf(INVALID_COMMAND_ERROR);
            return 3;
        }

        case MOVE : {
            if (move == NULL) {
                printf(INVALID_POSITION_MESSAGE);
                return 3; // should not print board
            }
            else if (!isSlotOccupied(game, move->sourceRowIndex, move->sourceColIndex, player)) {
                printf(NO_PLAYER_PIECE_LOCATION_MESSAGE);
                destroyGameMove(move);
                return 3; // should not print board
            }
            else if (!isMoveLegal(game, move)) {
                printf(ILLEGAL_MOVE_MESSAGE);
                destroyGameMove(move);
                return 3; // should not print board
            }
            else {
                performMove(game, move);
                push(stack, move);
            }
            return 0;
        }

        case GET_MOVES : {
            if (match->gameMode == 2){
                printf(GET_MOVES_NOT_SUPPORT_TWO_PLAYER);
                return 3; // feature is not supported for two players mode
            }
            if (match->level > 2) { // feature is not supported for level > 2
                printf(GET_MOVES_NOT_SUPPORT_LEVEL);
                return 3; // should not print board
            }
            if (x == -1 || y == -1){
                printf(INVALID_POSITION_MESSAGE);
                return 3; // should not print board
            }
            if (!isSlotOccupied(game, x, y, player)){
                printf(NO_PLAYER_PIECE_LOCATION_MESSAGE);
                return 3;
            }
            handleGetMovesCommand(x, y, game);
            return 3;
        }

        case SAVE : {
            writeMatchObjectToXmlFile(match, filename);
            return 3;
        }

        case UNDO : {
            if (mode == 2) printf(UNDO_UNAVAILABLE_MESSAGE);
            else if (stack->currentSize == 0) printf(EMPTY_HISTORY_MESSAGE);
            else {
                // twice for computer's turn as well
                switchPlayers(game);
                handleUndoMove(game, stack);
                switchPlayers(game);
                handleUndoMove(game, stack);
                printChessGameBoard(game);
            }
            return 3;
        }

        case RESET : {
            printf(RESTARTING_MESSAGE);
            return 1;
        }

        case QUIT_GAME : {
            printf(EXIT_MESSAGE);
            return 2;
        }

        default : return -1; // this should not happen. IF it does - there is a bug in the command parsing module
    }
}

bool initiateChessGame(CHESS_MATCH *match) {
    CHESS_GAME *game = match->game;
    int mode = match->gameMode;
    bool firstTurn = true;
    int status = 0;

    MOVES_STACK *stack = createEmptyStack(UNDO_CAPACITY);

    if (match->userColor == 0) status = AIFirstMove(match);

    // Game state loop
    while (status == 0 || status == 3) {
        bool shouldPrintMessage = (status == 3) ? false : true;

        if (shouldPrintMessage && mode == 2) { // if message is printed, current player should not switch (only 2 players mode)
            if (!firstTurn) switchPlayers(game); // switch turns
        }

        char player = game->currentPlayer;
        firstTurn = false;

        status = applyGameState(game);
        if (status == 2){
            break;
        }

        if (shouldPrintMessage) {
            printChessGameBoard(game);
            if (game->status == CHECK) {
                if (mode == 1) printf(CHECK_AI_MESSAGE);
                if (mode == 2) printf(CHECK_MESSAGE, color(player));
            }
        }
        printf("%s %s", color(game->currentPlayer), NEXT_MOVE_MESSAGE);

        GAME_STATE_COMMAND *cmd = parseUserGameCommand();
        status = evaluateGameStateCommand(match, cmd, stack);
        destroyGameCommand(cmd);

        if (status == 0 && mode == 1)  {
            status = AIMove(match, stack); // If command passed correctly, perform AI move
            if (status == -1) { // AI Move failed (probably memory allocation problem)
                status = 2;
                printf(FATAL_ERROR_MESSAGE);
            }
        }

        if (game->pruningThreshold > 0) game->pruningThreshold -= 0.01;
    }

    // Game is terminated after loop
    destroyChessMatch(match);
    destroyStack(stack);

    bool retValue = (status == 1) ? true : false; // quitting the loop is only status 1 or 2.
    return retValue;
}

int applyGameState(CHESS_GAME *game) {
    updateGameState(game); // detect check or checkmate or tie
    GAME_STATUS state = game->status;
    char player = game->currentPlayer;

    if (state == MATE) {
        printf(CHECKMATE_MESSAGE, color(opponent(player)));
        return 2;
    }

    else if (state == TIE) {
        printf(TIE_MESSAGE);
        return 2;
    }

    return 0;
}

void handleAIMoveMessage(GAME_MOVE *AIMove) {
    int x = AIMove->sourceRowIndex;
    int y = AIMove->sourceColIndex;
    int i = AIMove->destRowIndex;
    int j = AIMove->destColIndex;
    char movedPiece[PIECE_NAME_SIZE];

    int X = x + '1';
    int Y = y + 'A';
    int I = i + '1';
    int J = j + 'A';

    strcpy(movedPiece, pieceName(AIMove->sourceOriginalSymbol));
    printf(COMPUTER_MOVE_MESSAGE, movedPiece, X, Y, I, J);
}

int AIMove(CHESS_MATCH *match, MOVES_STACK *stack) {
    CHESS_GAME *game = match->game;
    switchPlayers(game); // switch to opponent for computer move
    int status = applyGameState(game);
    if (status == 0) { // game is not over
        int maxDepth;
        GAME_MOVE *AIMove = NULL;
        if (match->level == 5) {
            maxDepth = 4;
            AIMove = AINextMove(game, &(maxDepth), true);
        } else {
            maxDepth = match->level;
            AIMove = AINextMove(game, &(maxDepth), false);
        }
        if (AIMove == NULL) {
            GAME_MOVE *firstPossibleMove = getFirstPossibleMove(game);
            if (firstPossibleMove == NULL) return -1;
            AIMove = firstPossibleMove;
        }
        performMove(game, AIMove);
        push(stack, AIMove);
        switchPlayers(game); // switch back to player

        handleAIMoveMessage(AIMove);

    }

    return status;
}

int AIFirstMove(CHESS_MATCH *match) {
    CHESS_GAME *game = match->game;
    int maxDepth;
    GAME_MOVE *AIMove = NULL;

    if (match->level == 5) {
        maxDepth = 4;
        AIMove = AINextMove(game, &(maxDepth), true);
    } else {
        maxDepth = match->level;
        AIMove = AINextMove(game, &(maxDepth), false);
    }
    if (AIMove == NULL) return -1;

    performMove(game, AIMove);
    handleAIMoveMessage(AIMove);
    destroyGameMove(AIMove); // this move is not going to be on the stack

    switchPlayers(game); // switch back to player

    return 0;
}