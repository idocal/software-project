//
// Created by Ido on 9/10/17.
//

#include "ColorWindow.h"

int drawColorWindow(GENERIC_WINDOW *genericWindow, SDL_Window *sdlWindow, SDL_Renderer *renderer, CHESS_MATCH *match) {
    signed numWidgets = 5;
    genericWindow->numWidgets = numWidgets;
    int numWidgetsCreated = 0;
    genericWindow->type = SETTINGS_COLOR_WINDOW;
    genericWindow->handleWindowEvent = colorWindowEventHandler;

    // Assign the application window and renderer
    genericWindow->window = sdlWindow;
    genericWindow->renderer = renderer;

    WIDGET **widgets = (WIDGET **) calloc(numWidgets, sizeof(WIDGET *));
    if (widgets == NULL) return destroyWindowOnFailure(genericWindow, numWidgetsCreated); // On failure
    genericWindow->widgets = widgets;

    // Load widgets into widgets array
    widgets[0] = createWidget(createColorTitle, renderer);
    if (widgets[0] == NULL) return destroyWindowOnFailure(genericWindow, numWidgetsCreated); // On failure
    numWidgetsCreated++;

    widgets[1] = createWidget(createWhiteButton, renderer);
    if (widgets[1] == NULL) return destroyWindowOnFailure(genericWindow, numWidgetsCreated); // On failure
    numWidgetsCreated++;

    widgets[2] = createWidget(createBlackButton, renderer);
    if (widgets[2] == NULL) return destroyWindowOnFailure(genericWindow, numWidgetsCreated); // On failure
    numWidgetsCreated++;

    widgets[3] = createWidget(createBackButton, renderer);
    if (widgets[3] == NULL) return destroyWindowOnFailure(genericWindow, numWidgetsCreated); // On failure
    numWidgetsCreated++;

    widgets[4] = createWidget(createStartButton, renderer);
    if (widgets[4] == NULL) return destroyWindowOnFailure(genericWindow, numWidgetsCreated); // On failure
    numWidgetsCreated++;

    reRenderWindow(genericWindow);

    useMatchAndStack(match, NULL);
    // because of the generic infrastructure of the code a match/stack pointers have to be passed to an event handle function
    // the match/stack object is changed by few windows like the load game and setting and game window
    // therefore in order to avoid "unused argument error" we must use the arguments inside the function

    return 1;
}

int createColorTitle(WIDGET *widget, SDL_Renderer *renderer) {
    return createTitle("./img/color.bmp", renderer, widget);
}

int createWhiteButton(WIDGET *widget, SDL_Renderer *renderer) {
    int x = (WINDOW_WIDTH - (2 * BUTTON_WIDTH + BUTTON_MARGIN)) / 2;
    return createButton(x, 180, "./img/white_on.bmp", renderer, widget, true);
}

int createBlackButton(WIDGET *widget, SDL_Renderer *renderer) {
    int x = (WINDOW_WIDTH - (2 * BUTTON_WIDTH + BUTTON_MARGIN)) / 2 + BUTTON_WIDTH + BUTTON_MARGIN;
    return createButton(x, 180, "./img/black.bmp", renderer, widget, false);
}

EVENT_RESPONSE *colorWindowEventHandler(GENERIC_WINDOW *window, SDL_Event *event, CHESS_MATCH *match, MOVES_STACK *stack) {
    WINDOW_TYPE nextWindow = SETTINGS_COLOR_WINDOW;
    int widgetIndex = getClickedWidget(window, event);
    WIDGET *widget = window->widgets[widgetIndex];
    SDL_Renderer *renderer = window->renderer;
    EVENT_RESPONSE *response = createEventResponse(nextWindow, SAME_WINDOW);

    if (widgetIndex == 1) { // White button is clicked
        if (!widget->isActive) {
            toggleButton(widget, renderer);
            toggleButton(window->widgets[2], renderer);
            reRenderWindow(window);
            match->userColor = 1;
        }
    }

    if (widgetIndex == 2) { // Black button is clicked
        if (!widget->isActive) {
            toggleButton(widget, renderer);
            toggleButton(window->widgets[1], renderer);
            reRenderWindow(window);
            match->userColor = 0;
        }
    }

    if (widgetIndex == 3) { // Back button is clicked
        response->status = BACK_WINDOW;
    }

    if (widgetIndex == 4) { // Start button is clicked
        response->windowType = GAME_WINDOW;
        response->status = NEW_WINDOW;
    }

    useMatchAndStack(match, stack);
    // because of the generic infrastructure of the code a match/stack pointers have to be passed to an event handle function
    // the match/stack object is changed by few windows like the load game and setting and game window
    // therefore in order to avoid "unused argument error" we must use the arguments inside the function

    return response;
}