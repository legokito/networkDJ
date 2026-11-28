#include "headers/grid.hh"

int64_t Grid::get_total_frame_counter() {
    return total_frame_counter_;
}

const int64_t Grid::get_total_frame_counter() const {
    return total_frame_counter_;
}
    
void Grid::set_total_frame_counter(int64_t frames) {
    total_frame_counter_ = frames;
}

void Grid::update_total_frame_counter(int64_t frames) {
    total_frame_counter_ += frames;
}

int Grid::get_BPM() {
    return BPM;
}