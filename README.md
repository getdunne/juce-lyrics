# juce-lyrics
This is a [JUCE](https://juce.com) based plug-in to display karaoke-style lyrics, synchronized with song-position in a DAW.

## Graphical user interface

![interface](C:\Users\owner\Documents\GitHub\juce-lyrics\interface.png)

The screenshot above shows the full GUI, including the "settings controls" at the top, and the "lyrics area" below. When you first open the plug-in, the lyrics area will be empty; you must use the button at the very top to load lyrics from an [LRC file](https://en.wikipedia.org/wiki/LRC_(file_format)) (see below). As soon as some lyrics have been loaded (either manually from an LRC file, or because they were saved in the plug-in state), the GUI will open with the lyrics area expanded to the full window size, and the "gear" (settings) icon at the top left. Click the gear icon to show/hide the settings controls.

## Basic operation

The Lyrics plug-in (VST3 or AUv2) follows the current *song position* provided by the plug-in host (e.g. DAW). The lyrics data consist of a sequence of *lines*, each with an associated *start-time* and *end-time* in seconds, relative to the song start. At any given time (song position), the single matching lyric line is displayed in a *highlight font/color*, within the context of other lyric lines, as well as optional non-timed "other" lines (e.g., "Verse 1" in the example shown above). These "other" or non-lyric lines have no start- or end-time, and hence are never highlighted. The lyrics display automatically scrolls to keep the current highlighted line approximately centered vertically in the display area.

The lyrics data also include a *reference BPM rate*, which is the song tempo for which the lyric start- and end-times are exact. If the plug-in host's tempo is set faster or slower, these times are effectively adjusted to compensate. Finally, the lyrics data also includes an *offset time*, which causes the program to highlight each lyric line somewhat earlier (positive offset) or later (negative offset) then 

The plug-in GUI window may be resized by dragging the "gripper" icon at the bottom-right.

The **Load LRC file...** button opens a standard file-open dialog, allowing you to select an *.lrc* file anywhere on your system. See below for details about LRC files. Loading a new LRC file replaces any previously-loaded lyrics data.

The three rows of controls below the Load button allow you to define the height and color for each of the three fonts used to display lyrics, highlighted lyrics, and non-lyric (other) lines, respectively. Clicking any of the **Color...** buttons opens a standard JUCE *ColourSelector* dialog in a pop-up "callout box". Click anywhere outside the box to dismiss it.

The **Offset (sec)** slider allows you to adjust the *offset time* added to computed song-position times, in order to highlight individual lyric-lines slightly earlier or later than the associated start-time.

## LRC files

This plug-in makes use of a slight variation of the standard [Simple LRC format](https://en.wikipedia.org/wiki/LRC_(file_format)) format, wherein an extra "bpm" line, e.g. 

`[bpm: 94]`

may be included, to specify the *reference BPM rate*. If no such line is included, lyric start- and end-times will be treated as absolute; the plug-in will not attempt to adjust them based on the host BPM rate.

Note this plug-in *does not* recognize the LRC "A2 extension" (individual word time-tags).

Several web-based and native utilities are available to help you create and edit LRC files. [LRC Generator](http://www.lrcgenerator.com/) is a nice, free web-based one.

