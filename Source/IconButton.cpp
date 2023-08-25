#include "IconButton.h"

static juce::String codeForSvg(const juce::Colour colour)
{
    return juce::String("#") + colour.toDisplayString(false);
}

IconButton::IconButton(const char* iconSvg)
    : juce::DrawableButton({}, ButtonStyle::ImageFitted)
{
    setWantsKeyboardFocus(false);
    setMouseClickGrabsKeyboardFocus(false);
    setup(iconSvg);
}

IconButton::IconButton(const char* offSvg, const char* onSvg)
    : juce::DrawableButton({}, ButtonStyle::ImageFitted)
{
    setWantsKeyboardFocus(false);
    setMouseClickGrabsKeyboardFocus(false);
    setup(offSvg, onSvg);
}

void IconButton::setup(const char* iconSvg)
{
    std::unique_ptr<juce::XmlElement> svgXml(juce::XmlDocument::parse(iconSvg));

    svgXml->setAttribute("stroke", codeForSvg(iconColour.darker()));
    std::unique_ptr<juce::Drawable> normal(juce::Drawable::createFromSVG(*svgXml));

    svgXml->setAttribute("stroke", codeForSvg(iconColour));
    std::unique_ptr<juce::Drawable> over(juce::Drawable::createFromSVG(*svgXml));

    svgXml->setAttribute("stroke", codeForSvg(logoTextColour));
    std::unique_ptr<juce::Drawable> down(juce::Drawable::createFromSVG(*svgXml));

    svgXml->setAttribute("stroke", codeForSvg(highlightedIconColour.darker(0.3f)));
    std::unique_ptr<juce::Drawable> normalOn(juce::Drawable::createFromSVG(*svgXml));

    svgXml->setAttribute("stroke", codeForSvg(highlightedIconColour));
    std::unique_ptr<juce::Drawable> overOn(juce::Drawable::createFromSVG(*svgXml));

    svgXml->setAttribute("stroke", codeForSvg(logoTextColour));
    std::unique_ptr<juce::Drawable> overDown(juce::Drawable::createFromSVG(*svgXml));

    setImages(normal.get(), over.get(), down.get(), nullptr, normalOn.get(), overOn.get(), overDown.get());
}

void IconButton::setup(const char* offSvg, const char* onSvg)
{
    std::unique_ptr<juce::XmlElement> svgXml(juce::XmlDocument::parse(offSvg));

    svgXml->setAttribute("stroke", codeForSvg(iconColour.darker()));
    std::unique_ptr<juce::Drawable> normal(juce::Drawable::createFromSVG(*svgXml));

    svgXml->setAttribute("stroke", codeForSvg(iconColour));
    std::unique_ptr<juce::Drawable> over(juce::Drawable::createFromSVG(*svgXml));

    svgXml->setAttribute("stroke", codeForSvg(logoTextColour));
    std::unique_ptr<juce::Drawable> down(juce::Drawable::createFromSVG(*svgXml));

    svgXml = juce::XmlDocument::parse(onSvg);

    svgXml->setAttribute("stroke", codeForSvg(highlightedIconColour.darker(0.3f)));
    std::unique_ptr<juce::Drawable> normalOn(juce::Drawable::createFromSVG(*svgXml));

    svgXml->setAttribute("stroke", codeForSvg(highlightedIconColour));
    std::unique_ptr<juce::Drawable> overOn(juce::Drawable::createFromSVG(*svgXml));

    svgXml->setAttribute("stroke", codeForSvg(logoTextColour));
    std::unique_ptr<juce::Drawable> overDown(juce::Drawable::createFromSVG(*svgXml));

    setImages(normal.get(), over.get(), down.get(), nullptr, normalOn.get(), overOn.get(), overDown.get());
}
