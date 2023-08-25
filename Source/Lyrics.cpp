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
        if (fileLine.isNotEmpty())
        {
            if (fileLine.matchesWildcard("[??:??.??]*", true))
            {
                double timeSec = 60 * fileLine.substring(1, 3).getIntValue();
                int indexOfRightBracket = fileLine.indexOf("]");
                timeSec += fileLine.substring(4, indexOfRightBracket).getDoubleValue();
                lines.add(new Line({ timeSec, std::numeric_limits<double>::max(), fileLine.substring(indexOfRightBracket + 1).trim() }));
            }
            else if (fileLine.matchesWildcard("[bpm:*", true))
            {
                bpm = fileLine.substring(5).getDoubleValue();
            }
        }
    }

    // fix cue end-times (except the last one)
    for (int i = 0; i < (lines.size() - 1); i++)
        lines[i]->endSec = lines[i + 1]->startSec;
}

Lyrics::Line* Lyrics::getLineForTime(double timeSec)
{
    for (auto ll : lines)
        if ((timeSec >= ll->startSec) && (timeSec < ll->endSec)) return ll;
    return nullptr;
}

void Lyrics::getViewForTime(double timeSec, juce::TextEditor& view,
                            juce::Font& regularFont, juce::Colour regularColour,
                            juce::Font& boldFont, juce::Colour boldColour)
{
    view.clear();
    view.setCaretPosition(0);

    int startOfBoldText = 0;

    for (int i = 0; i < 50; i++) view.insertTextAtCaret("\n");
    for (auto ll : lines)
    {
        if ((timeSec >= ll->startSec) && (timeSec < ll->endSec))
        {
            view.setFont(boldFont);
            view.setColour(juce::TextEditor::textColourId, boldColour);
            startOfBoldText = view.getCaretPosition();
        }
        else
        {
            view.setFont(regularFont);
            view.setColour(juce::TextEditor::textColourId, regularColour);
        }
        view.insertTextAtCaret(ll->text + "\n");
    }
    for (int i = 0; i < 50; i++) view.insertTextAtCaret("\n");

    view.setCaretPosition(startOfBoldText);
}
