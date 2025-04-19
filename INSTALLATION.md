# Installation Guide for Dash Dot Conductor

This guide provides detailed instructions for installing the Dash Dot Conductor plugin on your system.

## System Requirements

- **Operating System**: macOS 10.13 (High Sierra) or later
- **CPU**: Intel or Apple Silicon processor
- **RAM**: 4GB minimum, 8GB or more recommended
- **Disk Space**: 50MB free space
- **Software**: Logic Pro X (10.4 or later), GarageBand, or any AU-compatible DAW

## Pre-built Installation

### Installing the Pre-compiled Plugin

1. **Download the Plugin**:
   - Go to [https://github.com/yourusername/dash-dot-conductor/releases](https://github.com/yourusername/dash-dot-conductor/releases)
   - Download the latest `DashDotConductor.component.zip` file

2. **Extract the Archive**:
   - Double-click the downloaded ZIP file to extract the component

3. **Install the Plugin**:
   Choose ONE of these installation locations:
   
   **For all users on the computer**:
   ```
   sudo cp -R DashDotConductor.component /Library/Audio/Plug-Ins/Components/
   ```
   
   **For current user only**:
   ```
   cp -R DashDotConductor.component ~/Library/Audio/Plug-Ins/Components/
   ```

4. **Set Permissions (if needed)**:
   ```
   sudo chmod -R 755 /Library/Audio/Plug-Ins/Components/DashDotConductor.component
   ```

## Installation from Source

If you'd prefer to build the plugin from source, follow these steps:

### Prerequisites

1. **Install Xcode**:
   - Download and install Xcode from the Mac App Store
   - Open Xcode and accept the license agreement
   - Install the Command Line Tools when prompted

2. **Install JUCE**:
   - Download JUCE from [https://juce.com/get-juce/download](https://juce.com/get-juce/download)
   - Run the installer and follow the on-screen instructions
   - Launch Projucer (JUCE's project manager) once installation is complete

### Building the Plugin

1. **Clone the Repository**:
   ```
   git clone https://github.com/yourusername/dash-dot-conductor.git
   cd dash-dot-conductor
   ```

2. **Open the Project**:
   - Launch Projucer
   - Open `DashDotConductor.jucer` from the repository

3. **Configure the Project**:
   - In Projucer, check that the "Audio Unit" format is enabled
   - Click "Save Project" to generate the Xcode project files

4. **Build the Plugin**:
   - Click "Save and Open in IDE" to open the project in Xcode
   - Select "Release" configuration
   - Build the project (Command+B)

5. **Install the Built Plugin**:
   - In Xcode's Products folder, right-click on DashDotConductor.component
   - Select "Show in Finder"
   - Copy the component file to one of the plugin directories mentioned above

## Validating the Installation

### Logic Pro X

1. Launch Logic Pro X
2. Go to Logic Pro > Preferences > Plug-in Manager
3. Click on "AU MIDI Effects"
4. Find "Dash Dot Conductor" in the list
5. If it's marked as "Failed Validation", select it and click "Reset & Rescan Selection"
6. Once validated, ensure the checkbox next to the plugin is enabled

### GarageBand

1. Launch GarageBand
2. Create a new project
3. Create a software instrument track
4. Click on the Smart Controls button (if not already visible)
5. Click on the "Plug-ins" menu
6. Navigate to "AU MIDI Effects" > "Dash Dot Conductor"

### Other DAWs

The process will vary by DAW, but generally:
1. Ensure your DAW supports Audio Unit (AU) MIDI effect plugins
2. Look for an option to rescan or refresh AU plugins
3. Add a MIDI effect to an instrument track
4. Look for "Dash Dot Conductor" in the AU MIDI Effects category

## Troubleshooting

### Plugin Not Found

If your DAW doesn't show the plugin after installation:

1. **Verify the Installation**:
   ```
   ls -la /Library/Audio/Plug-Ins/Components/DashDotConductor.component
   ```
   or
   ```
   ls -la ~/Library/Audio/Plug-Ins/Components/DashDotConductor.component
   ```

2. **Check for Validation Issues**:
   - In Logic Pro, check the Plug-in Manager for validation status
   - Look for console messages related to AU validation failures

3. **Reset the AU Database**:
   ```
   sudo killall -9 AudioComponentRegistrar
   ```
   Then restart your DAW.

### Validation Errors

If the plugin fails validation:

1. **Check Permissions**:
   ```
   sudo chmod -R 755 /Library/Audio/Plug-Ins/Components/DashDotConductor.component
   ```

2. **Check for System Incompatibility**:
   - Verify your macOS version is supported
   - Check the Logic Pro/DAW version is compatible

3. **Rebuild Audio Units Cache**:
   ```
   sudo killall -9 AudioComponentRegistrar
   rm ~/Library/Caches/AudioUnitCache/com.apple.audiounits.cache
   ```

## Uninstallation

To remove the plugin:

```
rm -rf /Library/Audio/Plug-Ins/Components/DashDotConductor.component
```
or
```
rm -rf ~/Library/Audio/Plug-Ins/Components/DashDotConductor.component
```

Then restart your DAW.

## Support

If you encounter any issues with installation:

1. Check the [GitHub Issues](https://github.com/yourusername/dash-dot-conductor/issues) page for similar problems
2. Create a new issue with detailed information about your system and the problem
3. Contact support at support@yourdomain.com
