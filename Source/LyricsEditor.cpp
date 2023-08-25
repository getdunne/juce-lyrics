#include "LyricsProcessor.h"
#include "LyricsEditor.h"

LyricsEditor::LyricsEditor(LyricsProcessor& p)
    : AudioProcessorEditor(&p), lyricsProcessor(p)
    , settingsButton(BinaryData::settings_svg)
{
    loadLrcButton.setButtonText("Load LRC file...");
    loadLrcButton.onClick = [this]()
    {
        juce::FileChooser chooser("LRC File",
            juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDocumentsDirectory), "*.lrc");
        if (chooser.browseForFileToOpen())
        {
            lyricsProcessor.loadLrcFile(chooser.getResult());
            updateLyricsView();
        }
    };
    addAndMakeVisible(loadLrcButton);

    lyricsFontSizeLabel.setJustificationType(juce::Justification::right);
    lyricsFontSizeLabel.setText("Lyrics", juce::NotificationType::dontSendNotification);
    lyricsFontSizeLabel.attachToComponent(&lyricsFontSizeSlider, true);
    lyricsFontSizeSlider.setRange(10.0, 60.0, 1.0);
    lyricsFontSizeSlider.setValue(double(lyricsProcessor.lyricsFontHeight), juce::NotificationType::dontSendNotification);
    lyricsFontSizeSlider.onValueChange = [this]()
    {
        lyricsProcessor.lyricsFontHeight = int(lyricsFontSizeSlider.getValue());
        updateLyricsView();
    };
    addAndMakeVisible(lyricsFontSizeSlider);

    lyricsColourChangeButton.colour = &(lyricsProcessor.lyricsColour);
    lyricsColourChangeButton.onColourChange = [this]() { updateLyricsView(); };
    addAndMakeVisible(lyricsColourChangeButton);

    highlightFontSizeLabel.setJustificationType(juce::Justification::right);
    highlightFontSizeLabel.setText("Highlight", juce::NotificationType::dontSendNotification);
    highlightFontSizeLabel.attachToComponent(&highlightFontSizeSlider, true);
    highlightFontSizeSlider.setRange(10.0, 60.0, 1.0);
    highlightFontSizeSlider.setValue(double(lyricsProcessor.highlightFontHeight), juce::NotificationType::dontSendNotification);
    highlightFontSizeSlider.onValueChange = [this]()
    {
        lyricsProcessor.highlightFontHeight = int(highlightFontSizeSlider.getValue());
        updateLyricsView();
    };
    addAndMakeVisible(highlightFontSizeSlider);

    highlightColourChangeButton.colour = &(lyricsProcessor.highlightColour);
    highlightColourChangeButton.onColourChange = [this]() { updateLyricsView(); };
    addAndMakeVisible(highlightColourChangeButton);

    otherFontSizeLabel.setJustificationType(juce::Justification::right);
    otherFontSizeLabel.setText("Other", juce::NotificationType::dontSendNotification);
    otherFontSizeLabel.attachToComponent(&otherFontSizeSlider, true);
    otherFontSizeSlider.setRange(10.0, 60.0, 1.0);
    otherFontSizeSlider.setValue(double(lyricsProcessor.otherFontHeight), juce::NotificationType::dontSendNotification);
    otherFontSizeSlider.onValueChange = [this]()
    {
        lyricsProcessor.otherFontHeight = int(otherFontSizeSlider.getValue());
        updateLyricsView();
    };
    addAndMakeVisible(otherFontSizeSlider);

    otherColourChangeButton.colour = &(lyricsProcessor.otherColour);
    otherColourChangeButton.onColourChange = [this]() { updateLyricsView(); };
    addAndMakeVisible(otherColourChangeButton);

    lyricsView.setMultiLine(true);
    lyricsView.setReadOnly(true);
    lyricsView.setJustification(juce::Justification::centred);
    lyricsView.setScrollbarsShown(false);
    addAndMakeVisible(lyricsView);

    settingsButton.setClickingTogglesState(true);
    settingsButton.onClick = [this]() { resized(); };
    addAndMakeVisible(settingsButton);

    setResizable(true, true);
    setSize (512, 400);

    lyricsProcessor.addChangeListener(this);
}

LyricsEditor::~LyricsEditor()
{
    lyricsProcessor.removeChangeListener(this);
}

void LyricsEditor::resized()
{
    auto area = getLocalBounds();
    if (settingsButton.getToggleState())
    {
        auto settingsArea = area.removeFromTop(5 * 10 + 4 * 24).reduced(10);
        settingsArea.removeFromLeft(100);   // space for labels

        loadLrcButton.setBounds(settingsArea.removeFromTop(24));
        settingsArea.removeFromTop(10);

        auto row = settingsArea.removeFromTop(24);
        lyricsColourChangeButton.setBounds(row.removeFromRight(70));
        row.removeFromRight(6);
        lyricsFontSizeSlider.setBounds(row);
        settingsArea.removeFromTop(10);

        row = settingsArea.removeFromTop(24);
        highlightColourChangeButton.setBounds(row.removeFromRight(70));
        row.removeFromRight(6);
        highlightFontSizeSlider.setBounds(row);
        settingsArea.removeFromTop(10);

        row = settingsArea.removeFromTop(24);
        otherColourChangeButton.setBounds(row.removeFromRight(70));
        row.removeFromRight(6);
        otherFontSizeSlider.setBounds(row);
    }
    lyricsView.setBounds(area);

    area = getLocalBounds();
    settingsButton.setBounds(area.removeFromTop(30).removeFromLeft(30).reduced(3));

    updateLyricsView();
}

void LyricsEditor::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void LyricsEditor::updateLyricsView()
{
    lyricsProcessor.getLyricsView(lyricsView);
    lyricsView.scrollEditorToPositionCaret(0, lyricsView.getHeight() / 2 - lyricsProcessor.highlightFontHeight);
}
