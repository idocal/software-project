//
// Created by Ido on 5/30/17.
//

#include "SPMiniMaxNode.h"
#include <stdio.h>

node *createNode(SPFiarGame *game, int depth, int moveColumn, int maxDepth) {
    node *nodePointer = (node *) malloc(sizeof(node));
    if (nodePointer == NULL) {
        printf("Error: createNode has failed\n");
        return NULL;
    }

    nodePointer->game = spFiarGameCopy(game);
    if (nodePointer->game == NULL) {
        free(nodePointer);
        return NULL;
    }

    SP_FIAR_GAME_MESSAGE gameMove = spFiarGameSetMove(nodePointer->game, moveColumn);
    if (gameMove != SP_FIAR_GAME_SUCCESS) {
        nodeDestroy(nodePointer);
        return NULL;
    }

    for (int i=0; i<7; i++) {
        nodePointer->children[i] = NULL;
    }

    nodePointer->depth = depth;
    nodePointer->score = 0;
    if (depth == maxDepth || spFiarCheckWinner(nodePointer->game) != '\0') {
        nodePointer->isLeaf = true;
    } else {
        nodePointer->isLeaf = false;
    }

    return nodePointer;
}

node *createRoot(SPFiarGame *game) {
    node *nodePointer = (node *) malloc(sizeof(node));
    if (nodePointer == NULL) {
        printf("Error: createRoot has failed\n");
        return NULL;
    }

    nodePointer->game = spFiarGameCopy(game);
    if (nodePointer->game == NULL) {
        free(nodePointer);
        return NULL;
    }

    for (int i=0; i<7; i++) {
        nodePointer->children[i] = NULL;
    }

    nodePointer->depth = 0;
    nodePointer->score = 0;
    nodePointer->isLeaf = false;

    return nodePointer;
}

void nodeDestroy(node *nodePointer) {
    spFiarGameDestroy(nodePointer->game);
    free(nodePointer);
}

