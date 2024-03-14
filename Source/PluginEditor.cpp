#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleVSTAudioProcessorEditor::SimpleVSTAudioProcessorEditor (SimpleVSTAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    addAndMakeVisible(HighCut);

    HighCut.setSliderStyle(juce::Slider::SliderStyle::Rotary); // Slider style
    HighCut.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20); //  Text box position

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    addAndMakeVisible(LowCut);

    LowCut.setSliderStyle(juce::Slider::SliderStyle::Rotary); // Slider style
    LowCut.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20); //  Text box position

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
    g.fillAll (juce::Colours::black.brighter(0.2));
}

void SimpleVSTAudioProcessorEditor::resized()
{
    HighCut.setBounds(getWidth()/2, getHeight() / 2, getWidth() / 2, getHeight()/2); // This function is in charge of placing slider on the screen
    LowCut.setBounds(0, getHeight() / 2, getWidth() / 2, getHeight()/2);
}
