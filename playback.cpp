#include "headers/playback.hh"
#include "headers/deck.hh"

#include <cassert>

SongMap& Playback::get_song_samples_map() {
    return song_samples_map_;
}

std::vector<std::string>& Playback::get_song_names() {
    return song_names_;
}

void Playback::process_output(float* output, ma_uint32 frameCount, int CHANNEL_COUNT) {
    /* play state checking and bounds checking */

    for (int deck = 0; deck < NUM_DECKS; deck++) { 
        const int song_index = decks_[deck].get_index();  

        /* no song selected */
        if (song_index == -1) { continue; }

        /* not currently playing */
        if (!decks_[deck].is_playing()) {
            continue;
        }

        const std::string& song_name = song_names_[song_index];

        ma_uint64 local_total_frames = decks_[deck].get_local_total_frames();
        float volume = decks_[deck].get_volume();
        ma_uint64 song_size = song_samples_map_[song_name].size();

        for (ma_uint32 i = 0; i < frameCount * CHANNEL_COUNT; i += 2) {
            /* finished song */
            if (local_total_frames * CHANNEL_COUNT + i >= song_size) {
                decks_[deck].play_pause();
                break;
            }

            (output)[i] += song_samples_map_[song_name][local_total_frames * CHANNEL_COUNT + i] * volume; /* have function to do the dsp */       
            (output)[i+1] += song_samples_map_[song_name][local_total_frames * CHANNEL_COUNT + i + 1] * volume;
        }

        decks_[deck].update_local_total_frames(frameCount);
    }

}

Deck& Playback::get_deck(int index) {
    assert(index < NUM_DECKS);
    return decks_[index];
}