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
    , currentLyricLine(nullptr)
    , currentTimeSec(0.0)
    , backgroundColour(juce::Colours::black)
    , lyricsColour(juce::Colours::white)
    , highlightColour(juce::Colours::aqua)
    , otherColour(juce::Colours::lightgreen)
    , lyricsFontHeight(20)
    , highlightFontHeight(24)
    , otherFontHeight(14)
{
    clear();
}

LyricsProcessor::~LyricsProcessor()
{
}

void LyricsProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentLyricLine = nullptr;
    currentTimeSec = 0.0;
}

void LyricsProcessor::releaseResources()
{
}

void LyricsProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (isEmpty()) return;

    bool currentLyricLineChanged = false;

    auto ph = getPlayHead();
    if (ph)
    {
        auto hostPosition = ph->getPosition();
        if (hostPosition)
        {
            auto hostBpm = hostPosition->getBpm();
            double refBpm = getBpm();
            auto timeSec = hostPosition->getTimeInSeconds();
            if (timeSec && hostBpm)
            {
                currentTimeSec = *timeSec;
                if (refBpm != 0.0) currentTimeSec *= (*hostBpm / refBpm);
                auto ll = getLineForTime(currentTimeSec);
                if (ll != currentLyricLine) currentLyricLineChanged = true;
                currentLyricLine = ll;
            }
        }
    }

    if (currentLyricLineChanged)
        sendChangeMessage();
}

void LyricsProcessor::getLyricsView(juce::TextEditor& view)
{
    juce::Font lyricsFont(lyricsFontHeight);
    juce::Font highlightFont(highlightFontHeight, juce::Font::bold);
    juce::Font otherFont(otherFontHeight);
    getView(currentLyricLine, view,
        lyricsFont, lyricsColour, highlightFont, highlightColour, otherFont, otherColour);
}

void LyricsProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    std::unique_ptr<juce::XmlElement> xml(new juce::XmlElement("Lyrics"));
    xml->setAttribute("lyricsFontHeight", lyricsFontHeight);
    xml->setAttribute("highlightFontHeight", highlightFontHeight);
    xml->setAttribute("lyricsColour", lyricsColour.toString());
    xml->setAttribute("highlightColour", highlightColour.toString());
    xml->setAttribute("otherColour", otherColour.toString());
    xml->setAttribute("highlightColour", highlightColour.toString());
    xml->addChildElement(getXml());
    copyXmlToBinary(*xml, destData);
}

void LyricsProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    auto xml = getXmlFromBinary(data, sizeInBytes);

    lyricsFontHeight = xml->getIntAttribute("lyricsFontHeight", lyricsFontHeight);
    highlightFontHeight = xml->getIntAttribute("highlightFontHeight", highlightFontHeight);

    juce::String colour = xml->getStringAttribute("lyricsColour");
    if (colour.isNotEmpty()) lyricsColour = juce::Colour::fromString(colour);
    colour = xml->getStringAttribute("highlightColour");
    if (colour.isNotEmpty()) highlightColour = juce::Colour::fromString(colour);
    colour = xml->getStringAttribute("otherColour");
    if (colour.isNotEmpty()) otherColour = juce::Colour::fromString(colour);

    putXml(xml->getChildByName("Lyrics"));
    sendChangeMessage();
}
