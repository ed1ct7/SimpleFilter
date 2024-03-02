#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class SimpleVSTAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SimpleVSTAudioProcessor();
    ~SimpleVSTAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void SimpleVSTAudioProcessor::updateFilter();

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts { *this, nullptr, "Parameters", createParameterLayout() }; // Constructor for apvts

private:

    juce::dsp::IIR::Filter <float> HPfilter; // Creates a filter

    juce::dsp::ProcessorDuplicator < juce::dsp::IIR::Filter < float >, juce::dsp::IIR::Coefficients <float>> HPFilter // Makes from mono stereo sound
                                                      { juce::dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(), 20000.0f, 0.1f ) };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleVSTAudioProcessor)
};
