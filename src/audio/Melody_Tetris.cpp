#include "audio/Melodies.h"

// Melody: Tetris (Korobeiniki) (~5 seconds)
static const Note TETRIS_NOTES[] = {
    { 659, 250 }, { 494, 125 }, { 523, 125 }, { 587, 250 },
    { 523, 125 }, { 494, 125 }, { 440, 250 }, { 440, 125 },
    { 523, 125 }, { 659, 250 }, { 587, 125 }, { 523, 125 },
    { 494, 375 }, { 523, 125 }, { 587, 250 }, { 659, 250 },
    { 523, 250 }, { 440, 250 }, { 440, 500 }
};

const Melody Melodies::Tetris = {
    TETRIS_NOTES,
    sizeof(TETRIS_NOTES) / sizeof(TETRIS_NOTES[0]),
    30
};
