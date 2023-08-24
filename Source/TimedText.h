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
	juce::OwnedArray<Cue> cues;

private:
};
