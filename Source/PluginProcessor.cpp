#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleVSTAudioProcessor::SimpleVSTAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

SimpleVSTAudioProcessor::~SimpleVSTAudioProcessor()
{
}

//==============================================================================
const juce::String SimpleVSTAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleVSTAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimpleVSTAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimpleVSTAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimpleVSTAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleVSTAudioProcessor::getNumPrograms()
{
    return 1;  
}

int SimpleVSTAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleVSTAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SimpleVSTAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimpleVSTAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SimpleVSTAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    filter.reset();
    
    /////////// Hands to the dsp module basic parameters to work with ////////////
    
    juce::dsp::ProcessSpec specs;

    specs.maximumBlockSize = samplesPerBlock;
    specs.numChannels = getTotalNumInputChannels();
    specs.sampleRate = sampleRate;
   
    filter.prepare(specs);

    ///////////////////////////////////////////////////////////////////////////////
}

void SimpleVSTAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleVSTAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else

    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SimpleVSTAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    auto g = apvts.getRawParameterValue("LowCut Freq");

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i){
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    filter.setCutoffFrequency(g->load());

    auto AudioBlock = juce::dsp::AudioBlock<float>(buffer);
    auto context = juce::dsp::ProcessContextReplacing<float>(AudioBlock);

    filter.process(context);
}

//==============================================================================
bool SimpleVSTAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* SimpleVSTAudioProcessor::createEditor()
{
    return new SimpleVSTAudioProcessorEditor (*this);
}

//==============================================================================
void SimpleVSTAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{

}

void SimpleVSTAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{

}

juce::AudioProcessorValueTreeState::ParameterLayout SimpleVSTAudioProcessor::createParameterLayout() //Basicly place which collects all the information from and for the GUI 
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>("LowCut Freq",
                                                           "LowCut Freq",
                                                           juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 1.0f),20.0f));
    return layout;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleVSTAudioProcessor();
}

void SimpleVSTAudioProcessor::setType() {
    filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
}