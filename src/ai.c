#include "ai.h"
#include "game.h"
#include "deck.h"
#include <SDL.h>

static AIThinkState g_aiThink = {0, 0, -1};

static Color best_wild_color(const Player *p, int excludeIndex) {
    int counts[4] = { 0, 0, 0, 0 };
    for (int i = 0; i < p->handCount; i++) {
        if (i == excludeIndex) continue;
        Card c = p->hand[i];
        if (c.color <= COL_BLUE) counts[c.color]++;
    }
    int best = 0;
    for (int i = 1; i < 4; i++) if (counts[i] > counts[best]) best = i;
    return (Color)best;
}

static int score_card(const GameState *state, const Player *p, int idx, int dangerOpponentLow) {
    Card c = p->hand[idx];
    int score = 0;

    if (c.rank <= RANK_9) {
        score = 10 + (int)c.rank;
    } else if (c.rank == RANK_DRAW2 || c.rank == RANK_SKIP || c.rank == RANK_REVERSE) {
        score = 30;
        if (dangerOpponentLow) score += 25;
    } else if (c.rank == RANK_WILD) {
        score = 20;
    } else if (c.rank == RANK_WILD4) {
        score = dangerOpponentLow ? 60 : 15;
    }

    int sameColor = 0;
    if (c.color <= COL_BLUE) {
        for (int i = 0; i < p->handCount; i++) {
            if (p->hand[i].color == c.color) sameColor++;
        }
        score += sameColor;
    }

    (void)state;
    return score;
}

void ai_take_turn(GameState *state, int playerIndex) {
    if (state->gameOver) return;
    if (playerIndex != state->currentPlayer) {
        if (g_aiThink.targetPlayerIndex == playerIndex) {
            g_aiThink.isThinking = 0;
            g_aiThink.targetPlayerIndex = -1;
        }
        return;
    }

    uint32_t currentTicks = SDL_GetTicks();

    if (!g_aiThink.isThinking || g_aiThink.targetPlayerIndex != playerIndex) {
        g_aiThink.isThinking = 1;
        g_aiThink.targetPlayerIndex = playerIndex;
        g_aiThink.thinkStartTicks = currentTicks;
        return;
    }

    if (currentTicks - g_aiThink.thinkStartTicks < 1000) {
        return;
    }

    g_aiThink.isThinking = 0;
    g_aiThink.targetPlayerIndex = -1;

    Player *self = &state->players[playerIndex];

    int dangerLow = 0;
    for (int i = 0; i < state->playerCount; i++) {
        if (i == playerIndex) continue;
        if (state->players[i].handCount <= 2) dangerLow = 1;
    }

    int bestIdx = -1, bestScore = -1000;
    for (int i = 0; i < self->handCount; i++) {
        if (!game_card_playable(state, self->hand[i])) continue;
        int s = score_card(state, self, i, dangerLow);
        if (s > bestScore) { bestScore = s; bestIdx = i; }
    }

    if (bestIdx < 0) {
        game_draw_card(state, playerIndex);

        if (state->currentPlayer == playerIndex && self->handCount > 0) {
            int lastIdx = self->handCount - 1;
            if (game_card_playable(state, self->hand[lastIdx])) {
                Color wildColor = COL_RED;
                if (card_is_wild(self->hand[lastIdx])) {
                    wildColor = best_wild_color(self, lastIdx);
                }
                game_play_card(state, playerIndex, lastIdx, wildColor);
            }
        }
        return;
    }

    Color wildColor = COL_RED;
    if (card_is_wild(self->hand[bestIdx])) {
        wildColor = best_wild_color(self, bestIdx);
    }

    if (self->handCount == 2) {
        game_call_uno(state, playerIndex);
    }

    game_play_card(state, playerIndex, bestIdx, wildColor);
}

// Temporary stub function, needs a complete rewrite to expose the bluffer's hand + force them to draw 4 cards.
void ai_maybe_catch_uno(GameState *state, int aiPlayerIndex) {
    if (state->gameOver) return;
    for (int i = 0; i < state->playerCount; i++) {
        if (i == aiPlayerIndex) continue;
        Player *t = &state->players[i];
        if (t->handCount == 1 && !t->saidUno) {
            if (rng_range(&state->rng, 0, 99) < 35) {
                game_catch_uno_failure(state, aiPlayerIndex, i);
            }
        }
    }
}
