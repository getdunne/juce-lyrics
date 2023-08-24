#include "LyricsProcessor.h"
#include "LyricsEditor.h"

// Creates new instance of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LyricsProcessor();
}

// Create a new editor (GUI) for the plugin
juce::AudioProcessorEditor* LyricsProcessor::createEditor()
{
    return new LyricsEditor(*this);
}


LyricsProcessor::LyricsProcessor()
    : AudioProcessor (BusesProperties()
                      .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                      )
    , currentCue(nullptr)
    , currentTimeSec(0.0)
{
    juce::File lrcFile("C:\\Users\\owner\\Documents\\GitHub\\juce-lyrics\\LRC Files\\test.lrc");
    loadLrcFile(lrcFile);
}

LyricsProcessor::~LyricsProcessor()
{
}

void LyricsProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentCue = nullptr;
    currentTimeSec = 0.0;
}

void LyricsProcessor::releaseResources()
{
}

void LyricsProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (isEmpty()) return;

    bool currentCueChanged = false;

    auto ph = getPlayHead();
    if (ph)
    {
        auto hostPosition = ph->getPosition();
        if (hostPosition)
        {
            auto timeSec = hostPosition->getTimeInSeconds();
            if (timeSec)
            {
                currentTimeSec = *timeSec;
                auto cue = getCueForTime(*timeSec);
                if (cue != currentCue) currentCueChanged = true;

                if (cue)
                {
                    currentCue = cue;
                    currentLyric = cue->text;
                }
                else currentLyric.clear();
            }
        }
    }

    if (currentCueChanged)
        sendChangeMessage();
}

void LyricsProcessor::getLyricsView(juce::TextEditor& view,
                                    int regularFontHeight, juce::Colour regularColour,
                                    int boldFontHeight, juce::Colour boldColour)
{
    juce::Font regularFont(regularFontHeight);
    juce::Font boldFont(boldFontHeight, juce::Font::bold);
    getLyricsViewForTime(currentTimeSec, view, regularFont, regularColour, boldFont, boldColour);
}

void LyricsProcessor::getStateInformation(juce::MemoryBlock& destData)
{
}

void LyricsProcessor::setStateInformation(const void* data, int sizeInBytes)
{
}
