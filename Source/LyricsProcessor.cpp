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
{
    juce::File lrcFile("C:\\Users\\owner\\Documents\\GitHub\\juce-lyrics\\LRC Files\\test.lrc");
    lyrics.loadLrcFile(lrcFile);
}

LyricsProcessor::~LyricsProcessor()
{
}

void LyricsProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentCue = nullptr;
}

void LyricsProcessor::releaseResources()
{
}

void LyricsProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (lyrics.isEmpty()) return;

    auto ph = getPlayHead();
    if (ph)
    {
        auto hostPosition = ph->getPosition();
        if (hostPosition)
        {
            auto timeSec = hostPosition->getTimeInSeconds();
            if (timeSec)
            {
                auto cue = lyrics.getCueForTime(*timeSec);
                if (cue)
                {
                    currentCue = cue;
                    currentLyric = cue->text;
                }
                else currentLyric.clear();
            }
            else currentLyric = "No Playhead Time!";
        }
        else currentLyric = "No Playhead Position!";
    }
    else currentLyric = "No Playhead!";

    sendChangeMessage();
}

void LyricsProcessor::getStateInformation(juce::MemoryBlock& destData)
{
}

void LyricsProcessor::setStateInformation(const void* data, int sizeInBytes)
{
}
