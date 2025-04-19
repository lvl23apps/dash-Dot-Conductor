/*
  ==============================================================================

 * Dash Dot Conductor - A Bidirectional Morse Code MIDI Converter
 * Created by Scotland (LVL23)
 *
 * ⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⣀⢀⠀⠀⠀⢀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀
 * ⠀⠀⠀⠀⠀⠀⠀⣴⠏⠁⠹⠿⣧⠀⣰⠋⠁⠉⠳⡄⠀⠀⠀⠀⠀⠀⠀⠀
 * ⠀⠀⠀⠀⠀⠀⠀⡏⠀⠀⠀⡇⢻⢠⣯⠀⠀⠈⢂⢻⡐⠀⠀⠀⠀⠀⠀⠀
 * ⠀⠀⠀⢀⠀⠀⢰⡗⠀⠀⣼⣁⣾⢸⡇⠀⠀⢀⣼⢨⡇⢀⣾⡴⠿⣯⠀⠀
 * ⠀⣴⠿⢛⡿⣦⡘⣧⡀⠀⡼⣳⡟⢸⣇⠀⢠⣈⠍⣹⣿⣾⠙⠀⢢⠹⣧⠀
 * ⠀⣿⠀⠀⠸⡮⢻⣎⠻⢷⡿⠋⠀⠀⠙⠳⢾⣿⠞⠁⣿⠁⠀⠀⠸⠃⣿⠀
 * ⢠⣿⡄⠀⠀⢸⠸⣏⠀⢀⣴⣾⡿⣿⣦⣤⣦⣤⣄⠀⢹⡆⠀⠀⠀⡆⣿⠄
 * ⠀⢿⣏⠁⠀⢸⣣⣿⢀⣿⠋⠀⠀⠀⠉⠉⠉⠉⠻⣷⠀⢿⡄⢀⡞⣠⡟⠀
 * ⠀⠐⢿⣯⣀⡾⠛⢃⣼⡏⠀⠀⠀⠀⠀⠈⡆⠀⠀⢿⣧⠸⠿⣿⠾⠋⠀⠀
 * ⠀⠀⠀⠈⠉⢣⣶⡿⠛⠀⠀⠀⠀⠀⠀⠀⠉⢶⣤⡈⢻⣧⠀⠀⠀⠀⠀⠀
 * ⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⡽⢿⠁⢰⣿⡇⠀⠀⠀⠀⠀
 * ⠀⠀⠀⠀⠀⢻⣿⣶⣦⣄⡀⠀⠀⠀⠰⣿⣏⣤⣞⣀⣼⣿⠀⠀⠀⠀⠀⠀
 * ⠀⠀⠀⠀⠀⠀⠈⠉⠉⠙⢿⣦⣄⣤⣴⡾⠛⠉⠟⠉⠉⠀⠀⠀⠀⠀⠀⠀
 * ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠛⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
 *
 * "The Universe says simply, and with an elegance that has little to do with
 * feelings: Here is your existence. You are free to interpret it however you wish."
 * - Iain M. Banks
 *
 * OwO What's this? A Morse code converter? *nuzzles your MIDI notes*
 * Let's turn your bweeps and boops into the most kawaii Morse patterns! UwU
 *
 * Version 1.0.0 - April 2025

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct MorseSymbol {
    char character;
    const char* code;
};

// Morse code mapping
static const MorseSymbol morseTable[] = {
    {'A', ".-"},
    {'B', "-..."},
    {'C', "-.-."},
    {'D', "-.."},
    {'E', "."},
    {'F', "..-."},
    {'G', "--."},
    {'H', "...."},
    {'I', ".."},
    {'J', ".---"},
    {'K', "-.-"},
    {'L', ".-.."},
    {'M', "--"},
    {'N', "-."},
    {'O', "---"},
    {'P', ".--."},
    {'Q', "--.-"},
    {'R', ".-."},
    {'S', "..."},
    {'T', "-"},
    {'U', "..-"},
    {'V', "...-"},
    {'W', ".--"},
    {'X', "-..-"},
    {'Y', "-.--"},
    {'Z', "--.."},
    {'0', "-----"},
    {'1', ".----"},
    {'2', "..---"},
    {'3', "...--"},
    {'4', "....-"},
    {'5', "....."},
    {'6', "-...."},
    {'7', "--..."},
    {'8', "---.."},
    {'9', "----."},
    {' ', "/"}
};

class DashDotConductorProcessor : public juce::AudioProcessor {
public:
    DashDotConductorProcessor();
    ~DashDotConductorProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // MIDI note generation parameters
    juce::AudioParameterInt* midiChannel;
    juce::AudioParameterInt* dotNoteNumber;
    juce::AudioParameterInt* dashNoteNumber;
    juce::AudioParameterInt* dotVelocity;
    juce::AudioParameterInt* dashVelocity;
    juce::AudioParameterFloat* dotDuration;    // in beats
    juce::AudioParameterFloat* dashDuration;   // in beats
    juce::AudioParameterFloat* symbolGap;      // in beats
    juce::AudioParameterFloat* letterGap;      // in beats
    juce::AudioParameterFloat* wordGap;        // in beats

    // Morse code input/output
    void setMorseInput(const juce::String& input);
    void generateMIDINotes();
    
    // MIDI to Morse code conversion
    juce::String convertMIDIToMorse(const juce::MidiBuffer& midiBuffer);
    juce::String convertMorseToText(const juce::String& morseCode);

private:
    juce::String morseInput;
    juce::Array<juce::MidiMessage> generatedNotes;
    int currentNoteIndex;
    int samplesPerQuarterNote;
    double currentSampleRate;

    juce::AudioParameterBool* autoGenerateParam;
    juce::AudioParameterChoice* inputTextParam;
    
    struct MIDINote {
        int noteNumber;
        double startTime;  // in beats
        double duration;   // in beats
    };
    juce::Array<MIDINote> midiNotes;
    bool midiChanged;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DashDotConductorProcessor)
};

class DashDotConductorEditor : public juce::AudioProcessorEditor,
                          public juce::TextEditor::Listener,
                          public juce::Button::Listener {
public:
    DashDotConductorEditor(DashDotConductorProcessor&);
    ~DashDotConductorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void textEditorTextChanged(juce::TextEditor&) override;
    void buttonClicked(juce::Button* button) override;

private:
    DashDotConductorProcessor& processor;

    juce::TextEditor textInput;
    juce::TextEditor morseOutput;
    juce::TextEditor decodedTextOutput;
    juce::TextButton generateButton;
    juce::TextButton decodeMIDIButton;
    juce::Label titleLabel;
    juce::Label textLabel;
    juce::Label morseLabel;
    juce::Label decodedTextLabel;

    juce::Slider dotNoteSlider;
    juce::Slider dashNoteSlider;
    juce::Slider dotDurationSlider;
    juce::Slider dashDurationSlider;
    juce::Slider dotVelocitySlider;
    juce::Slider dashVelocitySlider;
    
    juce::Label dotNoteLabel;
    juce::Label dashNoteLabel;
    juce::Label dotDurationLabel;
    juce::Label dashDurationLabel;
    juce::Label dotVelocityLabel;
    juce::Label dashVelocityLabel;

    juce::String convertTextToMorse(const juce::String& text);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DashDotConductorEditor)
};
