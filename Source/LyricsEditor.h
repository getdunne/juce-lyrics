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
    void resized() override;
    void paint (juce::Graphics&) override;

    // juce::ChangeListener
    void changeListenerCallback(juce::ChangeBroadcaster*) { updateLyricsView(); }

protected:
    void updateLyricsView();

private:
    LyricsProcessor& lyricsProcessor;

    juce::Colour backgroundColour, regularColour, boldColour;
    int regularFontHeight, boldFontHeight;
    juce::TextEditor lyricsView;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LyricsEditor)
};
