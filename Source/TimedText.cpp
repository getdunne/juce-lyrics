#include "TimedText.h"
#include <limits>

void TimedText::loadLrcFile(juce::File lrcFile)
{
    if (!lrcFile.existsAsFile()) return;
    cues.clear();

    // read the file
    juce::StringArray lines;
    lrcFile.readLines(lines);
    for (auto& line : lines)
    {
        if (line.isNotEmpty() && line.matchesWildcard("[??:??.??]*", true))
        {
            double timeSec = 60 * line.substring(1, 3).getIntValue();
            int indexOfRightBracket = line.indexOf("]");
            timeSec += line.substring(4, indexOfRightBracket).getDoubleValue();
            cues.add(new Cue({ timeSec, std::numeric_limits<double>::max(), line.substring(indexOfRightBracket + 1).trim() }));
        }
    }

    // fix cue end-times (except the last one)
    for (int i = 0; i < (cues.size() - 1); i++)
        cues[i]->endSec = cues[i + 1]->startSec;
}

TimedText::Cue* TimedText::getCueForTime(double timeSec)
{
    for (auto cue : cues)
        if ((timeSec >= cue->startSec) && (timeSec < cue->endSec)) return cue;
    return nullptr;
}
