# Chip 8 Emulator made in C and SDL3

<video src="https://github.com/user-attachments/assets/6a3bf920-9a16-40b7-beb8-e2b8fac7ecab" width="100%" autoplay loop muted playsinline></video>

## Requirements

* SDL3 development libraries
* Standard C development toolchain

## Building

### Normal Build

```bash
make
```

### Debug Build(really unusable unless debugging)

```bash
make debug
```

## Usage

```bash
./chip8 [options] <rom file>
```

### Options

| Long Flag | Short Flag | Description |
| :--- | :---: | :--- |
| `--cycles <cycles>` | `-c` | Set the cycles per frame |
| `--disp-wait` | `-d` | Enable the display wait quirk |
| `--file <path>` | `-f` | Set the chip 8 rom file path |
| `--mute` | `-m` | Mutes the buzzer |
| `--shift` | `-s` | Enable the shift quirk |
| `--vf-reset` | `-r` | Enable the Vf reset quirk |
| `--wrap` | `-w` | Enable the wrapping quirk |

### Keymap

```text
Original Hex Keypad      Modern QWERTY Map
+---+---+---+---+        +---+---+---+---+
| 1 | 2 | 3 | C |        | 1 | 2 | 3 | 4 |
+---+---+---+---+        +---+---+---+---+
| 4 | 5 | 6 | D |   -->  | Q | W | E | R |
+---+---+---+---+        +---+---+---+---+
| 7 | 8 | 9 | E |        | A | S | D | F |
+---+---+---+---+        +---+---+---+---+
| A | 0 | B | F |        | Z | X | C | V |
+---+---+---+---+        +---+---+---+---+
```
