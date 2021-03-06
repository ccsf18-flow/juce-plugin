/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "socket_handle.cpp"

//==============================================================================
Vst_midiAudioProcessor::Vst_midiAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    start_threads();
}

Vst_midiAudioProcessor::~Vst_midiAudioProcessor()
{
}

//==============================================================================
const String Vst_midiAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Vst_midiAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Vst_midiAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Vst_midiAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Vst_midiAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Vst_midiAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Vst_midiAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Vst_midiAudioProcessor::setCurrentProgram (int index)
{
}

const String Vst_midiAudioProcessor::getProgramName (int index)
{
    return {};
}

void Vst_midiAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Vst_midiAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Vst_midiAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Vst_midiAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void Vst_midiAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    buffer.clear();
    MidiBuffer processedMidi;
    int time;
    MidiMessage m;
    //Basic iterate strucutre for note buffer
    for (MidiBuffer::Iterator i (midiMessages); i.getNextEvent (m, time);)
    {
        if (m.isNoteOn())
        {
            uint8 newVel = (uint8)noteOnVel;
            m = MidiMessage::noteOn(m.getChannel(), m.getNoteNumber(), newVel); //Add properties to new note -> We probably care about getNoteNumber()
            if(m.getNoteNumber() < 247 && m.getNoteNumber() > 0){
                beat_msg msg = {static_cast<uint8_t>((m.getNoteNumber())), 100};
                queue_message(1, 200, msg);
            }
            printf("Going with channel %d\n", m.getChannel());
        }
        else if (m.isNoteOff())
        {
            beat_msg msg = {static_cast<uint8_t>((m.getNoteNumber())), 100};
            queue_message(2, 200, msg);
        }
        else if (m.isAftertouch())
        {
        }
        else if (m.isPitchWheel())
        {
        }
        //add event adds note to buffer
        processedMidi.addEvent (m, time);
    }
    //Buffers have to be switched due to some memory safety issue
    midiMessages.swapWith (processedMidi);
    
    /*
        Angle -> arduino
     
     */
}

//==============================================================================
bool Vst_midiAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Vst_midiAudioProcessor::createEditor()
{
    return new Vst_midiAudioProcessorEditor (*this);
}

//==============================================================================
void Vst_midiAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    
}

void Vst_midiAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Vst_midiAudioProcessor();
}
