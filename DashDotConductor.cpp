// DashDotConductor.cpp
/* Dash Dot Conductor - A Bidirectional Morse Code MIDI Converter
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
 */

#include "DashDotConductor.h"

DashDotConductorProcessor::DashDotConductorProcessor()
    : AudioProcessor(BusesProperties()
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      currentNoteIndex(0),
      samplesPerQuarterNote(0),
      currentSampleRate(44100.0),
      midiChanged(false)
{
    addParameter(midiChannel = new juce::AudioParameterInt("midiChannel", "MIDI Channel", 1, 16, 1));
    addParameter(dotNoteNumber = new juce::AudioParameterInt("dotNote", "Dot Note Number", 0, 127, 60));  // Middle C
    addParameter(dashNoteNumber = new juce::AudioParameterInt("dashNote", "Dash Note Number", 0, 127, 64)); // E
    addParameter(dotVelocity = new juce::AudioParameterInt("dotVelocity", "Dot Velocity", 1, 127, 100));
    addParameter(dashVelocity = new juce::AudioParameterInt("dashVelocity", "Dash Velocity", 1, 127, 100));
    addParameter(dotDuration = new juce::AudioParameterFloat("dotDuration", "Dot Duration (beats)", 0.1f, 1.0f, 0.25f));
    addParameter(dashDuration = new juce::AudioParameterFloat("dashDuration", "Dash Duration (beats)", 0.1f, 2.0f, 0.5f));
    addParameter(symbolGap = new juce::AudioParameterFloat("symbolGap", "Symbol Gap (beats)", 0.05f, 0.5f, 0.1f));
    addParameter(letterGap = new juce::AudioParameterFloat("letterGap", "Letter Gap (beats)", 0.1f, 1.0f, 0.5f));
    addParameter(wordGap = new juce::AudioParameterFloat("wordGap", "Word Gap (beats)", 0.5f, 2.0f, 1.0f));
    
    addParameter(autoGenerateParam = new juce::AudioParameterBool("autoGenerate", "Auto-generate MIDI", true));
    
    juce::StringArray presetTexts;
    presetTexts.add("SOS");
    presetTexts.add("HELLO WORLD");
    presetTexts.add("MORSE CODE");
    
    addParameter(inputTextParam = new juce::AudioParameterChoice("inputText", "Input Text", presetTexts, 0));
}

DashDotConductorProcessor::~DashDotConductorProcessor() {}

void DashDotConductorProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    currentSampleRate = sampleRate;
    // Calculate samples per quarter note (assuming 120 BPM)
    samplesPerQuarterNote = static_cast<int>((60.0 / 120.0) * sampleRate);
}

void DashDotConductorProcessor::releaseResources() {
    generatedNotes.clear();
    currentNoteIndex = 0;
}

void DashDotConductorProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    // Clear the buffer
    buffer.clear();
    
    // Process the MIDI events
    if (!generatedNotes.isEmpty() && currentNoteIndex < generatedNotes.size()) {
        midiMessages.addEvent(generatedNotes[currentNoteIndex], 0);
        currentNoteIndex++;
    }
    
    // Capture incoming MIDI for conversion to Morse
    if (!midiMessages.isEmpty() && midiMessages != juce::MidiBuffer()) {
        midiChanged = true;
        
        // Process incoming MIDI and store notes for conversion
        midiNotes.clear();
        
        for (const auto metadata : midiMessages) {
            auto message = metadata.getMessage();
            
            if (message.isNoteOn()) {
                MIDINote note;
                note.noteNumber = message.getNoteNumber();
                note.startTime = metadata.samplePosition / currentSampleRate;
                note.duration = 0.0; // Will be set when note off is received
                midiNotes.add(note);
            }
            else if (message.isNoteOff()) {
                // Find the matching note on and update its duration
                for (int i = midiNotes.size() - 1; i >= 0; --i) {
                    if (midiNotes[i].noteNumber == message.getNoteNumber() && midiNotes[i].duration == 0.0) {
                        midiNotes[i].duration = (metadata.samplePosition / currentSampleRate) - midiNotes[i].startTime;
                        break;
                    }
                }
            }
        }
    }
}

juce::AudioProcessorEditor* DashDotConductorProcessor::createEditor() {
    return new DashDotConductorEditor(*this);
}

bool DashDotConductorProcessor::hasEditor() const {
    return true;
}

const juce::String DashDotConductorProcessor::getName() const {
    return "Dash Dot Conductor";
}

bool DashDotConductorProcessor::acceptsMidi() const {
    return true;
}

