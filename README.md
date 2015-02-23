![](assets/buccaneer.png?raw=true)

This is a library collection of units for [RackoonIO](http://www.github.com/carrotsrc/rackoonIO/).

## Processing Units

### RuFlacLoad
This is used to load whole flacs into memory and read them as and when required. This uses libsndfile to decode the flac (See below)

### RuLevels
The beginnings of a Levels unit for controlling the levels of the sound - currently only handles the gain on a particular channel and a volume fader

### RuLoop
An experimental loop unit that can start recording the stream and loop it back live from memory. Heavily unoptimised and will hog CPU at the moment

### RuChannelMixer
A two channel mixer

### RuEcho
My first attempt at an audio effect - a delay, feedback function

### RuPitchBender
An experimental resampler which uses libResample (see below)

### RuAlsa
An ALSA handle and buffer which acts as the end point of a daisychain

## License

This is distributed under the terms of LGPL v3
libSndfile and libResample which are linked to at runtime, these are also LGPL (see in license/ for details)

# Credit

- libSndfile from [MegaNerd](http://www.mega-nerd.com/libsndfile/)
- libResample by Dominic Mazzoni, original based on resample-1.7 (http://www-ccrma.stanford.edu/~jos/resample/). It's not in the source tree because it is built separately as a shared library.
- Amaranth font for the logo