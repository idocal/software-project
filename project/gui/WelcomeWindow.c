//
// Created by Ido on 9/9/17.
//

#include "WelcomeWindow.h"

int drawWelcomeWindow(GENERIC_WINDOW *genericWindow, SDL_Window *sdlWindow, SDL_Renderer *renderer, CHESS_MATCH *match) {
    unsigned numWidgets = 3;
    genericWindow->numWidgets = numWidgets;
    int numWidgetsCreated = 0;
    genericWindow->type = WELCOME_WINDOW;
    genericWindow->handleWindowEvent = welcomeWindowEventHandler;

    // Assign the application window and renderer
    genericWindow->window = sdlWindow;
    genericWindow->renderer = renderer;

    WIDGET **widgets = (WIDGET **) calloc(numWidgets, sizeof(WIDGET *));
    if (widgets == NULL) return destroyWindowOnFailure(genericWindow, numWidgetsCreated); // On failure
    genericWindow->widgets = widgets;

    // Load widgets into widgets array
    widgets[0] = createWidget(createNewGameButton, renderer);
    if (widgets[0] == NULL) return destroyWindowOnFailure(genericWindow, numWidgetsCreated); // On failure
    numWidgetsCreated++;

    widgets[1] = createWidget(createLoadWelcomeButton, renderer);
    if (widgets[1] == NULL) return destroyWindowOnFailure(genericWindow, numWidgetsCreated); // On failure
    numWidgetsCreated++;

    widgets[2] = createWidget(createExitWelcomeButton, renderer);
    if (widgets[2] == NULL) return destroyWindowOnFailure(genericWindow, numWidgetsCreated); // On failure
    numWidgetsCreated++;

    reRenderWindow(genericWindow);

    useMatchAndStack(match, NULL);
    // because of the generic infrastructure of the code a match pointer has to be passed to a draw window function
    // the match object is changed by few windows like the load game and setting
    // therefore in order to avoid "unused argument error" we must use the argument inside the function

    return 1;
}

int createNewGameButton(WIDGET *widget, SDL_Renderer *renderer) {
    return createButton(WELCOME_WINDOW_MIDDLE_X_POS, PAGE_MARGIN, "./img/new_game.bmp", renderer, widget, false);
}

int createLoadWelcomeButton(WIDGET *widget, SDL_Renderer *renderer) {
    return createButton(WELCOME_WINDOW_MIDDLE_X_POS, PAGE_MARGIN + BUTTON_HEIGHT + BUTTON_MARGIN, "./img/load.bmp", renderer, widget, false);
}

int createExitWelcomeButton(WIDGET *widget, SDL_Renderer *renderer) {
    return createButton(WELCOME_WINDOW_MIDDLE_X_POS, WINDOW_HEIGHT - PAGE_MARGIN - BUTTON_HEIGHT, "./img/exit.bmp", renderer, widget, false);
}



EVENT_RESPONSE *welcomeWindowEventHandler(GENERIC_WINDOW *window, SDL_Event *event, CHESS_MATCH *match, MOVES_STACK *stack) {
    WINDOW_TYPE nextWindow = WELCOME_WINDOW;
    int widgetIndex = getClickedWidget(window, event);
    EVENT_RESPONSE *response = createEventResponse(nextWindow, SAME_WINDOW);

    if (widgetIndex == 0) { // The button clicked is New Game
        response->windowType = SETTINGS_MODE_WINDOW;
        response->status = NEW_WINDOW;
    }

    if (widgetIndex == 1) { // The button clicked is Load Game
        response->windowType = LOAD_WINDOW;
        response->status = NEW_WINDOW;
    }

    if (widgetIndex == 2) { // The button clicked is Exit
        response->status = EXIT_WINDOW; // NULL window determines exiting
    }

    useMatchAndStack(match, stack);
    // because of the generic infrastructure of the code a match/stack pointers have to be passed to an event handle function
    // the match/stack object is changed by few windows like the load game and setting and game window
    // therefore in order to avoid "unused argument error" we must use the arguments inside the function



    return response;
}

