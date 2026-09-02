#include "audio/Melodies.h"

// Melody: Mario Coin
static const Note MARIO_COIN_NOTES[] = {
    { 988, 100 },
    { 1319, 400 }
};

const Melody Melodies::MarioCoin = {
    MARIO_COIN_NOTES,
    sizeof(MARIO_COIN_NOTES) / sizeof(MARIO_COIN_NOTES[0]),
    50
};
