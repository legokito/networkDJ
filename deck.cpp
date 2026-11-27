#include "headers/deck.hh"
#include <algorithm>

bool Deck::is_playing() {
    return grid_anchor_ != -1;
}

void Deck::play_pause() {
    if (this->is_playing()) {
        // local_anchor_ = /* cur grid value */ - grid_anchor_; 
        grid_anchor_ = -1;
    }
    else {
        grid_anchor_ = 0;
        // grid_anchor_ = /* cur grid value */;
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
}

const int Deck::get_song_index() const {
    return song_index_;
}

ma_uint64 Deck::get_local_total_frames() {
    return local_total_frames_;
}

void Deck::update_local_total_frames(ma_uint32 frameCount) {
    local_total_frames_ += frameCount;
}