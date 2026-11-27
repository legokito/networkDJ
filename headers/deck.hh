#pragma once

#include "third_party/miniaudio.h"
#include <string>
#include <vector>

class Deck {
private:
    std::atomic<float> volume_ = 0;
    std::atomic<size_t> local_anchor_ = 0;
    std::atomic<ssize_t> grid_anchor_ = -1; // -1 means not playing
    std::atomic<int> song_index_ = -1;

    std::atomic<ma_uint64> local_total_frames_ = 0;

public:
    bool is_playing();

    void play_pause();
    void reset();

    void set_volume(float val);
    void update_volume(float delta);
    const float get_volume() const;

    ma_uint64 get_local_total_frames();
    void update_local_total_frames(ma_uint32 frameCount);

    void set_song_index(int song_index);
    const int get_song_index() const;
};