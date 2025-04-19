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
    
    // Important: Create a new MidiBuffer for the output to ensure messages get written to the track
    juce::MidiBuffer outputBuffer;
    
    // Process the MIDI events - add current note to the output buffer
    if (!generatedNotes.isEmpty() && currentNoteIndex < generatedNotes.size()) {
        // Calculate appropriate sample position within the buffer
        int samplePosition = 0; // Start at the beginning of the buffer
        
        // Add the current note to the output buffer
        outputBuffer.addEvent(generatedNotes[currentNoteIndex], samplePosition);
        
        // Increment to the next note for next time
        currentNoteIndex++;
        
        // Loop back to beginning if we've reached the end
        if (currentNoteIndex >= generatedNotes.size()) {
            // Only reset if we want looping behavior
            if (/* DISABLES CODE */ (false)) { // Set to true if you want looping
                currentNoteIndex = 0;
            }
        }
        
        // Debug output
        DBG("Added MIDI event to buffer, index: " + juce::String(currentNoteIndex-1) +
            " of " + juce::String(generatedNotes.size()));
    }
    
    // Capture incoming MIDI for conversion to Morse
    if (!midiMessages.isEmpty()) {
        midiChanged = true;
        
        // Process incoming MIDI and store notes for conversion
        for (const auto metadata : midiMessages) {
            auto message = metadata.getMessage();
            
            if (message.isNoteOn()) {
                MIDINote note;
                note.noteNumber = message.getNoteNumber();
                note.startTime = metadata.samplePosition / currentSampleRate;
                note.duration = 0.0; // Will be set when note off is received
                midiNotes.add(note);
                
                // Also pass through this MIDI message
                outputBuffer.addEvent(message, metadata.samplePosition);
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
                
                // Also pass through this MIDI message
                outputBuffer.addEvent(message, metadata.samplePosition);
            }
            else {
                // Pass through any other MIDI messages
                outputBuffer.addEvent(message, metadata.samplePosition);
            }
        }
    }
    
    // Replace the input MIDI buffer with our output buffer
    midiMessages.swapWith(outputBuffer);
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
    
    // Store the MIDI messages with proper timestamps
    for (int i = 0; i < morseInput.length(); ++i) {
        char c = morseInput[i];
        
        if (c == '.') {
            // Add Note On for dot
            juce::MidiMessage noteOn = juce::MidiMessage::noteOn(midiChannelIndex, *dotNoteNumber, static_cast<float>(*dotVelocity) / 127.0f);
            generatedNotes.add(noteOn);
            
            // Add Note Off for dot with proper timestamp
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
            
            // Add Note Off for dash with proper timestamp
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
    
    // Print debug info
    DBG("Generated " + juce::String(generatedNotes.size()) + " MIDI messages");
}

// Add a new method to handle text input from Morse code
void DashDotConductorEditor::morseInputChanged() {
    // Get the Morse code from the editor
    juce::String morseCode = morseOutput.getText();
    
    // Convert it to text
    juce::String decodedText = processor.convertMorseToText(morseCode);
    
    // Update the text input field without triggering another change event
    textInput.setText(decodedText, juce::dontSendNotification);
    
    // Update the decoded text field
    decodedTextOutput.setText(decodedText, juce::dontSendNotification);
    
    // Update the processor with the new Morse input
    processor.setMorseInput(morseCode);
}

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
    // Define this here too for safety
    static const int MORSE_TABLE_SIZE = 37;
    
    juce::String result;
    
    // Print the entire Morse table for debugging
    DBG("Morse Table Contents:");
    for (int i = 0; i < MORSE_TABLE_SIZE; ++i) {
        DBG("  [" + juce::String(i) + "] '" + juce::String(morseTable[i].character) +
            "' -> \"" + juce::String(morseTable[i].code) + "\"");
    }
    
    // Debug output
    DBG("Converting morse: \"" + morseCode + "\"");
    
    // Split into words (separated by "/")
    juce::StringArray words;
    if (morseCode.contains("/")) {
        words = juce::StringArray::fromTokens(morseCode, "/", "");
    } else {
        // If no "/" is found, treat the entire string as a single word
        words.add(morseCode);
    }
    
    DBG("Split into " + juce::String(words.size()) + " words");
    
    for (auto& word : words) {
        // Split word into letters (separated by spaces)
        juce::StringArray letters = juce::StringArray::fromTokens(word.trim(), " ", "");
        
        DBG("Word split into " + juce::String(letters.size()) + " letters");
        
        for (auto& letter : letters) {
            // Skip empty letters
            if (letter.isEmpty()) {
                DBG("Empty letter, skipping");
                continue;
            }
                
            // Debug each letter
            DBG("Processing morse letter: \"" + letter + "\"");
            
            bool found = false;
            // Check each entry in the morse table
            for (int i = 0; i < MORSE_TABLE_SIZE; ++i) {
                const juce::String tableCode = morseTable[i].code;
                
                // Use string comparison instead of pointer comparison
                if (letter.compare(tableCode) == 0) {
                    result += morseTable[i].character;
                    found = true;
                    DBG("Matched to character: '" + juce::String(morseTable[i].character) + "'");
                    break;
                }
            }
            
            // If not found, add a placeholder
            if (!found) {
                result += "?";
                DBG("No match found for: \"" + letter + "\"");
            }
        }
        
        // Add a space between words
        result += " ";
    }
    
    DBG("Final converted text: \"" + result.trim() + "\"");
    return result.trim();
}

