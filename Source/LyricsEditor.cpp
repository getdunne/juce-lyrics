#include "LyricsProcessor.h"
#include "LyricsEditor.h"

LyricsEditor::LyricsEditor(LyricsProcessor& p)
    : AudioProcessorEditor(&p), lyricsProcessor(p)
    , backgroundColour(juce::Colours::black)
    , regularColour(juce::Colours::white)
    , boldColour(juce::Colours::aqua)
    , regularFontHeight(20)
    , boldFontHeight(24)
{
    lyricsView.setMultiLine(true);
    lyricsView.setReadOnly(true);
    lyricsView.setJustification(juce::Justification::centred);
    lyricsView.setScrollbarsShown(false);
    addAndMakeVisible(lyricsView);

    setResizable(true, true);
    setSize (512, 300);

    lyricsProcessor.addChangeListener(this);
}

LyricsEditor::~LyricsEditor()
{
    lyricsProcessor.removeChangeListener(this);
}

void LyricsEditor::resized()
{
    lyricsView.setBounds(getLocalBounds());
    updateLyricsView();
}

void LyricsEditor::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void LyricsEditor::updateLyricsView()
{
    lyricsProcessor.getLyricsView(lyricsView, regularFontHeight, regularColour, boldFontHeight, boldColour);
    lyricsView.scrollEditorToPositionCaret(0, lyricsView.getHeight() / 2 - boldFontHeight);
}
