#include "headers/playback.hh"
#include "headers/deck.hh"

#include <cassert>

Playback::Playback(int CHANNEL_COUNT, int SAMPLE_RATE) : 
    CHANNEL_COUNT_(CHANNEL_COUNT), SAMPLE_RATE_(SAMPLE_RATE) 
{}

SongMap& Playback::get_song_samples_map() {
    return song_samples_map_;
}

std::vector<std::string>& Playback::get_song_names() {
    return song_names_;
}

const std::vector<std::string>& Playback::get_song_names() const {
    return song_names_;
}

void Playback::process_output(float* output, ma_uint32 frameCount) {
    /* play state checking and bounds checking - done */

    int grid_interval = (SAMPLE_RATE_ / grid_.get_BPM()) * 60;
    size_t total_frames = grid_.get_total_frame_counter();

    for (int deck = 0; deck < NUM_DECKS; deck++) { 
        const int song_index = decks_[deck].get_song_index();  

        /* no song selected */
        if (song_index == -1) { continue; }

        /* not currently playing */
        if (!decks_[deck].is_playing()) {
            continue;
        }

        const std::string& song_name = song_names_[song_index];

        // ma_uint64 local_total_frames = decks_[deck].get_local_total_frames();

        float volume = decks_[deck].get_volume();
        ma_uint64 song_size = song_samples_map_[song_name].size();

        for (ma_uint32 i = 0; i < frameCount * CHANNEL_COUNT_; i += 2) {
            /* finished song */
            if (local_total_frames * CHANNEL_COUNT_ + i >= song_size) {
                decks_[deck].play_pause(grid_); 
                break;
            }

            (output)[i] += song_samples_map_[song_name][local_total_frames * CHANNEL_COUNT_ + i] * volume; /* have function to do the dsp */       
            (output)[i+1] += song_samples_map_[song_name][local_total_frames * CHANNEL_COUNT_ + i + 1] * volume;
        }

        // decks_[deck].update_local_total_frames(frameCount);
    }

    grid_.update_total_frame_counter(frameCount);
}

Deck& Playback::get_deck(int index) {
    assert(index < NUM_DECKS);
    return decks_[index];
}
 
const Deck& Playback::get_deck(int index) const {
    assert(index < NUM_DECKS);
    return decks_[index];
}

int Deck::get_channel_count() {
    return CHANNEL_COUNT_;
}

const int Deck::get_channel_count() const {
    return CHANNEL_COUNT_;
}

int Deck::get_sample_rate() {
    return SAMPLE_RATE_;
}

const int Deck::get_sample_rate() const {
    return SAMPLE_RATE_;
}

Grid& Deck::get_grid() {
    return grid_;
}