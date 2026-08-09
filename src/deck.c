#include "deck.h"
#include <string.h>

void deck_build_standard(GameState *state) {
    int n = 0;
    Color colors[4] = { COL_RED, COL_YELLOW, COL_GREEN, COL_BLUE };

    for (int c = 0; c < 4; c++) {
        state->drawPile[n].color = colors[c];
        state->drawPile[n].rank  = RANK_0;
        n++;

        for (int r = RANK_1; r <= RANK_9; r++) {
            for (int copy = 0; copy < 2; copy++) {
                state->drawPile[n].color = colors[c];
                state->drawPile[n].rank  = (Rank)r;
                n++;
            }
        }

        Rank actions[3] = { RANK_SKIP, RANK_REVERSE, RANK_DRAW2 };
        for (int a = 0; a < 3; a++) {
            for (int copy = 0; copy < 2; copy++) {
                state->drawPile[n].color = colors[c];
                state->drawPile[n].rank  = actions[a];
                n++;
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        state->drawPile[n].color = COL_WILD;
        state->drawPile[n].rank  = RANK_WILD;
        n++;
    }
    for (int i = 0; i < 4; i++) {
        state->drawPile[n].color = COL_WILD;
        state->drawPile[n].rank  = RANK_WILD4;
        n++;
    }

    state->drawCount = (uint8_t)n;
    state->discardCount = 0;
}

void deck_shuffle(GameState *state) {
    for (int i = state->drawCount - 1; i > 0; i--) {
        int j = rng_range(&state->rng, 0, i);
        Card tmp = state->drawPile[i];
        state->drawPile[i] = state->drawPile[j];
        state->drawPile[j] = tmp;
    }
}

const char *card_rank_short(Rank r) {
    switch (r) {
        case RANK_0: return "0";
        case RANK_1: return "1";
        case RANK_2: return "2";
        case RANK_3: return "3";
        case RANK_4: return "4";
        case RANK_5: return "5";
        case RANK_6: return "6";
        case RANK_7: return "7";
        case RANK_8: return "8";
        case RANK_9: return "9";
        case RANK_SKIP: return "SKIP";
        case RANK_REVERSE: return "REV";
        case RANK_DRAW2: return "+2";
        case RANK_WILD: return "WILD";
        case RANK_WILD4: return "+4";
        default: return "?";
    }
}

const char *color_name(Color c) {
    switch (c) {
        case COL_RED: return "RED";
        case COL_YELLOW: return "YELLOW";
        case COL_GREEN: return "GREEN";
        case COL_BLUE: return "BLUE";
        case COL_WILD: return "WILD";
        default: return "";
    }
}

int card_is_wild(Card c) {
    return c.rank == RANK_WILD || c.rank == RANK_WILD4;
}

int card_is_action(Card c) {
    return c.rank == RANK_SKIP || c.rank == RANK_REVERSE ||
           c.rank == RANK_DRAW2 || card_is_wild(c);
}

int card_score_value(Card c) {
    if (c.rank <= RANK_9) return (int)c.rank;
    if (c.rank == RANK_SKIP || c.rank == RANK_REVERSE || c.rank == RANK_DRAW2) return 20;
    if (c.rank == RANK_WILD || c.rank == RANK_WILD4) return 50;
    return 0;
}
