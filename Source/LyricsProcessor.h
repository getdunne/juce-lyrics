#pragma once
#include <JuceHeader.h>
#include "Lyrics.h"

class LyricsProcessor   : public juce::AudioProcessor
                        , public juce::ChangeBroadcaster
                        , public Lyrics
{
public:
    LyricsProcessor();
    ~LyricsProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    bool hasEditor() const override { return true; }
    juce::AudioProcessorEditor* createEditor() override;

    const juce::String getName() const override { return JucePlugin_Name; }

    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int index) override {}
    const juce::String getProgramName(int index) override { return {}; }
    void changeProgramName(int index, const juce::String& newName) override {}

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    Lyrics::Line* currentLyricLine;

    juce::String getCurrentLyric() { return currentLyricLine ? currentLyricLine->text : juce::String(); }

    juce::Colour backgroundColour, lyricsColour, highlightColour, otherColour;
    int lyricsFontHeight, highlightFontHeight, otherFontHeight;

    void getLyricsView(juce::TextEditor& view);

private:
    double currentTimeSec;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LyricsProcessor)
};
