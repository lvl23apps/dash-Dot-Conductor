# dash-Dot-Conductor
Dash Dot Conductor - A Bidirectional Morse Code MIDI Converter
Created by Scotland (LVL23)
 - April 2025

⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⣀⢀⠀⠀⠀⢀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⣴⠏⠁⠹⠿⣧⠀⣰⠋⠁⠉⠳⡄⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⡏⠀⠀⠀⡇⢻⢠⣯⠀⠀⠈⢂⢻⡐⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⢀⠀⠀⢰⡗⠀⠀⣼⣁⣾⢸⡇⠀⠀⢀⣼⢨⡇⢀⣾⡴⠿⣯⠀⠀
⠀⣴⠿⢛⡿⣦⡘⣧⡀⠀⡼⣳⡟⢸⣇⠀⢠⣈⠍⣹⣿⣾⠙⠀⢢⠹⣧⠀
⠀⣿⠀⠀⠸⡮⢻⣎⠻⢷⡿⠋⠀⠀⠙⠳⢾⣿⠞⠁⣿⠁⠀⠀⠸⠃⣿⠀
⢠⣿⡄⠀⠀⢸⠸⣏⠀⢀⣴⣾⡿⣿⣦⣤⣦⣤⣄⠀⢹⡆⠀⠀⠀⡆⣿⠄
⠀⢿⣏⠁⠀⢸⣣⣿⢀⣿⠋⠀⠀⠀⠉⠉⠉⠉⠻⣷⠀⢿⡄⢀⡞⣠⡟⠀
⠀⠐⢿⣯⣀⡾⠛⢃⣼⡏⠀⠀⠀⠀⠀⠈⡆⠀⠀⢿⣧⠸⠿⣿⠾⠋⠀⠀
⠀⠀⠀⠈⠉⢣⣶⡿⠛⠀⠀⠀⠀⠀⠀⠀⠉⢶⣤⡈⢻⣧⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⡽⢿⠁⢰⣿⡇⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⢻⣿⣶⣦⣄⡀⠀⠀⠀⠰⣿⣏⣤⣞⣀⣼⣿⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠈⠉⠉⠙⢿⣦⣄⣤⣴⡾⠛⠉⠟⠉⠉⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠛⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
OwO What's this? A Morse code converter? *nuzzles your MIDI notes

A bidirectional Morse code to MIDI converter plugin for Logic Pro and other AU-compatible DAWs.

# Dash Dot Conductor

![Dash Dot Conductor Logo](docs/logo.png)

A bidirectional Morse code to MIDI converter plugin for Logic Pro and other AU-compatible DAWs.

## Features

- **Text to Morse to MIDI**: Type any text and convert it to Morse code patterns, then into MIDI notes
- **MIDI to Morse to Text**: Play or record MIDI, then convert it back to Morse code and human-readable text
- **Fully Customizable**: Configure note values, durations, velocities, and timing gaps
- **Real-time Processing**: Works in real-time during recording or playback
- **Easy Integration**: Works as an Audio Unit MIDI effect in Logic Pro, GarageBand, or any AU-compatible host

## Screenshot

![Dash Dot Conductor Interface](docs/screenshot.png)

## Installation

### Pre-compiled Binary (Recommended)

1. Download the latest release from the [Releases](https://github.com/yourusername/dash-dot-conductor/releases) page
2. Copy the `DashDotConductor.component` file to:
   - `/Library/Audio/Plug-Ins/Components/` (for all users)
   - or `~/Library/Audio/Plug-Ins/Components/` (for current user only)
3. Restart your DAW or rescan AU plugins

### Building from Source

See the [Build Instructions](docs/BUILD.md) document for detailed steps on building from source.

## Usage

1. Add the Dash Dot Conductor as a MIDI effect to an instrument track in your DAW
2. Enter text in the "Input Text" field to generate Morse code
3. Click "Generate MIDI" to create a MIDI pattern from the Morse code
4. Or, play MIDI notes and click "Decode MIDI to Morse/Text" to convert back to text

For detailed usage instructions, see the [User Guide](docs/USER_GUIDE.md).

## Musical Applications

- Create rhythmic patterns from famous quotes or messages
- Use names or words as the basis for musical motifs
- Encode secret messages in your music
- Educational tool for learning Morse code
- Generate unique rhythmic sequences from text

## System Requirements

- macOS 10.13 or later
- Logic Pro X, GarageBand, or any AU-compatible host
- Intel or Apple Silicon processor

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- [JUCE Framework](https://juce.com/) - The backbone of this audio plugin
- [International Morse Code standard](https://en.wikipedia.org/wiki/Morse_code) - For the character mappings

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## Contact

Project Link: [https://github.com/yourusername/dash-dot-conductor](https://github.com/yourusername/dash-dot-conductor)
