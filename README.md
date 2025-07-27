# PPM Image Processor



## Overview

The PPM Image Processor is a command-line tool for performing various image processing operations on PPM (Portable Pixmap) format images. It supports operations like color inversion, grayscale conversion, flipping, rotation, and sepia filtering. This tool is implemented in C and features automatic directory creation for output files, robust error handling, and an interactive menu system.

## Features

- **Multiple Image Operations**:
  - Color Inversion
  - Grayscale Conversion
  - Horizontal Flip
  - Vertical Flip
  - 90° Clockwise Rotation
  - Sepia Filter
  
- **User-Friendly Interface**:
  - Interactive menu system
  - Automatic directory creation for output files
  - Real-time feedback
  
- **Cross-Platform Support**:
  - Works on Windows, Linux, and macOS
  - Automatic handling of path differences
  
- **Integrated Preview**:
  - Works with VS Code PPM viewer extensions
  - Auto-refresh of preview images

## Requirements

1. **Compiler**:
   - GCC (MinGW for Windows)
   - Clang (macOS/Linux)

2. **VS Code Extensions** (Recommended):
   - C/C++ by Microsoft
   - Code Runner
   - PPM/PGM/PBM Viewer or Image Preview

## Installation

### Windows
```powershell
# Install Chocolatey (if not installed)
Set-ExecutionPolicy Bypass -Scope Process -Force; 
[System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; 
iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))

# Install required tools
choco install mingw make -y

# Add to PATH
$env:Path += ";C:\ProgramData\chocolatey\lib\mingw\tools\install\mingw64\bin;C:\ProgramData\chocolatey\bin"
```

### Linux (Debian/Ubuntu)
```bash
sudo apt update
sudo apt install build-essential make -y
```

### macOS
```bash
brew install make
xcode-select --install
```

## Compilation

### Using Makefile
```bash
make
```

### Manual Compilation
```bash
gcc -g -Wall main.c ppm.c -o ppm_processor
```

## Usage

### Basic Workflow
1. Run the program:
   ```bash
   ./ppm_processor
   ```
2. Enter input image path when prompted
3. Enter output image path
4. Choose an operation from the menu
5. Preview the result in VS Code

### Interactive Menu
```
PPM Image Processor
1. Invert Colors
2. Convert to Grayscale
3. Flip Horizontally
4. Flip Vertically
5. Rotate 90° Clockwise
6. Apply Sepia Filter
7. Exit
Enter your choice: 
```

### Command Line Options
```bash
# Process without interactive menu
./ppm_processor input.ppm output.ppm operation

# Supported operations:
#   invert, grayscale, hflip, vflip, rotate90, sepia
```

### Creating Test Images
Create a test PPM file named `test.ppm`:
```ppm
P6
3 2
255
255 0 0   0 255 0   0 0 255
255 255 0 255 255 255 0 0 0
```

## File Structure

```
ppm_processor/
├── .vscode/               # VS Code configuration
│   ├── tasks.json
│   ├── launch.json
│   └── settings.json
├── src/                   # Source code
│   ├── main.c
│   ├── ppm.c
│   └── ppm.h
├── Makefile               # Build automation
├── test_images/           # Sample images
│   ├── input/
│   └── output/
├── README.md              # This file
└── demo.gif               # Example animation
```

## Image Processing Algorithms

### Color Inversion
```c
r = maxval - r
g = maxval - g
b = maxval - b
```

### Grayscale Conversion
```c
gray = 0.299*r + 0.587*g + 0.114*b
```

### Sepia Filter
```c
new_r = min(255, 0.393*r + 0.769*g + 0.189*b)
new_g = min(255, 0.349*r + 0.686*g + 0.168*b)
new_b = min(255, 0.272*r + 0.534*g + 0.131*b)
```

### Rotation
```c
rotated->pixels[j][cols-1-i] = original->pixels[i][j]
```

## VS Code Integration

### Recommended Settings (.vscode/settings.json)
```json
{
    "ppm-pgm-pbm-viewer.autoRefresh": true,
    "files.autoSave": "afterDelay",
    "files.autoSaveDelay": 500,
    "C_Cpp.default.configurationProvider": "ms-vscode.makefile-tools"
}
```

### Debug Configuration (.vscode/launch.json)
```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug PPM Processor",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/ppm_processor",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],
            "preLaunchTask": "make"
        }
    ]
}
```

## Troubleshooting

### Common Issues
1. **Make not found**:
   - Windows: Ensure Chocolatey install path is in system PATH
   - Linux/macOS: Verify build-essential/xcode-tools are installed

2. **Image preview not working**:
   - Ensure file has .ppm extension
   - Verify header starts with "P6"
   - Install PPM viewer extension

3. **Permission denied errors**:
   ```powershell
   # Windows
   Set-ExecutionPolicy RemoteSigned -Scope CurrentUser
   ```

### Debugging Tips
1. Check PPM header format:
   ```bash
   head -n 3 your_image.ppm
   ```
2. Verify image dimensions:
   ```c
   printf("Image loaded: %ux%u\n", image->cols, image->rows);
   ```
3. Test with small images first (e.g., 3x2 test pattern)

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a pull request

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

## Support

For questions or issues, please [open an issue](https://github.com/M-user143/Basic_ppm6_image_processing_in_C/) on GitHub.
