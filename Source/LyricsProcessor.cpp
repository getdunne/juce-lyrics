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
    , currentLyricIndex(-1)
{
    juce::File lrcFile("C:\\Users\\owner\\Documents\\GitHub\\juce-lyrics\\LRC Files\\test.lrc");
    loadLrcFile(lrcFile);
}

LyricsProcessor::~LyricsProcessor()
{
}

void LyricsProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentLyricIndex = -1;
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
                int lyricIndex = getIndexOfLyricForTime(*timeSec);
                if (lyricIndex < 0)
                {
                    currentLyric.clear();
                    sendChangeMessage();
                }
                else if (lyricIndex != currentLyricIndex)
                {
                    currentLyricIndex = lyricIndex;
                    currentLyric = lyrics[currentLyricIndex]->lyricText;
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
            currentLyric = "No Playhead Position!";
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

void LyricsProcessor::loadLrcFile(juce::File lrcFile)
{
    if (!lrcFile.existsAsFile()) return;
    lyrics.clear();

    juce::StringArray lines;
    lrcFile.readLines(lines);
    for (auto& line : lines)
    {
        if (line.isNotEmpty() && line.matchesWildcard("[??:??.??]*", true))
        {
            double timeSec = 60 * line.substring(1, 3).getIntValue();
            int indexOfRightBracket = line.indexOf("]");
            timeSec += line.substring(4, indexOfRightBracket).getDoubleValue();
            lyrics.add(new TimedLyric({ timeSec, line.substring(indexOfRightBracket + 1).trim() }));
        }
    }
}

int LyricsProcessor::getIndexOfLyricForTime(double timeSec)
{
    int foundIndex = -1;
    int index = 0;
    for (auto lyric : lyrics)
    {
        if (timeSec >= lyric->timeSec)
            foundIndex = index;
        index++;
    }
    return foundIndex;
}
