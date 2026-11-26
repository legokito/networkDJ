#pragma once

#include "third_party/miniaudio.h"
#include "headers/deck.hh"

using SongMap = std::unordered_map<std::string, std::vector<float>>;

constexpr int NUM_DECKS 2;

class Playback {
private:
    std::array<Deck, NUM_DECKS> decks_{};
    SongMap song_samples_map_{};

public:
    SongMap& get_song_samples_map();
    void process_output(float* output, ma_uint32 frameCount);
    Deck& get_deck(int index);
};