bool DashDotConductorProcessor::producesMidi() const {
    return true;
}

bool DashDotConductorProcessor::isMidiEffect() const {
    return true;
}

double DashDotConductorProcessor::getTailLengthSeconds() const {
    return 0.0;
}

int DashDotConductorProcessor::getNumPrograms() {
    return 1;
}

int DashDotConductorProcessor::getCurrentProgram() {
    return 0;
}

void DashDotConductorProcessor::setCurrentProgram(int index) {
}

const juce::String DashDotConductorProcessor::getProgramName(int index) {
    return "Default";
}

void DashDotConductorProcessor::changeProgramName(int index, const juce::String& newName) {
}

void DashDotConductorProcessor::getStateInformation(juce::MemoryBlock& destData) {
    juce::MemoryOutputStream stream(destData, true);
    stream.writeFloat(*dotDuration);
    stream.writeFloat(*dashDuration);
    stream.writeInt(*dotNoteNumber);
    stream.writeInt(*dashNoteNumber);
    stream.writeInt(*dotVelocity);
    stream.writeInt(*dashVelocity);
    stream.writeFloat(*symbolGap);
    stream.writeFloat(*letterGap);
    stream.writeFloat(*wordGap);
    stream.writeInt(*midiChannel);
    stream.writeBool(*autoGenerateParam);
    stream.writeInt(inputTextParam->getIndex());
    stream.writeString(morseInput);
}

void DashDotConductorProcessor::setStateInformation(const void* data, int sizeInBytes) {
    juce::MemoryInputStream stream(data, static_cast<size_t>(sizeInBytes), false);
    
    if (stream.getNumBytesRemaining() > 0) {
        *dotDuration = stream.readFloat();
        *dashDuration = stream.readFloat();
        *dotNoteNumber = stream.readInt();
        *dashNoteNumber = stream.readInt();
        *dotVelocity = stream.readInt();
        *dashVelocity = stream.readInt();
        *symbolGap = stream.readFloat();
        *letterGap = stream.readFloat();
        *wordGap = stream.readFloat();
        *midiChannel = stream.readInt();
        *autoGenerateParam = stream.readBool();
        inputTextParam->setIndex(stream.readInt());
        morseInput = stream.readString();
        
        if (*autoGenerateParam)
            generateMIDINotes();
    }
}

void DashDotConductorProcessor::setMorseInput(const juce::String& input) {
    morseInput = input;
    currentNoteIndex = 0;
    
    if (*autoGenerateParam)
        generateMIDINotes();
}

void DashDotConductorProcessor::generateMIDINotes() {
    generatedNotes.clear();
    currentNoteIndex = 0;
    
    double currentTime = 0.0;  // in beats
    int midiChannelIndex = *midiChannel - 1;  // MIDI channels are 0-based internally
    
    for (int i = 0; i < morseInput.length(); ++i) {
        char c = morseInput[i];
        
        if (c == '.') {
            // Add Note On for dot
            juce::MidiMessage noteOn = juce::MidiMessage::noteOn(midiChannelIndex, *dotNoteNumber, (uint8) *dotVelocity);
            generatedNotes.add(noteOn);
            
            // Add Note Off for dot
            juce::MidiMessage noteOff = juce::MidiMessage::noteOff(midiChannelIndex, *dotNoteNumber);
            currentTime += *dotDuration;
            generatedNotes.add(noteOff);
            
            // Add symbol gap if not the last symbol
            if (i < morseInput.length() - 1 && morseInput[i + 1] != ' ' && morseInput[i + 1] != '/') {
                currentTime += *symbolGap;
            }
        }
        else if (c == '-') {
            // Add Note On for dash
            juce::MidiMessage noteOn = juce::MidiMessage::noteOn(midiChannelIndex, *dashNoteNumber, (uint8) *dashVelocity);
            generatedNotes.add(noteOn);
            
            // Add Note Off for dash
            juce::MidiMessage noteOff = juce::MidiMessage::noteOff(midiChannelIndex, *dashNoteNumber);
            currentTime += *dashDuration;
            generatedNotes.add(noteOff);
            
            // Add symbol gap if not the last symbol
            if (i < morseInput.length() - 1 && morseInput[i + 1] != ' ' && morseInput[i + 1] != '/') {
                currentTime += *symbolGap;
            }
        }
        else if (c == ' ') {
            // Add letter gap
            currentTime += *letterGap;
        }
        else if (c == '/') {
            // Add word gap
            currentTime += *wordGap;
        }
    }
}

