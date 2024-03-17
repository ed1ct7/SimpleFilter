#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class SimpleVSTAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SimpleVSTAudioProcessorEditor (SimpleVSTAudioProcessor&);
    ~SimpleVSTAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::Image background;

    juce::Slider HighCut, LowCut; // Creates the sliders
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> HighCutAttachment, LowCutAttachment; // Makes an object to provide a connection between slider and apvts

    SimpleVSTAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleVSTAudioProcessorEditor)
};
