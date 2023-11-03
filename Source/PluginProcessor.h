/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class SimpleEQ3AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SimpleEQ3AudioProcessor();
    ~SimpleEQ3AudioProcessor() override;

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

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    juce::AudioProcessorValueTreeState apvts {*this,
        nullptr, "Parameters", createParameterLayout()};

private:

    using Filter = juce::dsp::IIR::Filter<float>;  // declaring an alias for readability
    using CutFilter = juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter>;  // In order to go up to -48dB for filter slope, we need to chain filters
    using MonoChain = juce::dsp::ProcessorChain<CutFilter, Filter, CutFilter>;  // Full chain allowing us to chain the HP, peak and LP filters

    MonoChain leftChain, rightChain;  // We need two instances if we want to do stereo processing

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleEQ3AudioProcessor)
};