DashDotConductorEditor::DashDotConductorEditor(DashDotConductorProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    // Set up text input
    addAndMakeVisible(titleLabel);
    titleLabel.setText("Dash Dot Conductor", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(18.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);
    
    addAndMakeVisible(textLabel);
    textLabel.setText("Input Text:", juce::dontSendNotification);
    
    addAndMakeVisible(textInput);
    textInput.setMultiLine(false);
    textInput.setReturnKeyStartsNewLine(false);
    textInput.setReadOnly(false);
    textInput.setScrollbarsShown(true);
    textInput.setCaretVisible(true);
    textInput.setPopupMenuEnabled(true);
    textInput.setText("SOS", juce::dontSendNotification);
    textInput.addListener(this);
    
    addAndMakeVisible(morseLabel);
    morseLabel.setText("Morse Code:", juce::dontSendNotification);
    
    addAndMakeVisible(morseOutput);
    morseOutput.setMultiLine(false);
    morseOutput.setReturnKeyStartsNewLine(false);
    morseOutput.setReadOnly(true);
    morseOutput.setScrollbarsShown(true);
    morseOutput.setCaretVisible(false);
    morseOutput.setPopupMenuEnabled(true);
    morseOutput.setText(convertTextToMorse(textInput.getText()), juce::dontSendNotification);
    
    addAndMakeVisible(generateButton);
    generateButton.setButtonText("Generate MIDI");
    generateButton.addListener(this);
    
    addAndMakeVisible(decodedTextLabel);
    decodedTextLabel.setText("Decoded Text:", juce::dontSendNotification);
    
    addAndMakeVisible(decodedTextOutput);
    decodedTextOutput.setMultiLine(false);
    decodedTextOutput.setReturnKeyStartsNewLine(false);
    decodedTextOutput.setReadOnly(true);
    decodedTextOutput.setScrollbarsShown(true);
    decodedTextOutput.setCaretVisible(false);
    decodedTextOutput.setPopupMenuEnabled(true);
    
    addAndMakeVisible(decodeMIDIButton);
    decodeMIDIButton.setButtonText("Decode MIDI to Morse/Text");
    decodeMIDIButton.addListener(this);
    
    // Parameter sliders
    addAndMakeVisible(dotNoteLabel);
    dotNoteLabel.setText("Dot Note", juce::dontSendNotification);
    addAndMakeVisible(dotNoteSlider);
    dotNoteSlider.setRange(0, 127, 1);
    dotNoteSlider.setValue(processor.dotNoteNumber->get());
    dotNoteSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    
    addAndMakeVisible(dashNoteLabel);
    dashNoteLabel.setText("Dash Note", juce::dontSendNotification);
    addAndMakeVisible(dashNoteSlider);
    dashNoteSlider.setRange(0, 127, 1);
    dashNoteSlider.setValue(processor.dashNoteNumber->get());
    dashNoteSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    
    addAndMakeVisible(dotDurationLabel);
    dotDurationLabel.setText("Dot Duration", juce::dontSendNotification);
    addAndMakeVisible(dotDurationSlider);
    dotDurationSlider.setRange(0.1, 1.0, 0.05);
    dotDurationSlider.setValue(processor.dotDuration->get());
    dotDurationSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    
    addAndMakeVisible(dashDurationLabel);
    dashDurationLabel.setText("Dash Duration", juce::dontSendNotification);
    addAndMakeVisible(dashDurationSlider);
    dashDurationSlider.setRange(0.1, 2.0, 0.05);
    dashDurationSlider.setValue(processor.dashDuration->get());
    dashDurationSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    
    addAndMakeVisible(dotVelocityLabel);
    dotVelocityLabel.setText("Dot Velocity", juce::dontSendNotification);
    addAndMakeVisible(dotVelocitySlider);
    dotVelocitySlider.setRange(1, 127, 1);
    dotVelocitySlider.setValue(processor.dotVelocity->get());
    dotVelocitySlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    
    addAndMakeVisible(dashVelocityLabel);
    dashVelocityLabel.setText("Dash Velocity", juce::dontSendNotification);
    addAndMakeVisible(dashVelocitySlider);
    dashVelocitySlider.setRange(1, 127, 1);
    dashVelocitySlider.setValue(processor.dashVelocity->get());
    dashVelocitySlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    
    // Window size
    setSize(500, 580);
    
    // Send initial Morse code to processor
    processor.setMorseInput(convertTextToMorse(textInput.getText()));
}

DashDotConductorEditor::~DashDotConductorEditor() {
}

void DashDotConductorEditor::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::white);
    g.setColour(juce::Colours::black);
}
