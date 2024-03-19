#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleVSTAudioProcessorEditor::SimpleVSTAudioProcessorEditor (SimpleVSTAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    addAndMakeVisible(HighCut);

    HighCut.setSliderStyle(juce::Slider::SliderStyle::Rotary); // Slider style
    HighCut.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 90, 20); //  Text box position
    HighCut.setTextValueSuffix(" hz");
    HighCut.setTextBoxIsEditable(true);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    addAndMakeVisible(LowCut);

    LowCut.setSliderStyle(juce::Slider::SliderStyle::Rotary); // Slider style
    LowCut.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 90, 20); //  Text box position
    LowCut.setTextValueSuffix(" hz");
    LowCut.setTextBoxIsEditable(true);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    HighCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "HighCut Freq", HighCut); // Attaches slider with apvts
    LowCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "LowCut Freq", LowCut); // Attaches slider with apvts

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::pink.darker(0.3));
    getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::pink.darker(0.3));
    getLookAndFeel().setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    setSize (400, 300);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

SimpleVSTAudioProcessorEditor::~SimpleVSTAudioProcessorEditor()
{

}

//==============================================================================
void SimpleVSTAudioProcessorEditor::paint (juce::Graphics& g)
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    background = juce::ImageCache::getFromMemory(BinaryData::ds2_jpg, BinaryData::ds2_jpgSize);
    g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    g.setColour(juce::Colours::pink.darker(0.3));
    g.drawRoundedRectangle(getWidth() / 2 + 35, 60, getWidth() / 2 - 70, getHeight() / 2 + 20, 20.0f, 4.0f);
    g.drawRoundedRectangle(0 + 35, 60, getWidth() / 2 - 70, getHeight() / 2 + 20, 20.0f, 4.0f);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void SimpleVSTAudioProcessorEditor::resized()
{
    HighCut.setBounds(getWidth() / 2, 60, getWidth() / 2, getHeight() / 2); // This function is in charge of placing slider on the screen
    LowCut.setBounds(0, 60, getWidth() / 2, getHeight() / 2);
}
