#pragma once
#include <JuceHeader.h>

class TimedText
{
public:
	struct Cue
	{ 
		double startSec, endSec;
		juce::String text;
	};

	bool isEmpty() { return cues.isEmpty(); }

	void loadLrcFile(juce::File);
	Cue* getCueForTime(double timeSec);

protected:
	void getLyricsViewForTime(double timeSec, juce::TextEditor& view,
		juce::Font& regularFont, juce::Colour regularColour,
		juce::Font& boldFont, juce::Colour boldColour);

private:
	juce::OwnedArray<Cue> cues;
};
