#include "headers/playback.hh"

SongMap& Playback::get_song_samples_map() {
    return song_samples_map_;
}

void Playback::process_output(float* output, ma_uint32 frameCount) {
    static ma_uint64 totalFrames = 0;

    for (int deck = 0; deck < NUM_DECKS; deck++) { 
        const std::string& song_name = decks_[deck].get_name(); 

        if (song_name == "") { continue; }

        for (ma_uint32 i = 0; i < frameCount * CHANNEL_COUNT; i += 2) {
            (output)[i] += song_samples_map_[song_name][totalFrames * CHANNEL_COUNT + i];        
            (output)[i+1] += song_samples_map_[song_name][totalFrames * CHANNEL_COUNT + i + 1];
        }
    }

    totalFrames += frameCount;
}

Deck& Playback::get_deck(int index) {
    if (index >= NUM_DECKS) {
        std::cerr << "invalid deck index";
        return nullptr;
    }
    else {
        return decks_[index];
    }
}