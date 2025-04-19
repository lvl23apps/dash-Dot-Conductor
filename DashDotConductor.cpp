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

// DashDotConductor.cpp


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
    if (!midiMessages.isEmpty()) {
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
                        auto& note = midiNotes.getReference(i);
                        note.duration = (metadata.samplePosition / currentSampleRate) - note.startTime;
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
        int choiceIndex = stream.readInt();
        *inputTextParam = choiceIndex;
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
            juce::MidiMessage noteOn = juce::MidiMessage::noteOn(midiChannelIndex, *dotNoteNumber, static_cast<float>(*dotVelocity) / 127.0f);
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
            juce::MidiMessage noteOn = juce::MidiMessage::noteOn(midiChannelIndex, *dashNoteNumber, static_cast<float>(*dashVelocity) / 127.0f);
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

// Add the new MIDI to Morse and Morse to Text conversion functions

juce::String DashDotConductorProcessor::convertMIDIToMorse(const juce::MidiBuffer& midiBuffer) {
    juce::String morseCode;
    
    // Sort MIDI notes by start time
    struct {
        bool operator()(const MIDINote& a, const MIDINote& b) const { return a.startTime < b.startTime; }
    } customSort;
    
    juce::Array<MIDINote> sortedNotes = midiNotes;
    std::sort(sortedNotes.begin(), sortedNotes.end(), customSort);
    
    // Analyze patterns to determine dots and dashes
    double avgDuration = 0.0;
    if (!sortedNotes.isEmpty()) {
        // Calculate average duration to help determine dots vs dashes
        double totalDuration = 0.0;
        for (const auto& note : sortedNotes) {
            totalDuration += note.duration;
        }
        avgDuration = totalDuration / sortedNotes.size();
    }
    
    // Threshold to distinguish dots from dashes (ratio)
    const double dotDashThreshold = 0.67;
    
    // Analyze time gaps to determine letter and word boundaries
    double avgGap = 0.0;
    if (sortedNotes.size() > 1) {
        double totalGap = 0.0;
        int gapCount = 0;
        
        for (int i = 1; i < sortedNotes.size(); ++i) {
            double gap = sortedNotes[i].startTime - (sortedNotes[i-1].startTime + sortedNotes[i-1].duration);
            if (gap > 0.01) { // Ignore very small gaps
                totalGap += gap;
                gapCount++;
            }
        }
        
        if (gapCount > 0) {
            avgGap = totalGap / gapCount;
        }
    }
    
    // Constants for gap classification
    const double symbolGapThreshold = 0.5;  // Below this ratio of avgGap is a symbol gap
    const double letterGapThreshold = 1.5;  // Above symbolGap but below this is a letter gap
                                           // Above letterGap is a word gap
    
    // Interpret the notes as Morse code
    for (int i = 0; i < sortedNotes.size(); ++i) {
        const auto& note = sortedNotes[i];
        
        // Determine if it's a dot or dash based on duration
        if (note.duration < avgDuration * dotDashThreshold) {
            morseCode += ".";
        } else {
            morseCode += "-";
        }
        
        // Add appropriate spacing based on gap to next note
        if (i < sortedNotes.size() - 1) {
            double gap = sortedNotes[i+1].startTime - (note.startTime + note.duration);
            
            if (gap > avgGap * letterGapThreshold) {
                morseCode += " / "; // Word gap
            } else if (gap > avgGap * symbolGapThreshold) {
                morseCode += " "; // Letter gap
            }
            // Otherwise, it's a symbol gap (no explicit space needed)
        }
    }
    
    return morseCode;
}

juce::String DashDotConductorProcessor::convertMorseToText(const juce::String& morseCode) {
    juce::String result;
    juce::StringArray morseWords;
    
    // Split by word delimiter (/)
    morseWords = juce::StringArray::fromTokens(morseCode, "/", "");
    
    for (const auto& word : morseWords) {
        // Split by letter delimiter (space)
        juce::StringArray letters = juce::StringArray::fromTokens(word.trim(), " ", "");
        
        for (const auto& letter : letters) {
            bool found = false;
            
            // Find the Morse code in the lookup table
            for (int i = 0; i < sizeof(morseTable) / sizeof(morseTable[0]); ++i) {
                if (letter == morseTable[i].code) {
                    result += morseTable[i].character;
                    found = true;
                    break;
                }
            }
            
            // If not found, add a placeholder
            if (!found && letter.isNotEmpty()) {
                result += "?";
            }
        }
        
        // Add a space between words
        result += " ";
    }
    
    return result.trim();
}

DashDotConductorEditor::DashDotConductorEditor(DashDotConductorProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    // Set up text input
    addAndMakeVisible(titleLabel);
    titleLabel.setText("Dash Dot Conductor", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(juce::Font::getDefaultSansSerifFontName(), 18.0f, juce::Font::bold));
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

void DashDotConductorEditor::resized() {
    auto area = getLocalBounds().reduced(10);
    
    titleLabel.setBounds(area.removeFromTop(30));
    area.removeFromTop(10);
    
    auto textRow = area.removeFromTop(25);
    textLabel.setBounds(textRow.removeFromLeft(100));
    textInput.setBounds(textRow);
    
    area.removeFromTop(10);
    
    auto morseRow = area.removeFromTop(25);
    morseLabel.setBounds(morseRow.removeFromLeft(100));
    morseOutput.setBounds(morseRow);
    
    area.removeFromTop(20);
    
    auto buttonRow = area.removeFromTop(30);
    auto buttonWidth = 230;
    generateButton.setBounds(buttonRow.withSizeKeepingCentre(buttonWidth, 30));
    
    area.removeFromTop(10);
    
    auto decodedTextRow = area.removeFromTop(25);
    decodedTextLabel.setBounds(decodedTextRow.removeFromLeft(100));
    decodedTextOutput.setBounds(decodedTextRow);
    
    area.removeFromTop(10);
    
    auto decodeButtonRow = area.removeFromTop(30);
    decodeMIDIButton.setBounds(decodeButtonRow.withSizeKeepingCentre(buttonWidth, 30));
    
    area.removeFromTop(20);
    
    auto controlsArea = area.reduced(20, 0);
    auto sliderHeight = 30;
    
    auto dotNoteRow = controlsArea.removeFromTop(sliderHeight);
    dotNoteLabel.setBounds(dotNoteRow.removeFromLeft(100));
    dotNoteSlider.setBounds(dotNoteRow);
    
    controlsArea.removeFromTop(5);
    
    auto dashNoteRow = controlsArea.removeFromTop(sliderHeight);
    dashNoteLabel.setBounds(dashNoteRow.removeFromLeft(100));
    dashNoteSlider.setBounds(dashNoteRow);
    
    controlsArea.removeFromTop(5);
    
    auto dotDurationRow = controlsArea.removeFromTop(sliderHeight);
    dotDurationLabel.setBounds(dotDurationRow.removeFromLeft(100));
    dotDurationSlider.setBounds(dotDurationRow);
    
    controlsArea.removeFromTop(5);
    
    auto dashDurationRow = controlsArea.removeFromTop(sliderHeight);
    dashDurationLabel.setBounds(dashDurationRow.removeFromLeft(100));
    dashDurationSlider.setBounds(dashDurationRow);
    
    controlsArea.removeFromTop(5);
    
    auto dotVelocityRow = controlsArea.removeFromTop(sliderHeight);
    dotVelocityLabel.setBounds(dotVelocityRow.removeFromLeft(100));
    dotVelocitySlider.setBounds(dotVelocityRow);
    
    controlsArea.removeFromTop(5);
    
    auto dashVelocityRow = controlsArea.removeFromTop(sliderHeight);
    dashVelocityLabel.setBounds(dashVelocityRow.removeFromLeft(100));
    dashVelocitySlider.setBounds(dashVelocityRow);
}

void DashDotConductorEditor::textEditorTextChanged(juce::TextEditor& editor) {
    if (&editor == &textInput) {
        juce::String morseCode = convertTextToMorse(editor.getText());
        morseOutput.setText(morseCode, juce::dontSendNotification);
        processor.setMorseInput(morseCode);
    }
}

void DashDotConductorEditor::buttonClicked(juce::Button* button) {
    if (button == &generateButton) {
        processor.generateMIDINotes();
    }
    else if (button == &decodeMIDIButton) {
        // Decode MIDI to Morse
        juce::MidiBuffer dummyBuffer; // We'll use stored notes from the processor
        juce::String morseCode = processor.convertMIDIToMorse(dummyBuffer);
        morseOutput.setText(morseCode, juce::dontSendNotification);
        
        // Convert Morse to text
        juce::String decodedText = processor.convertMorseToText(morseCode);
        decodedTextOutput.setText(decodedText, juce::dontSendNotification);
    }
}

juce::String DashDotConductorEditor::convertTextToMorse(const juce::String& text) {
    juce::String result;
    juce::String upperText = text.toUpperCase();
    
    for (int i = 0; i < upperText.length(); ++i) {
        char c = upperText[i];
        bool found = false;
        
        // Find the character in the morse table
        for (int j = 0; j < sizeof(morseTable) / sizeof(morseTable[0]); ++j) {
            if (morseTable[j].character == c) {
                result += morseTable[j].code;
                found = true;
                break;
            }
        }
        
        // If character is not found in the table, ignore it
        if (!found && c != ' ') {
            continue;
        }
        
        // Add space between letters (but not after a space)
        if (i < upperText.length() - 1 && c != ' ') {
            result += " ";
        }
    }
    
    return result;
}

// Create the audio processor
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new DashDotConductorProcessor();
}
