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

	void clear() { lines.clear(); bpm = 0.0; }
	bool isEmpty() { return lines.isEmpty(); }
	double getBpm() { return bpm; }	// returns 0.0 if no reference BPM available

	void loadLrcFile(juce::File);
	Line* getLineForTime(double timeSec);

	juce::XmlElement* getXml();
	void putXml(juce::XmlElement* xml);

protected:
	void getViewForTime(double timeSec, juce::TextEditor& view,
		juce::Font& regularFont, juce::Colour regularColour,
		juce::Font& boldFont, juce::Colour boldColour);

	double bpm = 0.0;
	juce::OwnedArray<Line> lines;
};
