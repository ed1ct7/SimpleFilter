#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleVSTAudioProcessorEditor::SimpleVSTAudioProcessorEditor (SimpleVSTAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(LowCut); // Creats a child component

    LowCut.setSliderStyle(juce::Slider::SliderStyle::Rotary); // Slider style
    LowCut.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 20); //  Text box position

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
    LowCut.setBounds(getBounds()); // This function is in charge of the placing slider on the screen
}
