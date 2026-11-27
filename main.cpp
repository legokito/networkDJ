#include "third_party/miniaudio.h"
#include "headers/tui.hh"
#include "headers/deck.hh"
#include "headers/playback.hh"
#include <ncurses.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
 

#define SAMPLE_FORMAT   ma_format_f32
#define CHANNEL_COUNT   2
#define SAMPLE_RATE     48000

namespace fs = std::filesystem;

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    // In playback mode copy data to pOutput. In capture mode read data from pInput. In full-duplex mode, both
    // pOutput and pInput will be valid and you can move data from pInput into pOutput. Never process more than
    // frameCount frames.


    auto pb = static_cast<Playback*>(pDevice->pUserData);
    float* pOutputF32 = static_cast<float*>(pOutput);

    pb->process_output(pOutputF32, frameCount, CHANNEL_COUNT);
}

int main(int argc, char* argv[])
{
    const fs::path song_folder("./songs");

    /* invalid folder - quit */
    if (!fs::exists(song_folder) || !fs::is_directory(song_folder)) {
        std::cerr << "invalid folder";
        return 1;
    }

    Playback playback;

    /* setup audio callback and output device */
    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format   = SAMPLE_FORMAT;   // Set to ma_format_unknown to use the device's native format.
    config.playback.channels = CHANNEL_COUNT;               // Set to 0 to use the device's native channel count.
    config.sampleRate        = SAMPLE_RATE;           // Set to 0 to use the device's native sample rate.
    config.dataCallback      = data_callback;   // This function will be called when miniaudio needs more data.
    config.pUserData         = &playback;   // Can be accessed from the device object (device.pUserData).

    ma_device device;
    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) {
        return -1;  // Failed to initialize the device.
    }

    /* decode and load every song into memory */
    ma_decoder_config decoder_config = ma_decoder_config_init(SAMPLE_FORMAT, CHANNEL_COUNT, SAMPLE_RATE);

    SongMap& song_samples_map = playback.get_song_samples_map();
    std::vector<std::string>& song_names_ = playback.get_song_names();
    for (auto const& song : fs::directory_iterator{song_folder}) {
        if (song.path().extension() != ".wav") { continue; }

        ma_decoder decoder;
        ma_result result; 

        result = ma_decoder_init_file(song.path().c_str(), &decoder_config, &decoder);
        if (result != MA_SUCCESS) {
            std::cerr << "error in initializing decoder";
            return -1;
        } 
   
        ma_uint64 total_frames;
        result = ma_decoder_get_length_in_pcm_frames(&decoder, &total_frames);
        if (result != MA_SUCCESS) {
            std::cerr << "error in getting total frame length";
            return -1;
        }

        std::string song_name = song.path().filename().string();
        song_names_.push_back(song_name); 
        song_samples_map[song_name].resize(total_frames * CHANNEL_COUNT);

        ma_uint64 frames_read;
        result = ma_decoder_read_pcm_frames(&decoder, song_samples_map[song_name].data(), total_frames, &frames_read); 
        if (result != MA_SUCCESS || total_frames != frames_read) {
            std::cerr << "error in reading in pcm frames into map";
            return -1;
        }

        ma_decoder_uninit(&decoder);
    }

    /* ncurses tui setup */
    Tui tui;

    /* main loop */
    ma_device_start(&device);    /* start audio callback */ 

    while (true) {
        tui.get_key(playback);
        tui.redraw_ui(playback);
    }

    ma_device_uninit(&device); /* end audio callback */

    return 0;
}