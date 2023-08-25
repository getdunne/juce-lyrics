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

    regularFontSizeLabel.setJustificationType(juce::Justification::right);
    regularFontSizeLabel.setText("Text Size", juce::NotificationType::dontSendNotification);
    regularFontSizeLabel.attachToComponent(&regularFontSizeSlider, true);
    regularFontSizeSlider.setRange(10.0, 60.0, 1.0);
    regularFontSizeSlider.setValue(double(lyricsProcessor.regularFontHeight), juce::NotificationType::dontSendNotification);
    regularFontSizeSlider.onValueChange = [this]()
    {
        lyricsProcessor.regularFontHeight = int(regularFontSizeSlider.getValue());
        updateLyricsView();
    };
    addAndMakeVisible(regularFontSizeSlider);

    regularColourChangeButton.colour = &(lyricsProcessor.regularColour);
    regularColourChangeButton.onColourChange = [this]() { updateLyricsView(); };
    addAndMakeVisible(regularColourChangeButton);

    boldFontSizeLabel.setJustificationType(juce::Justification::right);
    boldFontSizeLabel.setText("Bold Size", juce::NotificationType::dontSendNotification);
    boldFontSizeLabel.attachToComponent(&boldFontSizeSlider, true);
    boldFontSizeSlider.setRange(10.0, 60.0, 1.0);
    boldFontSizeSlider.setValue(double(lyricsProcessor.boldFontHeight), juce::NotificationType::dontSendNotification);
    boldFontSizeSlider.onValueChange = [this]()
    {
        lyricsProcessor.boldFontHeight = int(boldFontSizeSlider.getValue());
        updateLyricsView();
    };
    addAndMakeVisible(boldFontSizeSlider);

    boldColourChangeButton.colour = &(lyricsProcessor.boldColour);
    boldColourChangeButton.onColourChange = [this]() { updateLyricsView(); };
    addAndMakeVisible(boldColourChangeButton);

    lyricsView.setMultiLine(true);
    lyricsView.setReadOnly(true);
    lyricsView.setJustification(juce::Justification::centred);
    lyricsView.setScrollbarsShown(false);
    addAndMakeVisible(lyricsView);

    settingsButton.setClickingTogglesState(true);
    settingsButton.onClick = [this]() { resized(); };
    addAndMakeVisible(settingsButton);

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
    auto area = getLocalBounds();
    if (settingsButton.getToggleState())
    {
        auto settingsArea = area.removeFromTop(4 * 10 + 3 * 24).reduced(10);
        settingsArea.removeFromLeft(100);   // space for labels
        loadLrcButton.setBounds(settingsArea.removeFromTop(24));
        settingsArea.removeFromTop(10);
        auto row = settingsArea.removeFromTop(24);
        regularColourChangeButton.setBounds(row.removeFromRight(70));
        row.removeFromRight(6);
        regularFontSizeSlider.setBounds(row);
        settingsArea.removeFromTop(10);
        row = settingsArea.removeFromTop(24);
        boldColourChangeButton.setBounds(row.removeFromRight(70));
        row.removeFromRight(6);
        boldFontSizeSlider.setBounds(row);
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
    lyricsView.scrollEditorToPositionCaret(0, lyricsView.getHeight() / 2 - lyricsProcessor.boldFontHeight);
}
