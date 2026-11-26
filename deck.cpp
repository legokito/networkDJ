#include "headers/deck.hh"

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
}

void Deck::set_volume(float val){
    volume_ = val;
    volume_ = volume_ > 1 ? 1 : volume_;
    volume_ = volume_ < 0 ? 0 : volume_;
}

float Deck::get_volume() {
    return volume_;
}

void Deck::set_name(std::string& song_name) {
    song_name_ = song_name;
}

const std::string& Deck::get_name() {
    return song_name_;
}