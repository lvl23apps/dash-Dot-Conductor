# Dash Dot Conductor User Guide

## Table of Contents

1. [Installation](#installation)
2. [Interface Overview](#interface-overview)
3. [Text to MIDI Conversion](#text-to-midi-conversion)
4. [MIDI to Text Conversion](#midi-to-text-conversion)
5. [Parameter Adjustments](#parameter-adjustments)
6. [Creative Applications](#creative-applications)
7. [Troubleshooting](#troubleshooting)
8. [Tips & Tricks](#tips--tricks)

## Installation

1. Copy the `DashDotConductor.component` file to:
   - `/Library/Audio/Plug-Ins/Components/` (for all users)
   - or `~/Library/Audio/Plug-Ins/Components/` (for current user only)

2. Launch Logic Pro (or your AU-compatible DAW)

3. If this is your first time using the plugin, you may need to validate it:
   - In Logic Pro, go to Logic Pro > Preferences > Plug-in Manager
   - Find "Dash Dot Conductor" in the list
   - Ensure it's enabled and validate if necessary

## Interface Overview

![Dash Dot Conductor Interface](docs/interface_annotated.png)

The Dash Dot Conductor interface is divided into several sections:

1. **Text Input Area**: Enter your text to be converted to Morse code
2. **Morse Code Display**: Shows the Morse code representation of your text
3. **Decoded Text Display**: Shows text decoded from MIDI/Morse code
4. **Control Buttons**:
   - Generate MIDI: Converts current Morse code to MIDI
   - Decode MIDI to Morse/Text: Analyzes incoming MIDI to determine Morse and text
5. **Parameter Controls**: Customize MIDI note values, durations, and velocities

## Text to MIDI Conversion

### Basic Conversion

1. Type your text in the "Input Text" field
2. The Morse code representation will automatically appear in the "Morse Code" field
3. Click "Generate MIDI" to create MIDI notes corresponding to the Morse pattern

### Recording the Generated MIDI

1. Set up a MIDI track in Logic Pro with your desired instrument
2. Add Dash Dot Conductor as a MIDI effect on the track
3. Arm the track for recording
4. Click the record button in Logic Pro
5. Click "Generate MIDI" in the plugin
6. Stop recording when finished

### Supported Characters

Dash Dot Conductor supports:
- All 26 letters (A-Z, case insensitive)
- Numbers (0-9)
- Spaces between words

## MIDI to Text Conversion

### Live Input Conversion

1. Play MIDI notes into the track with Dash Dot Conductor
2. Click "Decode MIDI to Morse/Text"
3. View the detected Morse code and corresponding text

### Recorded MIDI Conversion

1. Place the playhead where you want to start analysis
2. Play back the recorded MIDI
3. Click "Decode MIDI to Morse/Text" during or after playback
4. View the results in the Morse and Decoded Text fields

### How the Decoder Works

The decoder analyzes:
- Note durations (to distinguish dots from dashes)
- Time gaps between notes (to distinguish between symbols, letters, and words)
- Note values (matching configured dot/dash values)

## Parameter Adjustments

### Dot and Dash Settings

- **Dot Note**: MIDI note number for dots (default: 60/middle C)
- **Dash Note**: MIDI note number for dashes (default: 64/E)
- **Dot Duration**: Length of dot notes in beats (default: 0.25)
- **Dash Duration**: Length of dash notes in beats (default: 0.5)
- **Dot Velocity**: Velocity value for dot notes (1-127, default: 100)
- **Dash Velocity**: Velocity value for dash notes (1-127, default: 100)

### Gap Settings

- **Symbol Gap**: Space between symbols within a letter (default: 0.1 beats)
- **Letter Gap**: Space between letters (default: 0.5 beats)
- **Word Gap**: Space between words (default: 1.0 beats)

## Creative Applications

### Musical Morse Signatures

Create a musical motif from:
- Your name or initials
- A short phrase significant to your project
- Song titles or key lyrics

### Rhythmic Foundations

Use Morse code patterns as:
- Drum patterns
- Rhythm section foundations
- Accent patterns for sustained notes

### Melodic Development

1. Convert text to MIDI using Dash Dot Conductor
2. Use the generated pattern as a seed for melodies
3. Expand and develop the pattern using traditional composition techniques

### Hidden Messages

1. Encode meaningful text as musical passages
2. Incorporate these passages into your compositions
3. Document the hidden messages for listeners to discover

## Troubleshooting

### Plugin Not Appearing

**Solution**: Ensure the plugin is in the correct directory and rescan:
1. Check `/Library/Audio/Plug-Ins/Components/` or `~/Library/Audio/Plug-Ins/Components/`
2. In Logic Pro, go to Logic Pro > Preferences > Plug-in Manager
3. Click "Reset & Rescan Selection"

### No MIDI Output

**Solution**:
1. Check that the plugin is on a valid MIDI or Instrument track
2. Confirm that you've clicked "Generate MIDI" after entering text
3. Verify that your parameters (especially durations) are in a detectable range

### Inaccurate MIDI to Text Conversion

**Solution**:
1. Ensure consistent timing when playing MIDI notes
2. Adjust the dot and dash note parameters to match the incoming MIDI
3. For best results, use distinctive duration differences between dots and dashes

## Tips & Tricks

### Optimizing for Readability

- Use shorter words for clearer Morse code patterns
- Separate phrases with additional spaces for clarity
- Choose notes with sufficient separation (e.g., at least a third apart)

### Musical Enhancements

- Add velocity variation to emphasize specific characters
- Layer multiple Morse patterns at different pitch levels
- Use different instruments for dots versus dashes

### Performance Techniques

- Map "Generate MIDI" to a MIDI controller button
- Create presets with different text-to-MIDI configurations
- Use automation to change parameters during playback

### Integration with Logic Pro

- Save Dash Dot Conductor configurations as channel strip presets
- Create track alternatives with different Morse code patterns
- Use markers to identify different encoded sections
