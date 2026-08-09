#ifndef UNO_DECK_H
#define UNO_DECK_H

#include "types.h"

void deck_build_standard(GameState *state);

void deck_shuffle(GameState *state);

const char *card_rank_short(Rank r);
const char *color_name(Color c);

int card_is_wild(Card c);
int card_is_action(Card c);
int card_score_value(Card c);

#endif
