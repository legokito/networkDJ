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

    int64_t total_frames = grid_.get_total_frame_counter();

    for (int deck = 0; deck < NUM_DECKS; deck++) { 
        const int song_index = decks_[deck].get_song_index();  

        /* no song selected */
        if (song_index == -1) { continue; }

        /* not currently playing */
        if (!decks_[deck].is_playing()) {
            continue;
        }

        const std::string& song_name = song_names_[song_index];

        std::vector<float>& vec = song_samples_map_[song_name];
        int64_t song_size = vec.size();

        float volume = decks_[deck].get_volume();
        int64_t local_anchor = decks_[deck].get_local_anchor();
        int64_t grid_anchor = decks_[deck].get_grid_anchor();

        /* scheduled for later - clips songs more often than it needs to; not priority to fix */
        if (grid_anchor > total_frames) { 
            continue;
        }

        int64_t actual_local_position = total_frames - grid_anchor + local_anchor;

        for (ma_uint32 i = 0; i < frameCount * CHANNEL_COUNT_; i += 2) {
            /* finished song */
            if (actual_local_position * CHANNEL_COUNT_ + i >= song_size) {
                /* says currently playing even when track is finished; not priority to fix */ 
                break;
            }

            (output)[i] += vec[actual_local_position * CHANNEL_COUNT_ + i] * volume; /* have function to do the dsp */       
            (output)[i+1] += vec[actual_local_position * CHANNEL_COUNT_ + i + 1] * volume;
        }

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

int Playback::get_channel_count() {
    return CHANNEL_COUNT_;
}

const int Playback::get_channel_count() const {
    return CHANNEL_COUNT_;
}

int Playback::get_sample_rate() {
    return SAMPLE_RATE_;
}

const int Playback::get_sample_rate() const {
    return SAMPLE_RATE_;
}

Grid& Playback::get_grid() {
    return grid_;
}