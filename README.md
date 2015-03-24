![](assets/rackscallion.png?raw=true)

This is a library collection of units for [RackoonIO](http://www.github.com/carrotsrc/rackoonIO/).

**Most units working with RackoonIO -- RuLoop still dodge**

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


## Building

Rackscallion and the tests are two separate build scripts. Depending on whether you're in the repository root directory or the tests/ directory, the process is still the same:

` ccmake . `

Since the includes and the library are not going to be in the usual place for the system - set the path (absolute or relative) for them respectively using RACKOONIO_LIB/INC configuration options

LIBRSAMPLE setting will enable the compilation of the (disastriously experimental) PitchBend unit. Probably best to avoid building those code paths.

Once it is configured and the makefile generated:

` make `


## Dependencies
- librackio
- libasound
- libsndfile
- libresample (optional / experimental)

## License

This is distributed under the terms of LGPL v3
libSndfile and libResample which are linked to at runtime, these are also LGPL (see in license/ for details)

# Credit

- libSndfile from [MegaNerd](http://www.mega-nerd.com/libsndfile/)
- libResample by Dominic Mazzoni, original based on resample-1.7 (http://www-ccrma.stanford.edu/~jos/resample/). It's not in the source tree because it is built separately as a shared library.
- Amaranth font for the logo
