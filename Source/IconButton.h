#pragma once
#include <JuceHeader.h>

/*  IconButton is an icon that is clickable like a button, and remembers any modifier keys
    held down when it was clicked, so the onClick() handler can access them.
*/

class IconButton   : public juce::DrawableButton
{
public:
    IconButton(const char* iconSvg);
    IconButton(const char* offSvg, const char* onSvg);
    virtual ~IconButton() = default;

    void setup(const char* iconSvg);
    void setup(const char* offSvg, const char* onSvg);

    void clicked(const juce::ModifierKeys& modifiers) override
    {
        mods = modifiers;
        Button::clicked();
    }

    juce::ModifierKeys mods;

protected:
    const juce::Colour iconColour = juce::Colour(0.0f, 0.0f, 0.85f, 1.0f);
    const juce::Colour highlightedIconColour = juce::Colour(89, 252, 224);
    const juce::Colour logoTextColour = juce::Colour(41, 196, 159);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IconButton)
};
