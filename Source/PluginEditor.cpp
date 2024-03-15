#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleVSTAudioProcessorEditor::SimpleVSTAudioProcessorEditor (SimpleVSTAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    addAndMakeVisible(HighCut);

    HighCut.setSliderStyle(juce::Slider::SliderStyle::Rotary); // Slider style
    HighCut.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 100, 30); //  Text box position
    HighCut.setTextValueSuffix(" hz");

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    addAndMakeVisible(LowCut);

    LowCut.setSliderStyle(juce::Slider::SliderStyle::Rotary); // Slider style
    LowCut.setTextBoxStyle(juce::Slider::TextBoxAbove, true, 100, 30); //  Text box position
    LowCut.setTextValueSuffix(" hz");

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    HighCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "HighCut Freq", HighCut); // Attaches slider with apvts
    LowCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "LowCut Freq", LowCut); // Attaches slider with apvts

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
    background = juce::ImageCache::getFromMemory(BinaryData::_543_png, BinaryData::_543_pngSize);
    g.fillAll (juce::Colours::black.brighter(0.2));
    g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit);
}

void SimpleVSTAudioProcessorEditor::resized()
{
    HighCut.setBounds(200, getHeight() / 2, 150, getHeight() / 2); // This function is in charge of placing slider on the screen
    LowCut.setBounds(40, getHeight() / 2, 150, getHeight()/2);
}