DashDotConductorEditor::DashDotConductorEditor(DashDotConductorProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    // Set up text input
    addAndMakeVisible(titleLabel);
    titleLabel.setText("Dash Dot Conductor", juce::dontSendNotification);
    
    // Create a standard font with a larger size (avoids deprecation warning)
    titleLabel.setFont(juce::Font(18.0f));
    
    // Define a constant for the size of the morse table
    static const int MORSE_TABLE_SIZE = 37; // Number of entries in morseTable
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
    
    // Parameter sliders with terminal styling
    addAndMakeVisible(dotNoteLabel);
    dotNoteLabel.setText("Dot Note (MIDI)", juce::dontSendNotification);
    dotNoteLabel.setColour(juce::Label::textColourId, juce::Colours::green);
    
    addAndMakeVisible(dotNoteSlider);
    dotNoteSlider.setRange(0, 127, 1);
    dotNoteSlider.setValue(processor.dotNoteNumber->get());
    dotNoteSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    // Terminal styling
    dotNoteSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgreen);
    dotNoteSlider.setColour(juce::Slider::trackColourId, juce::Colours::green);
    dotNoteSlider.setColour(juce::Slider::thumbColourId, juce::Colours::lightgreen);
    dotNoteSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::green);
    dotNoteSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    dotNoteSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::darkgreen);
    
    addAndMakeVisible(dashNoteLabel);
    dashNoteLabel.setText("Dash Note (MIDI)", juce::dontSendNotification);
    dashNoteLabel.setColour(juce::Label::textColourId, juce::Colours::green);
    
    addAndMakeVisible(dashNoteSlider);
    dashNoteSlider.setRange(0, 127, 1);
    dashNoteSlider.setValue(processor.dashNoteNumber->get());
    dashNoteSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    // Terminal styling
    dashNoteSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgreen);
    dashNoteSlider.setColour(juce::Slider::trackColourId, juce::Colours::green);
    dashNoteSlider.setColour(juce::Slider::thumbColourId, juce::Colours::lightgreen);
    dashNoteSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::green);
    dashNoteSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    dashNoteSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::darkgreen);
    
    addAndMakeVisible(dotDurationLabel);
    dotDurationLabel.setText("Dot Duration (beats)", juce::dontSendNotification);
    dotDurationLabel.setColour(juce::Label::textColourId, juce::Colours::green);
    
    addAndMakeVisible(dotDurationSlider);
    dotDurationSlider.setRange(0.1, 1.0, 0.05);
    dotDurationSlider.setValue(processor.dotDuration->get());
    dotDurationSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    // Terminal styling
    dotDurationSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgreen);
    dotDurationSlider.setColour(juce::Slider::trackColourId, juce::Colours::green);
    dotDurationSlider.setColour(juce::Slider::thumbColourId, juce::Colours::lightgreen);
    dotDurationSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::green);
    dotDurationSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    dotDurationSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::darkgreen);
    
    addAndMakeVisible(dashDurationLabel);
    dashDurationLabel.setText("Dash Duration (beats)", juce::dontSendNotification);
    dashDurationLabel.setColour(juce::Label::textColourId, juce::Colours::green);
    
    addAndMakeVisible(dashDurationSlider);
    dashDurationSlider.setRange(0.1, 2.0, 0.05);
    dashDurationSlider.setValue(processor.dashDuration->get());
    dashDurationSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    // Terminal styling
    dashDurationSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgreen);
    dashDurationSlider.setColour(juce::Slider::trackColourId, juce::Colours::green);
    dashDurationSlider.setColour(juce::Slider::thumbColourId, juce::Colours::lightgreen);
    dashDurationSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::green);
    dashDurationSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    dashDurationSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::darkgreen);
    
    addAndMakeVisible(dotVelocityLabel);
    dotVelocityLabel.setText("Dot Velocity (1-127)", juce::dontSendNotification);
    dotVelocityLabel.setColour(juce::Label::textColourId, juce::Colours::green);
    
    addAndMakeVisible(dotVelocitySlider);
    dotVelocitySlider.setRange(1, 127, 1);
    dotVelocitySlider.setValue(processor.dotVelocity->get());
    dotVelocitySlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    // Terminal styling
    dotVelocitySlider.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgreen);
    dotVelocitySlider.setColour(juce::Slider::trackColourId, juce::Colours::green);
    dotVelocitySlider.setColour(juce::Slider::thumbColourId, juce::Colours::lightgreen);
    dotVelocitySlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::green);
    dotVelocitySlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    dotVelocitySlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::darkgreen);
    
    addAndMakeVisible(dashVelocityLabel);
    dashVelocityLabel.setText("Dash Velocity (1-127)", juce::dontSendNotification);
    dashVelocityLabel.setColour(juce::Label::textColourId, juce::Colours::green);
    
    addAndMakeVisible(dashVelocitySlider);
    dashVelocitySlider.setRange(1, 127, 1);
    dashVelocitySlider.setValue(processor.dashVelocity->get());
    dashVelocitySlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    // Terminal styling
    dashVelocitySlider.setColour(juce::Slider::backgroundColourId, juce::Colours::darkgreen);
    dashVelocitySlider.setColour(juce::Slider::trackColourId, juce::Colours::green);
    dashVelocitySlider.setColour(juce::Slider::thumbColourId, juce::Colours::lightgreen);
    dashVelocitySlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::green);
    dashVelocitySlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    dashVelocitySlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::darkgreen);
    
    // Connect the sliders to real-time value change handlers
    dotNoteSlider.onValueChange = [this] {
        *processor.dotNoteNumber = (int)dotNoteSlider.getValue();
    };
    
    dashNoteSlider.onValueChange = [this] {
        *processor.dashNoteNumber = (int)dashNoteSlider.getValue();
    };
    
    dotDurationSlider.onValueChange = [this] {
        *processor.dotDuration = (float)dotDurationSlider.getValue();
    };
    
    dashDurationSlider.onValueChange = [this] {
        *processor.dashDuration = (float)dashDurationSlider.getValue();
    };
    
    dotVelocitySlider.onValueChange = [this] {
        *processor.dotVelocity = (int)dotVelocitySlider.getValue();
    };
    
    dashVelocitySlider.onValueChange = [this] {
        *processor.dashVelocity = (int)dashVelocitySlider.getValue();
    };
    
    // Window size
    setSize(500, 580);
    
    // Send initial Morse code to processor
    processor.setMorseInput(convertTextToMorse(textInput.getText()));
    
    // Start timer to check for MIDI input (check every 100ms)
    startTimer(100);
    
    // Send initial Morse code to processor
    processor.setMorseInput(convertTextToMorse(textInput.getText()));
}

