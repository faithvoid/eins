#ifndef UNO_AI_H
#define UNO_AI_H

#include "types.h"
#include <stdint.h>

typedef struct {
    int isThinking;
    uint32_t thinkStartTicks;
    int targetPlayerIndex;
} AIThinkState;

void ai_take_turn(GameState *state, int playerIndex);
void ai_maybe_catch_uno(GameState *state, int aiPlayerIndex);

#endif
