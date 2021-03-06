//
// Created by okleinfeld on 7/28/2017.
//

#ifndef PROJECT_COMMANDPARSING_H
#define PROJECT_COMMANDPARSING_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "GameMoves.h"

#define MAX_COMMAND_LENGTH 1024

/**
 * CommandParsing Summary:
 *
 * The structures and functions defined in this header are part of the module
 * responsible for parsing input from the user through the stdin file.
 *
 */



/**
 * Enum defining the possible types of a user command
 * including the INVALID_COMMAND for general purpose errors/ unknown commands.
 *
 */
typedef enum {
    GAME_MODE,
    DIFFICULTY,
    USER_COLOR,
    LOAD,
    DEFAULT,
    PRINT_SETTING,
    QUIT,
    START,
    INVALID_COMMAND,
} SETTING_STATE_COMMAND_NAME;


/**
 * Structure that represents a parsed user command - only for the setting phase commands!
 * the commandArgument and addressForLoadCommand fields assigned with default values
 * and their semantics varies depending on the actual command
 *
 */
typedef struct setting_state_command {
    SETTING_STATE_COMMAND_NAME command_name;
    int commandArgument;
    char *addressForLoadCommand;
} SETTING_STATE_COMMAND;

/**
 * Enum defining all possible game actions
 */
typedef enum {
    MOVE,
    GET_MOVES,
    SAVE,
    UNDO,
    RESET,
    QUIT_GAME,
    INVALID_GAME_COMMAND
} GAME_STATE_COMMAND_NAME;

/**
 * Structure that represents a parsed user command - only for the actual game phase commands!
 * move only for MOVE command.
 * filename only for SAVE command.
 * <x,y> only for GET_MOVES command
 *
 */
typedef struct game_command {
    GAME_STATE_COMMAND_NAME command_name;
    GAME_MOVE *move;
    char *filename;
    int x;
    int y;
} GAME_STATE_COMMAND;


/**
 * Creates a new empty/default SettingStateCommand object and return a pointer to it.
 * Preforms all the memory allocation commands needed.
 *
 *
 * @return
 * NULL if  a memory allocation failure occurs.
 * Otherwise, a pointer to a new SETTING_STATE_COMMAND object.
 *
 * Initialization of the object's fields (default initialization values)
 * command_name -> INVALID_COMMAND
 * commandArgument -> -1
 * addressForLoadCommand -> NULL
 */
SETTING_STATE_COMMAND *createDefaultStateCommand();



/**
 * Free all the memory allocated for given command object
 *
 *
 * @param command - a pointer to a valid SETTING_STATE_COMMAND object
 */
void destroyStateCommand(SETTING_STATE_COMMAND *command);

/**
 * Transform a user input (from stdin) to a matching SETTING_STATE_COMMAND object
 *
 *
 * @return - a Setting State Command Name object.
 * string includes more than 2 word (delimited by space) - return INVALID_COMMAND
 * string includes at least 1 word but doesn't match any command name - return INVALID_COMMAND
 * DEFAULT/PRINT_SETTING/QUIT/START(0 arguments commands):
 *         return matching command name if string includes 0 argument
 *         otherwise return INVALID_COMMAND
 * LOAD - return command with LOAD name and address argument will contain the rest of the string
 * doesn't check that the file name is valid or even not NULL - this will be checked in later functions
 *
 * GAME_MODE/DIFFICULTY/USER_COLOR-
 * if the attached argument is a single digit - return object with command name + argument integer value
 * otherwise - return object with command name + argument value = -1.
 * do not return INVALID_COMMAND in order to apply a distinction between these error in later functions.
*/

SETTING_STATE_COMMAND *parseUserSettingCommand();

/**
 * Transform a user input (from stdin) to a matching GAME_STATE_COMMAND object
 *
 * @return a Game State Command object
 */
GAME_STATE_COMMAND *parseUserGameCommand();


/**
 * Parse a user command given as a string and transform it to the right
 * command name.
 *
 * @param string - a user command represented as a string
 *
 * @return - a Setting State Command Name object.
 * notice that the string has to be identical to the command name (without extra chars at all)
 * otherwise the function will return an object with a value of "INVALID_COMMAND"
*/
SETTING_STATE_COMMAND_NAME parseStringToSettingCommandEnum(char *string);


/**
 * Transform a decimal digit (digit! not number) represented as a string
 * to the correct integer value it represents
 *
 * @param commandArg - a user command string that represents a single digit
 *
 * @return - an integer with a matching value
 * notice that if the string contains more than 1 character (for example "12" or "1a")
 * or a single character that doesn't represent a digit (for example "a")
 * the function will return the value -1 to indicate a fault/error value.
*/
int transformCommandArgFromStrToInt(const char *commandArg);

/**
 * Creates a new empty/default GameCommand object and return a pointer to it.
 * Preforms all the memory allocation commands needed.
 *
 *
 * @return
 * NULL if  a memory allocation failure occurs.
 * Otherwise, a pointer to a new GAME_STATE_COMMAND object is returned
 *
 * Initialization of the object's fields (default initialization values)
 * command_name -> INVALID_COMMAND_NAME
 * command move -> NULL
 * command file name -> NULL
 * command x -> -1
 * command y -> -1
 */

GAME_STATE_COMMAND *createDefaultGameCommand();

/**
 * a destructor for a game command.
 * frees all the associated memory
 * @param command
 */

void destroyGameCommand(GAME_STATE_COMMAND *command);

/**
 * parse the first token of the user's input string
 * @param commandString
 * @return a matching command name enum or INVALID_GAME_COMMAND of no match was found
 */

GAME_STATE_COMMAND_NAME parseStringToGameCommandEnum(char *commandString);

/**
 * the function checks if a position string is in valid format and has valid value,
 * i.e movePosSting == "<[1-8],[A-H]>"
 * @param movePosSting
 * @return
 * 1 if the string is in the right format and has valid values
 * 0 otherwise
 */

int checkValidityOfMovePositionString(char *movePosSting);

/**
 * the function checks if the position string is in the right format,
 * without checking for validity of the values themselves.
 * i.e movePosSting == "<.{1}, .{1}>"
 * @param movePosSting
 * @return
 * 1 if the string is in the right format
 * 0 otherwise
 */

int checkStructureOfMovePositionString(char *movePosSting);

#endif //PROJECT_COMMANDPARSING_H
