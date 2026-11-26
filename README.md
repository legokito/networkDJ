# networkDJ
DJ together with your friends from anywhere!  

## How it works
Every friend/peer has the same music library installed locally. One peer acts as the master to make decisions and maintain correct state - it broadcasts this state to peers periodically. Other peers send changes as "intents" that are reconciled by the master.  

The master broadcasts a tiny packet containing state information/meta-data. Each peer (including master) has their own local beat-grid to interpret the packet against and play audio locally. This way the network is used for lightweight data transfers and audio output is handled by each peer independently.  

## MVP scope
- controls: play/pause, cue, volume fader, (maybe EQ bands if time permits)
- fixed BPM (128), and each track must start on the downbeat. 
- playback quantized to local grid (for clean/smooth sounding mixes)
- tracks must be 48 kHz WAV 
- linux and macOS only.  

## Notes 
- https://miniaud.io/docs/examples/simple_mixing.html was helpful
