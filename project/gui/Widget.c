//
// Created by Ido on 9/9/17.
//

#include "Widget.h"

int loadTexture(WIDGET *widget, char *originalImgPath, SDL_Renderer *renderer);

WIDGET *createWidget(int(*createWidgetFunc)(WIDGET *, SDL_Renderer *), SDL_Renderer *renderer) {
    WIDGET *widget = (WIDGET *) calloc(sizeof(WIDGET), sizeof(char));
    if (widget == NULL) return NULL;

    int res = (*createWidgetFunc)(widget, renderer);
    if (res == -1) return NULL;

    widget->isActive = false;
    widget->isEnable = true;
    return widget;
}

void destroyWidget(WIDGET *widget) {
    if (widget == NULL) return;
    SDL_DestroyTexture(widget->texture);
    free(widget);
}

int createButton(int x, int y, char *imgPath, SDL_Renderer *renderer, WIDGET *widget) {
    SDL_Surface *loadingSurface = NULL;
    strcpy(widget->imgPath, imgPath);

    // WIDGET rect
    SDL_Rect rect = {.x = x, .y = y, .w = BUTTON_WIDTH, .h = BUTTON_HEIGHT};
    widget->rect = rect;

    // Load new texture with imgPath updated
    loadTexture(widget, widget->imgPath, renderer);

    return 1;
}

int createTitle(char *imgPath, SDL_Renderer *renderer, WIDGET *widget) {
    SDL_Surface *loadingSurface = NULL;
    strcpy(widget->imgPath, imgPath);

    // WIDGET rect
    SDL_Rect rect = {.x = (WINDOW_WIDTH - TITLE_WIDTH) / 2, .y = PAGE_MARGIN, .w = TITLE_WIDTH, .h = TITLE_HEIGHT};
    widget->rect = rect;

    // Load new texture with imgPath updated
    loadTexture(widget, widget->imgPath, renderer);

    return 1;
}

void toggleButton(WIDGET *widget, SDL_Renderer *renderer) {
    // Save previous img path to restore in case of failure
    char originalImgPath[1024];
    strcpy(originalImgPath,widget->imgPath);

    if (!widget->isActive) {
        // Change image path to _on.bmp
        char *imgPath = widget->imgPath;
        imgPath += 2; // incrementing pointer by two to avoid previous folder prefix
        while (*imgPath != '.') {
            imgPath++;
        }
        *imgPath = '\0';
        char *imgNewSuffix = "_on.bmp";
        strcat(widget->imgPath, imgNewSuffix);
    }

    else { // Widget is inactive
        // Change image path to .bmp
        char *imgPath = widget->imgPath;
        imgPath += 2; // incrementing pointer by two to avoid previous folder prefix
        while (*imgPath != '.') {
            imgPath++;
        }
        imgPath -= 3; // go backwards 3 chars to substitute the "_on" suffix
        *imgPath = '\0';
        char *imgNewSuffix = ".bmp";
        strcat(widget->imgPath, imgNewSuffix);
    }

    // Load new texture with imgPath updated
    loadTexture(widget, originalImgPath, renderer);

    // Widget is now active
    widget->isActive = !widget->isActive;
}

void toggleButtonAbility(WIDGET *widget, SDL_Renderer *renderer) {
    // Save previous img path to restore in case of failure
    char originalImgPath[1024];
    strcpy(originalImgPath,widget->imgPath);

    if (widget->isEnable) {
        // Change image path to _disabled.bmp
        char *imgPath = widget->imgPath;
        imgPath += 2; // incrementing pointer by two to avoid previous folder prefix
        while (*imgPath != '.') {
            imgPath++;
        }
        *imgPath = '\0';
        char *imgNewSuffix = "_disabled.bmp";
        strcat(widget->imgPath, imgNewSuffix);
    }
    else { // Widget is disabled
        // Change image path to .bmp
        char *imgPath = widget->imgPath;
        imgPath += 2; // incrementing pointer by two to avoid previous folder prefix
        while (*imgPath != '.') {
            imgPath++;
        }
        imgPath -= 9; // go backwards 9 chars to substitute the "_on" suffix
        *imgPath = '\0';
        char *imgNewSuffix = ".bmp";
        strcat(widget->imgPath, imgNewSuffix);
    }

    // Load new texture with imgPath updated
    loadTexture(widget, originalImgPath, renderer);

    // Widget is now disabled
    widget->isEnable = !widget->isEnable;
}

int loadTexture(WIDGET *widget, char *originalImgPath, SDL_Renderer *renderer) {
    // Reload texture
    SDL_Surface *loadingSurface = SDL_LoadBMP(widget->imgPath);
    if (loadingSurface == NULL) {
        strcpy(widget->imgPath, originalImgPath); // revert to previous img path
        return -1;
    }

    // Widget texture
    SDL_Texture *prevTexture = widget->texture;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, loadingSurface);
    if (texture == NULL) {
        SDL_FreeSurface(loadingSurface);
        strcpy(widget->imgPath, originalImgPath); // revert to previous img path
        return -1;
    }

    widget->texture = texture;
    SDL_DestroyTexture(prevTexture); // Destroy previous texture
    SDL_FreeSurface(loadingSurface);
}