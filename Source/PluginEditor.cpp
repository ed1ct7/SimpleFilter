#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleVSTAudioProcessorEditor::SimpleVSTAudioProcessorEditor (SimpleVSTAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(HighCut); // Creats a child component

    HighCut.setSliderStyle(juce::Slider::SliderStyle::Rotary); // Slider style
    HighCut.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20); //  Text box position

    HighCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "HighCut Freq", HighCut); // Attaches slider with apvts

    setSize (400, 300);
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
    HighCut.setBounds(getBounds()); // This function is in charge of the placing slider on the screen
}
