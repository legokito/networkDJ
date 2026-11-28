#include "headers/deck.hh"
#include <algorithm>

bool Deck::is_playing() {
    return grid_anchor_ != -1;
}

const bool Deck::is_playing() const {
    return grid_anchor_ != -1;
}

void Deck::play_pause(Playback& playback) {
    /* use grid to resolve where the anchors should go */
    Grid& grid = playback.get_grid();
    int sample_rate = playback.get_sample_rate();
    int channel_count = playback.get_channel_count();
    int bpm = grid.get_BPM();

    int64_t beat_frame_len = (sample_rate * 60) / bpm;
    int64_t cur_grid_frames = grid.get_total_frame_counter(); 

    /* the modulo things are for quantizing - i just hardcoded it in cause i'm really tired and its 6am. cleaner decomp could defo help */
    if (this->is_playing()) {
        int64_t stored_grid_anchor = grid_anchor_;
        grid_anchor_ = -1;

        if (cur_grid_frames > stored_grid_anchor) {
            local_anchor_ += cur_grid_frames - stored_grid_anchor; 
            local_anchor_ -= local_anchor_ % beat_frame_len;
        }
    }
    else {
        cur_grid_frames = std::max(0, cur_grid_frames-1000); /* 1000 is num of frames grace period for quantizing backward */

        grid_anchor_ = cur_grid_frames + beat_frame_len - (cur_grid_frames % beat_frame_len);
    }
}

void Deck::reset() {
    local_anchor_ = 0;
    grid_anchor_ = -1;

    local_total_frames_ = 0;
}

void Deck::set_volume(float val){
    volume_ = std::clamp(val, 0.0f, 1.0f);
}

void Deck::update_volume(float delta) {
    this->set_volume(volume_ + delta);
}

const float Deck::get_volume() const {
    return volume_;
}

void Deck::set_song_index(int song_index) {
    song_index_ = song_index;
    reset();
}

const int Deck::get_song_index() const {
    return song_index_;
}

// ma_uint64 Deck::get_local_total_frames() {
//     return local_total_frames_;
// }

// void Deck::update_local_total_frames(ma_uint32 frameCount) {
//     local_total_frames_ += frameCount;
// }