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
void SimpleVSTAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    juce::dsp::ProcessSpec spec;

    spec.maximumBlockSize = samplesPerBlock;

    spec.numChannels = 1;

    spec.sampleRate = sampleRate;

    leftChain.prepare(spec);
    rightChain.prepare(spec);
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

void SimpleVSTAudioProcessor::updateFilter() {

    auto cutCoefficientsH = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(lowCutFreq, getSampleRate(), 6);

    auto& leftLowCut = leftChain.get<ChainPositions::LowCut>();

    *leftLowCut.get<0>().coefficients = *cutCoefficientsH[0];
    *leftLowCut.get<1>().coefficients = *cutCoefficientsH[1];
    *leftLowCut.get<2>().coefficients = *cutCoefficientsH[2];


    auto& rightLowCut = rightChain.get<ChainPositions::LowCut>();

    *rightLowCut.get<0>().coefficients = *cutCoefficientsH[0];
    *rightLowCut.get<1>().coefficients = *cutCoefficientsH[1];
    *rightLowCut.get<2>().coefficients = *cutCoefficientsH[2];

    auto cutCoefficientsL = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(highCutFreq, getSampleRate(), 6);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    auto& leftHighCut = leftChain.get<ChainPositions::HighCut>();
    *leftHighCut.get<0>().coefficients = *cutCoefficientsL[0];
    *leftHighCut.get<1>().coefficients = *cutCoefficientsL[1];
    *leftHighCut.get<2>().coefficients = *cutCoefficientsL[2];



    auto& rightHighCut = rightChain.get<ChainPositions::HighCut>();
    *rightHighCut.get<0>().coefficients = *cutCoefficientsL[0];
    *rightHighCut.get<1>().coefficients = *cutCoefficientsL[1];
    *rightHighCut.get<2>().coefficients = *cutCoefficientsL[2];
}

void SimpleVSTAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    lowCutFreq = apvts.getRawParameterValue("LowCut Freq")->load();
    highCutFreq = apvts.getRawParameterValue("HighCut Freq")->load();

    updateFilter();

    juce::dsp::AudioBlock<float> block(buffer);

    auto leftBlock = block.getSingleChannelBlock(0);
    auto rightBlock = block.getSingleChannelBlock(1);

    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);

    leftChain.process(leftContext);
    rightChain.process(rightContext);
}

//==============================================================================
bool SimpleVSTAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* SimpleVSTAudioProcessor::createEditor()
{
    return new SimpleVSTAudioProcessorEditor(*this);
}

//==============================================================================
void SimpleVSTAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{

}

void SimpleVSTAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{

}

juce::AudioProcessorValueTreeState::ParameterLayout SimpleVSTAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>("LowCut Freq",
                                                            "LowCut Freq",
                                                            juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.6),20.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>("HighCut Freq",
                                                            "HighCut Freq",
                                                            juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 0.6f),20000.f));

    return layout;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleVSTAudioProcessor();
}
