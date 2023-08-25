#include "Lyrics.h"
#include <limits>

juce::XmlElement* Lyrics::Line::getXml()
{
    auto xml = new juce::XmlElement("LyricLine");
    xml->setAttribute("startSec", startSec);
    xml->setAttribute("endSec", endSec);
    xml->setAttribute("text", text);
    return xml;
}

void Lyrics::Line::putXml(juce::XmlElement* xml)
{
    jassert(xml->hasTagName("LyricLine"));
    startSec = xml->getDoubleAttribute("startSec", 0.0);
    endSec = xml->getDoubleAttribute("endSec", std::numeric_limits<double>::max());
    text = xml->getStringAttribute("text");
}


juce::XmlElement* Lyrics::getXml()
{
    auto xml = new juce::XmlElement("Lyrics");
    xml->setAttribute("bpm", bpm);
    xml->setAttribute("offsetSec", offsetSec);
    for (auto cue : lines)
        xml->addChildElement(cue->getXml());
    return xml;
}

void Lyrics::putXml(juce::XmlElement* xml)
{
    clear();
    if (xml)
    {
        jassert(xml->hasTagName("Lyrics"));
        bpm = xml->getDoubleAttribute("bpm");
        offsetSec = xml->getDoubleAttribute("offsetSec");
        for (auto cueXml : xml->getChildWithTagNameIterator("LyricLine"))
        {
            auto ll = new Line;
            ll->putXml(cueXml);
            lines.add(ll);
        }
    }
}

void Lyrics::loadLrcFile(juce::File lrcFile)
{
    if (!lrcFile.existsAsFile()) return;
    clear();

    // read the file
    juce::StringArray fileLines;
    lrcFile.readLines(fileLines);
    for (auto& fileLine : fileLines)
    {
        if (! fileLine.startsWithChar('['))
        {
            // non-lyric ("other") line
            lines.add(new Line({ 0.0, 0.0, fileLine.trim() }));
        }
        else if (fileLine.matchesWildcard("[??:??.??]*", true))
        {
            // lyric line
            double timeSec = 60 * fileLine.substring(1, 3).getIntValue();
            int indexOfRightBracket = fileLine.indexOf("]");
            timeSec += fileLine.substring(4, indexOfRightBracket).getDoubleValue();
            lines.add(new Line({ timeSec, std::numeric_limits<double>::max(), fileLine.substring(indexOfRightBracket + 1).trim() }));
        }
        else if (fileLine.matchesWildcard("[bpm:*", true))
        {
            // reference BPM tag
            bpm = fileLine.substring(5).getDoubleValue();
        }
        else if (fileLine.matchesWildcard("[offset:*", true))
        {
            // time offset, seconds
            offsetSec = fileLine.substring(8).getDoubleValue();
        }
    }

    // fix lyric-line end-times (except the last one)
    for (int i = 0; i < (lines.size() - 1); i++)
    {
        if (lines[i]->endSec == std::numeric_limits<double>::max())
        {
            // search past any non-lyric lines for next lyric line
            for (int j = i + 1; j < lines.size(); j++)
            {
                if (lines[j]->endSec == std::numeric_limits<double>::max())
                {
                    lines[i]->endSec = lines[j]->startSec;
                    break;
                }
            }
        }
    }

#if 0
    std::unique_ptr<juce::XmlElement> xml(getXml());
    DBG(xml->toString());
#endif
}

Lyrics::Line* Lyrics::getLineForTime(double timeSec)
{
    timeSec += offsetSec;
    for (auto ll : lines)
        if ((timeSec >= ll->startSec) && (timeSec < ll->endSec))
        {
#if 0
            if (timeSec != 0.0) DBG(juce::String(timeSec) + ": " + ll->text);
#endif
            return ll;
        }

    // if no match, return the first lyric line
    for (auto ll : lines)
        if (ll->endSec > ll->startSec) return ll;

    // if no lyric lines at all, return null
    return nullptr;
}

void Lyrics::getView(Line* highlightedLine, juce::TextEditor& view,
                     juce::Font& lyricsFont, juce::Colour lyricsColour,
                     juce::Font& highlightFont, juce::Colour highlightColour,
                     juce::Font& otherFont, juce::Colour otherColour)
{
    view.clear();
    view.setCaretPosition(0);

    int startOfBoldText = 0;

    for (int i = 0; i < 50; i++) view.insertTextAtCaret("\n");
    for (auto ll : lines)
    {
        if (ll == highlightedLine)
        {
            view.setFont(highlightFont);
            view.setColour(juce::TextEditor::textColourId, highlightColour);
            startOfBoldText = view.getCaretPosition();
        }
        else if (ll->endSec > ll->startSec)
        {
            view.setFont(lyricsFont);
            view.setColour(juce::TextEditor::textColourId, lyricsColour);
        }
        else
        {
            view.setFont(otherFont);
            view.setColour(juce::TextEditor::textColourId, otherColour);
        }
        view.insertTextAtCaret(ll->text + "\n");
    }
    for (int i = 0; i < 50; i++) view.insertTextAtCaret("\n");

    view.setCaretPosition(startOfBoldText);
}
