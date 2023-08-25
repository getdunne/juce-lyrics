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
    , regularColour(juce::Colours::white)
    , boldColour(juce::Colours::aqua)
    , regularFontHeight(20)
    , boldFontHeight(24)
{
    clear();
    lines.add(new Line({ 0.0, 1.0, "Click icon to open Settings," }));
    lines.add(new Line({ 1.0, std::numeric_limits<double>::max(), "Then load a LRC file" }));
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
            auto timeSec = hostPosition->getTimeInSeconds();
            if (timeSec && hostBpm)
            {
                currentTimeSec = *timeSec * *hostBpm / getBpm();
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
    juce::Font regularFont(regularFontHeight);
    juce::Font boldFont(boldFontHeight, juce::Font::bold);
    getViewForTime(currentTimeSec, view, regularFont, regularColour, boldFont, boldColour);
}

void LyricsProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    std::unique_ptr<juce::XmlElement> xml(new juce::XmlElement("Lyrics"));
    xml->setAttribute("regularFontHeight", regularFontHeight);
    xml->setAttribute("boldFontHeight", boldFontHeight);
    xml->setAttribute("regularColour", regularColour.toString());
    xml->setAttribute("boldColour", boldColour.toString());
    xml->addChildElement(getXml());
    copyXmlToBinary(*xml, destData);
}

void LyricsProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    auto xml = getXmlFromBinary(data, sizeInBytes);

    regularFontHeight = xml->getIntAttribute("regularFontHeight", regularFontHeight);
    boldFontHeight = xml->getIntAttribute("boldFontHeight", boldFontHeight);

    juce::String colour = xml->getStringAttribute("regularColour");
    if (colour.isNotEmpty()) regularColour = juce::Colour::fromString(colour);
    colour = xml->getStringAttribute("boldColour");
    if (colour.isNotEmpty()) boldColour = juce::Colour::fromString(colour);

    putXml(xml->getChildByName("Lyrics"));
    sendChangeMessage();
}
