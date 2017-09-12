//
// Created by Ido on 9/9/17.
//

#include "GenericWindow.h"

GENERIC_WINDOW *createGenericWindow(int(*drawFunc)(GENERIC_WINDOW*, SDL_Window *, SDL_Renderer *renderer), SDL_Window *sdlWindow, SDL_Renderer *renderer) {
    GENERIC_WINDOW *genericWindow = (GENERIC_WINDOW *) calloc(sizeof(GENERIC_WINDOW), sizeof(char));
    if (genericWindow == NULL) return NULL;

    (*drawFunc)(genericWindow, sdlWindow, renderer);
    return genericWindow;
}

void destroyWindow(GENERIC_WINDOW *genericWindow) {
    if (genericWindow == NULL) return;

    for (int i = 0; i < genericWindow->numWidgets; ++i) {
        destroyWidget(genericWindow->widgets[i]);
    }
    free(genericWindow->widgets);
    free(genericWindow);
}

int destroyWindowOnFailure(GENERIC_WINDOW *window, int numWidgetsCreated) {
    SDL_DestroyRenderer(window->renderer);
    for (int i = 0; i < numWidgetsCreated; i++) {
        destroyWidget(window->widgets[i]);
    }
    free(window->widgets);
    window->window = NULL;
    SDL_DestroyWindow(window->window);
    SDL_Quit();
    return -1;
}

int exitEventHandler(SDL_Event *event) {
    return 0;
}

int loadGameEventHandler(SDL_Event *event) {
    return 0;
}

int detectWidgetByLocation(GENERIC_WINDOW *window ,int x, int y) {
    int widgetNum = -1;

    for (int i = 0; i < window->numWidgets; ++i) {
        if (window->widgets[i] == NULL) continue; // If a widget is NULL, it cannot be a detected one.

        int widgetX = window->widgets[i]->rect.x;
        int widgetY = window->widgets[i]->rect.y;
        int widgetW = window->widgets[i]->rect.w;
        int widgetH = window->widgets[i]->rect.h;

        if (window->widgets[i]->isClickable) { // Only clickable widgets can be detected
            if (x >= widgetX && x <= widgetX + widgetW && y >= widgetY && y <= widgetY +widgetH) {
                widgetNum = i;
                break;
            }
        }
    }

    return widgetNum;
}

int getClickedWidget(GENERIC_WINDOW *window, SDL_Event *event) {
    if (window == NULL || event == NULL) return -1;
    int widgetIndex = detectWidgetByLocation(window, event->button.x, event->button.y);

    return widgetIndex;
}

void renderWindowWidgets(GENERIC_WINDOW *window) {
    for (int i = 0; i < window->numWidgets; ++i) {
        SDL_RenderCopy(window->renderer, window->widgets[i]->texture, NULL, &(window->widgets[i]->rect));
    }
}

void reRenderWindow(GENERIC_WINDOW *window) {
    SDL_SetRenderDrawColor(window->renderer, COLOR_WHITE);
    SDL_RenderClear(window->renderer);
    // Place images on screen
    renderWindowWidgets(window);
    SDL_RenderPresent(window->renderer);
}

EVENT_RESPONSE *createEventResponse(GENERIC_WINDOW *window, RESPONSE_STATUS status) {
    if (window == NULL) return NULL;

    EVENT_RESPONSE *response = (EVENT_RESPONSE *) malloc(sizeof(EVENT_RESPONSE));
    if (response == NULL) return NULL;

    response->status = status;
    response->window = window;
    return response;
}

void destroyEventResponse(EVENT_RESPONSE *response) {
    free(response);
}

int findActiveButton(GENERIC_WINDOW *window) {
    int activeButton = -1;
    for (int i = 0; i < window->numWidgets; ++i) {
        WIDGET *widget = window->widgets[i];
        if (widget->isActive) return i;
    }
    return activeButton;
}
