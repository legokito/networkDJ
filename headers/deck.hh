#pragma once

#include <string>
#include <vector>

class Deck {
private:
    float volume_ = 0;
    size_t local_anchor_ = 0;
    ssize_t grid_anchor_ = -1; // -1 means not playing
    std::string song_name_ = "";

public:
    bool is_playing();

    void play_pause();
    void reset();

    void set_volume(float val);
    float get_volume();

    void set_name(std::string& song_name);
    const std::string& get_name();
};