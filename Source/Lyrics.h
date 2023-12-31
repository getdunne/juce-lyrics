#pragma once
#include <JuceHeader.h>

class Lyrics
{
public:
	struct Line
	{
		double startSec, endSec;
		juce::String text;

		juce::XmlElement* getXml();
		void putXml(juce::XmlElement* xml);
	};

	void clear() { lines.clear(); bpm = offsetSec = 0.0; }
	bool isEmpty() { return lines.isEmpty(); }
	double getBpm() { return bpm; }	// returns 0.0 if no reference BPM available
	double getOffsetSec() { return offsetSec; }
	void setOffsetSec(double ofs) { offsetSec = ofs; }

	void loadLrcFile(juce::File);
	Line* getLineForTime(double timeSec);

	juce::XmlElement* getXml();
	void putXml(juce::XmlElement* xml);

protected:
	void getView(Line* highlightedLine, juce::TextEditor& view,
		juce::Font& lyricsFont, juce::Colour lyricsColour,
		juce::Font& highlightFont, juce::Colour highlightColour,
		juce::Font& otherFont, juce::Colour otherColour);

	double bpm = 0.0;
	double offsetSec = 0.0;
	juce::OwnedArray<Line> lines;
};