DashDotConductorEditor::~DashDotConductorEditor() {
    // Stop the timer when editor is destroyed
    stopTimer();
}

void DashDotConductorEditor::paint(juce::Graphics& g) {
    // Terminal-like black background
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::green);
    
    // Add a terminal-style header border
    g.drawRect(getLocalBounds(), 1);
    
    // Add a terminal cursor effect at the bottom if desired
    int cursorY = getHeight() - 15;
    g.fillRect(10, cursorY, 8, 2);
}

// Add timer callback implementation
void DashDotConductorEditor::timerCallback()
{
    // Check if new MIDI has been received
    if (processor.getMidiChanged())
    {
        // Convert the MIDI notes to Morse code
        juce::String morseCode = processor.convertMIDIToMorse(juce::MidiBuffer());
        
        // Update the interface
        if (morseCode.isNotEmpty()) {
            morseOutput.setText(morseCode, juce::dontSendNotification);
            
            // Convert Morse to text
            juce::String decodedText = processor.convertMorseToText(morseCode);
            decodedTextOutput.setText(decodedText, juce::dontSendNotification);
            textInput.setText(decodedText, juce::dontSendNotification);
        }
        
        // Reset the flag
        processor.resetMidiChanged();
    }
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
        // Text to Morse conversion
        juce::String morseCode = convertTextToMorse(editor.getText());
        morseOutput.setText(morseCode, juce::dontSendNotification);
        processor.setMorseInput(morseCode);
    }
    else if (&editor == &morseOutput) {
        // Morse to Text conversion
        morseInputChanged();
    }
}

