#pragma once

#include "third_party/miniaudio.h"
#include "headers/deck.hh"
#include <unordered_map>

using SongMap = std::unordered_map<std::string, std::vector<float>>;

constexpr int NUM_DECKS = 2;

class Playback {
private:
    std::array<Deck, NUM_DECKS> decks_{}; /* initialized with 2 decks */
    SongMap song_samples_map_{};
    std::vector<std::string> song_names_{};

public:
    SongMap& get_song_samples_map();
    void process_output(float* output, ma_uint32 frameCount, int CHANNEL_COUNT);

    Deck& get_deck(int index);
    const Deck& get_deck(int index) const; 

    std::vector<std::string>& get_song_names();
    const std::vector<std::string>& get_song_names() const;
};
