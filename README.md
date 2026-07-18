# myimgui - Dear ImGui CAD Viewer & Playground

A C++ playground for learning and testing **Dear ImGui**, **ImPlot**, and **ImGuiFileDialog**, featuring a custom STL (Stereolithography) CAD parser.

---

## Project Structure

```
├── src/                      # Source files
│   ├── main.cpp              # ImGui GUI viewer & entry point
│   ├── cad_model.cpp         # STL CAD file parser implementation
│   └── cad_model.h           # CAD parser API definitions
├── tests/                    # Unit Tests
│   └── test.cpp              # Test cases using Doctest
├── external/                 # External Git submodules (cloned automatically)
│   ├── imgui/                # Dear ImGui core & backends
│   ├── implot/               # Real-time data plotting library
│   ├── ImGuiFileDialog/      # File selection dialog
│   └── doctest/              # Fast unit testing header library
├── build.bat                 # Windows MSVC Build script
├── Makefile                  # Cross-platform Makefile (Linux / MinGW)
├── myimgui.sln               # Visual Studio Solution File
└── myimgui.vcxproj           # Visual Studio Project File
```

---

## Dependencies & Setup

Clone the repository with submodules recursively:
```bash
git clone --recursive <repository-url>
```
If you already cloned it without submodules, initialize them manually:
```bash
git submodule update --init --recursive
```

---

## Building and Running

### 🐧 Linux
Install the system prerequisites (OpenGL development files and GLFW):
* **Ubuntu/Debian**:
  ```bash
  sudo apt-get install build-essential pkg-config libglfw3-dev libgl-dev
  ```
* **Fedora**:
  ```bash
  sudo dnf groupinstall "Development Tools"
  sudo dnf install glfw-devel mesa-libGL-devel pkgconf-pkg-config
  ```

#### Commands:
* **Build Application (Release - Optimized)**:
  ```bash
  make
  ./myimgui
  ```
* **Build Application (Debug - Unoptimized)**:
  ```bash
  make DEBUG=1
  ./myimgui
  ```
* **Build and Run Unit Tests**:
  ```bash
  make test          # Release configuration
  make test DEBUG=1  # Debug configuration
  ```
* **Clean Built Files**:
  ```bash
  make clean
  ```

---

### 🪟 Windows (MSVC Compiler)

#### Option 1: Double-click Solution
Simply open **`myimgui.sln`** in **Visual Studio 2022**, select your desired configuration (`Debug`/`Release`) and platform (`Win32`/`x64`), and press **F5** to build and run.

#### Option 2: Command Line Batch Script (`build.bat`)
Run the script from a normal Command Prompt or Visual Studio Developer Command Prompt:
* **Build Debug Version** (Default, includes PDB symbols):
  ```cmd
  build.bat
  ```
* **Build Release Version** (Fully optimized `/O2`, defines `NDEBUG`):
  ```cmd
  build.bat release
  ```
* **Show Help Info**:
  ```cmd
  build.bat help
  ```

---

## Features Include
* **STL Parser**: Safe, tokenized parsing of ASCII Stereolithography (STL) CAD files.
* **Modern Typography**: High-definition, anti-aliased text using the native `DroidSans.ttf` vector font (loaded at 22px).
* **Doctest Integration**: Clean unit testing layout matching standard parser behavior.
* **Filters Organized**: Visual Studio project configurations cleanly organize custom code under `sources` and library code under `imgui/backends`, `implot`, and `ImGuiFileDialog` respectively.
