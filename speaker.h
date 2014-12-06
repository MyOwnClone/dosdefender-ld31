#pragma once

#include "int.h"
#include "tone.h"

struct note {
    unsigned frequency;
    uint16_t length;
};

struct sample {
    int priority;
    size_t length;
    struct note notes[];
};

struct sample fx_intro_music = {
    .priority = 100,
    .length = 11,
    {
        {262, 10},
        {247, 10},
        {220, 10},
        {196, 10},
        {247, 10},
        {220, 10},
        {196, 10},
        {247, 10},
        {220, 10},
        {0,   10},
        {110, 20},
    }
};

struct sample fx_end_music = {
    .priority = 200,
    .length = 15,
    {
        {62, 50},
        {0,  2},
        {62, 50},
        {0,  2},
        {62, 50},
        {0,  2},
        {73, 25},
        {0,  2},
        {65, 20},
        {0,  2},
        {62, 50},
        {0,  2},
        {55, 50},
        {0,  2},
        {62, 100},
    }
};

struct sample fx_explode = {
    .priority = 10,
    .length = 11,
    {
        {62, 1},
        {47, 1},
        {20, 1},
        {96, 1},
        {47, 1},
        {20, 1},
        {96, 1},
        {47, 1},
        {20, 1},
        {89, 1},
        {60, 1},
    }
};

struct sample fx_fire0 = {
    .priority = 0,
    .length = 4,
    {
        {200, 1},
        {190, 1},
        {180, 1},
        {170, 1}
    }
};

struct sample fx_fire1 = {
    .priority = -10,
    .length = 5,
    {
        {150, 1},
        {120, 1},
        {100, 1},
        {120, 1},
        {135, 1}
    }
};

struct sample fx_hit = {
    .priority = 5,
    .length = 5,
    {
        {120, 3},
        {130, 3},
        {140, 3},
        {150, 3},
        {160, 3}
    }
};

struct speaker {
    struct sample *sample;
    int index, step;
};

void speaker_play(struct speaker *speaker, struct sample *sample)
{
    if (!speaker->sample || speaker->sample->priority < sample->priority) {
        speaker->sample = sample;
        speaker->index = -1;
        speaker->step = 0;
    }
}

void speaker_step(struct speaker *speaker)
{
    if (speaker->sample) {
        if (speaker->step) {
            speaker->step--;
        } else {
            speaker->index++;
            if (speaker->index >= speaker->sample->length) {
                speaker->sample = 0;
                tone_off();
            } else {
                struct note note = speaker->sample->notes[speaker->index];
                if (note.frequency == 0)
                    tone_off();
                else {
                    tone(note.frequency);
                    tone_on();
                }
                speaker->step = note.length;
            }
        }
    }
}