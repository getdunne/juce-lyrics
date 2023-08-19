#pragma once
#include <JuceHeader.h>
#include "LyricsProcessor.h"

class LyricsEditor  : public juce::AudioProcessorEditor
                    , public juce::ChangeListener
{
public:
    LyricsEditor (LyricsProcessor&);
    ~LyricsEditor() override;

    // juce::Component
    void paint (juce::Graphics&) override;

    // juce::ChangeListener
    void changeListenerCallback(juce::ChangeBroadcaster*);

private:
    LyricsProcessor& lyricsProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LyricsEditor)
};