void DashDotConductorEditor::buttonClicked(juce::Button* button) {
    if (button == &generateButton) {
        // Update parameter values from sliders before generating
        *processor.dotNoteNumber = (int)dotNoteSlider.getValue();
        *processor.dashNoteNumber = (int)dashNoteSlider.getValue();
        *processor.dotDuration = (float)dotDurationSlider.getValue();
        *processor.dashDuration = (float)dashDurationSlider.getValue();
        *processor.dotVelocity = (int)dotVelocitySlider.getValue();
        *processor.dashVelocity = (int)dashVelocitySlider.getValue();
        
        // Generate MIDI notes
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
    // Define this here too for safety
    static const int MORSE_TABLE_SIZE = 37;
    
    juce::String result;
    juce::String upperText = text.toUpperCase();
    
    // Debug output
    DBG("Converting text to morse: " + upperText);
    
    for (int i = 0; i < upperText.length(); ++i) {
        char c = upperText[i];
        bool found = false;
        
        // Handle space character specially
        if (c == ' ') {
            result += "/";
            found = true;
            DBG("Converted space to /");
        } else {
            // Find the character in the morse table
            for (int j = 0; j < MORSE_TABLE_SIZE; ++j) {
                if (morseTable[j].character == c) {
                    result += morseTable[j].code;
                    DBG("Converted character: " + juce::String(c) + " to morse: " + juce::String(morseTable[j].code));
                    found = true;
                    break;
                }
            }
        }
        
        // If character is not found in the table, ignore it
        if (!found) {
            DBG("No morse equivalent found for: " + juce::String(c));
            continue;
        }
        
        // Add space between letters (but not after a space)
        if (i < upperText.length() - 1 && c != ' ') {
            result += " ";
        }
    }
    
    DBG("Final morse code: " + result);
    return result;
}

// Create the audio processor
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new DashDotConductorProcessor();
}
