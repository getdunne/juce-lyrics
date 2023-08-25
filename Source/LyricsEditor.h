#pragma once
#include <JuceHeader.h>
#include "LyricsProcessor.h"
#include "IconButton.h"

class ColourChangeButton    : public juce::TextButton
                            , public juce::ChangeListener
{
public:
    std::function<void()> onColourChange;
    juce::Colour* colour = nullptr;

    ColourChangeButton() : TextButton("Color...")
    {
        setSize(10, 24);
        changeWidthToFitText();
    }

    void clicked() override
    {
        auto colourSelector = std::make_unique<juce::ColourSelector>(juce::ColourSelector::showAlphaChannel
            | juce::ColourSelector::showColourAtTop
            | juce::ColourSelector::editableColour
            | juce::ColourSelector::showSliders
            | juce::ColourSelector::showColourspace);

        colourSelector->setName("background");
        if (colour) colourSelector->setCurrentColour(*colour);
        colourSelector->addChangeListener(this);
        colourSelector->setColour(juce::ColourSelector::backgroundColourId, juce::Colours::transparentBlack);
        colourSelector->setSize(300, 400);

        juce::CallOutBox::launchAsynchronously(std::move(colourSelector), getScreenBounds(), nullptr);
    }

    using TextButton::clicked;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override
    {
        if (auto* cs = dynamic_cast<juce::ColourSelector*> (source))
        {
            if (colour) *colour = cs->getCurrentColour();
            if (onColourChange) onColourChange();
        }
    }
};

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
    void changeListenerCallback(juce::ChangeBroadcaster*) override { updateLyricsView(); }

protected:
    void updateLyricsView();

private:
    LyricsProcessor& lyricsProcessor;

    IconButton settingsButton;

    juce::TextButton loadLrcButton;

    juce::Label lyricsFontSizeLabel;
    juce::Slider lyricsFontSizeSlider;
    ColourChangeButton lyricsColourChangeButton;

    juce::Label highlightFontSizeLabel;
    juce::Slider highlightFontSizeSlider;
    ColourChangeButton highlightColourChangeButton;

    juce::Label otherFontSizeLabel;
    juce::Slider otherFontSizeSlider;
    ColourChangeButton otherColourChangeButton;

    juce::Label offsetLabel;
    juce::Slider offsetSlider;

    juce::TextEditor lyricsView;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LyricsEditor)
};
