## TypeBattle

## Description

This project uses the **SFML 2.5.1** library to create a graphical application in C++. Additionally, it utilizes an **ODBC database**, so the appropriate development environment configuration is required.

---

## Requirements

- Visual Studio 2017 or newer  
- SFML 2.5.1 (must be downloaded manually)  
- Microsoft Access Database Engine 2016 Redistributable (64-bit version)  
- Configured ODBC data source (DSN)

---

## Project Configuration

### 1. Download and Install SFML

Download **SFML 2.5.1** from the official website:  
[https://www.sfml-dev.org/download.php](https://www.sfml-dev.org/download.php)  
Extract the archive and place it in a convenient location on your drive.

### 2. Install Microsoft Access Database Engine

To enable ODBC connections to `.accdb` (MS Access) files, download and install:

[AccessDatabaseEngine_X64.exe (Microsoft)](https://www.microsoft.com/en-us/download/details.aspx?id=54920)

> **Note:** Make sure to use the 64-bit version that matches your compiler architecture.

### Library Linking

- **Project → Properties → Linker → Input**:  
  - Add the required SFML libraries (e.g., `sfml-graphics.lib`, `sfml-window.lib`, `sfml-system.lib`, etc.)  
  - Also add the following library:
    ```
    odbc32.lib
    ```

### Preprocessor Definitions

- **Project → Properties → Configuration Properties → C/C++ → Preprocessor → Preprocessor Definitions**:  
  - Remove:
    ```
    UNICODE
    _UNICODE
    ```

---

## Running the Application

After configuring the environment and building the project, run the application from within Visual Studio. Make sure the SFML DLL files are located in the same directory as the `.exe` file, or add them to the `PATH` environment variable.

---