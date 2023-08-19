#include "LyricsProcessor.h"
#include "LyricsEditor.h"

LyricsProcessor::LyricsProcessor()
    : AudioProcessor (BusesProperties()
                      .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                      )
    , currentLyric("Yahoo")
{
}

LyricsProcessor::~LyricsProcessor()
{
}

void LyricsProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    lastTimeSec = 0;
}

void LyricsProcessor::releaseResources()
{
}

void LyricsProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    auto ph = getPlayHead();
    if (ph)
    {
        auto hostPosition = ph->getPosition();
        if (hostPosition)
        {
            auto timeSec = hostPosition->getTimeInSeconds();
            if (timeSec && (int(*timeSec) != lastTimeSec))
            {
                lastTimeSec = int(*timeSec);
                currentLyric = juce::String(lastTimeSec);
                sendChangeMessage();
            }
        }
        else
        {
            currentLyric = "No Playhead Time!";
            sendChangeMessage();
        }
    }
    else
    {
        currentLyric = "No Playhead!";
        sendChangeMessage();
    }
}

void LyricsProcessor::getStateInformation(juce::MemoryBlock& destData)
{
}

void LyricsProcessor::setStateInformation(const void* data, int sizeInBytes)
{
}

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
