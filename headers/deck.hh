#pragma once

#include <string>
#include <vector>

#define MAX_SONG_LENGTH 44100 * 2 * 300  // 300 seconds = 5 minutes

class Deck {
private:
    float volume_ = 1;
    size_t local_anchor_ = 0;
    ssize_t grid_anchor_ = -1; // -1 means not playing
    std::string file_name_ = "";
    std::vector<float> song_vector_{};

public:
    Deck() {
        song_vector_.resize(MAX_SONG_LENGTH);
    }

    void set_song(std::string song_name);




};