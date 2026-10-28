# bacus-security

**Bacus Protector** is a lightweight C++17 security toolkit
designed for defensive security, authorized testing and Purple
Team workflows.

## Features in version 0.3.0

- Lightweight terminal interface (TUI).
- Optional Qt 6 graphical interface (GUI).
- File inspection using 64 KiB read blocks.
- EICAR test-signature detection.
- Basic Linux `/tmp` permission check.
- Inventory of selected security tools in PATH.
- Explicit reporting of incomplete and pending checks.

## Planned modules

- ClamAV integration for antivirus scanning.
- YARA rule evaluation.
- SHA-256 file integrity verification.
- `.bp` package inspection and verification.
- Authorized network inventory.
- TLS certificate inspection.
- System log analysis.
- Incident reports and Purple Team validation.

Planned features are not implemented until the application
performs the checks and reports their results.

## Requirements

- CMake 3.16 or newer.
- A C++17 compiler.
- Qt 6 Widgets only for the GUI build.

## Build the TUI

```bash
cmake -S . -B build \
  -DBACUS_BUILD_TUI=ON \
  -DBACUS_BUILD_GUI=OFF

cmake --build build --parallel 2
./build/bacus-protector-tui
```

## Build the GUI

Qt 6 Widgets must be installed first.

```bash
cmake -S . -B build-gui \
  -DBACUS_BUILD_TUI=OFF \
  -DBACUS_BUILD_GUI=ON

cmake --build build-gui --parallel 2
```

## Commands

```bash
bacus-protector-tui --help
bacus-protector-tui --version
bacus-protector-tui scan FILE
bacus-protector-tui audit
bacus-protector-tui tools
bacus-protector-tui ports
```

## Security notice

This version is an early prototype, not a complete antivirus
or penetration-testing suite.

The file scanner currently checks for the EICAR test signature.
A clean result does not establish that a file is safe.

Tool inventory checks availability in PATH only. They do not
prove that an external tool works or that its databases are current.

The port-audit module is not implemented yet.

Do not use this prototype as the sole security gate for installing
packages. Validate package integrity, scan results and failure
handling before integrating it with Bacus-Pkg.

Use active testing only on systems you own or are authorized to test.

