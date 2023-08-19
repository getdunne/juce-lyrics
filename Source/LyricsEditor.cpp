#include "LyricsProcessor.h"
#include "LyricsEditor.h"

LyricsEditor::LyricsEditor (LyricsProcessor& p)
    : AudioProcessorEditor (&p), lyricsProcessor (p)
{
    setSize (400, 300);

    lyricsProcessor.addChangeListener(this);
}

LyricsEditor::~LyricsEditor()
{
    lyricsProcessor.removeChangeListener(this);
}

void LyricsEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText(lyricsProcessor.currentLyric, getLocalBounds(), juce::Justification::centred, 1);
}

void LyricsEditor::changeListenerCallback(juce::ChangeBroadcaster*)
{
    repaint();
}
