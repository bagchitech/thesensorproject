# The Sensor Project

![Language](https://img.shields.io/badge/language-C11-blue.svg)
![Build](https://img.shields.io/badge/build-CMake-064F8C.svg)
![Status](https://img.shields.io/badge/status-learning%20project-brightgreen.svg)

A sensor **simulation and monitoring engine** written in C. It loads a set of virtual sensors from a config file, ticks them forward in a loop to generate readings, tracks each sensor's health with a state machine, and keeps a rolling history of readings in a ring buffer.

Built as a hands-on refresher on Embedded C fundamentals - data modelling, pointers, manual memory management, file parsing, and finite state machine.
---

## What It Does

- Reads sensor definitions from a plain-text **config file** at runtime
- Simulates three sensor types - **Temperature**, **Humidity**, and **Pressure**
- Advances all sensors each **tick**, generating new readings
- Injects configurable **fault behaviours** to exercise the health logic
- Runs each sensor through a **state machine** (`ACTIVE, FAULTY`, plus `PASSIVE`)
- Stores a rolling **history** of readings per sensor in a ring buffer
- Configurable via **command-line flags**; exits cleanly on `Ctrl+C`

## Concepts Demonstrated

| Concept | Where it's used |
| --- | --- |
| `enum` | `SensorType`, `Status`, and `FaultMode` - fixed, readable sets of states |
| `struct` | `Sensor` stores ID, name, config, history, status, and timestamps into one record |
| `union` (tagged) | `DataConfig` shares memory across per-type config (temperature/humidity/pressure), with `SensorType` as the tag |
| Pointers | Pervasive - sensors passed by pointer, pointer-based parsing, dynamic arrays |
| Dynamic memory | `malloc`/`free` for the sensor array and each sensor's ring buffer, with clear ownership |
| File I/O & parsing | `fopen`/`fgets` plus a hand-written tokenizer that parses the config format |
| Ring buffer | Custom circular buffer that overwrites the oldest reading when full |
| State machine | Sensor lifecycle driven by consecutive good/bad reads and staleness thresholds |
| CLI & signals | Argument parsing and a `SIGINT` handler for graceful shutdown |

## How It Works

```
 sensors.conf --> load_sensor_config()  -->  Sensor[] (heap)
 -- main tick loop
 -- tick_sensor(s) 
 -- generate reading (per fault mode)
 -- set_reading , update timestamp / sequence no.
 -- is_reading_valid   bounds-check for the sensor type
 -- update_sensor_state
 -- append to rolling history
 -- print_all_sensors()   (unless --quiet)
```

Each tick, every sensor generates a new value according to its **fault mode**:

| Fault mode | Behaviour |
| --- | --- |
| `NORMAL` | Small random walk around the current reading |
| `STUCK` | Reading never changes (goes stale) |
| `DRIFTING` | Reading creeps steadily in one direction |
| `NOISY` | Wild, out-of-range values |
| `DEAD` | No updates at all - sensor goes silent |

The **state machine** then reacts: an `ACTIVE` sensor that logs too many consecutive invalid reads (or goes stale while `STUCK`) transitions to `FAULTY`; a `FAULTY` sensor that recovers enough consecutive valid reads returns to `ACTIVE`.

## Configuration Format

Sensors are declared one per line in `sensors.conf`:

```
sensor <id> "<name>" <TYPE> [history_capacity] [key=value ...]
```

- **`<TYPE>`** is one of `TEMPERATURE`, `HUMIDITY`, or `PRESSURE`
- **`history_capacity`** - optional ring-buffer size (defaults to 100)
- **`key=value`** options, by type:
  - Temperature: `min=`, `max=` (valid range)
  - Humidity: `calibration=`
  - Pressure: `altitude=`
  - Any type: `fault=` (`NORMAL` | `STUCK` | `DRIFTING` | `NOISY` | `DEAD`)

Lines beginning with `#` are treated as comments.

**Example:**

```
sensor 1 "Living Room" TEMPERATURE 10 min=-10 max=50
sensor 2 "Basement"    HUMIDITY    calibration=1.02
sensor 3 "Kitchen"     TEMPERATURE 3  min=-10 max=50 fault=DRIFTING
```

## Getting Started

### Prerequisites

- A C11 compiler (GCC or Clang)
- CMake 3.10+
- **Note:** the tick loop currently uses `Sleep()` from `<windows.h>`, so the project builds on **Windows** as-is. See [Portability](#portability) to run it on Linux/macOS.

### Build 

```bash
gcc -std=c11 -Wall -Wextra -Iinclude src/*.c -o thesensorproject -lm
```

### Run

```bash
./thesensorproject --config sensors.conf --tick-rate 1000
```

### Command-Line Options

| Flag | Description | Default |
| --- | --- | --- |
| `--config <path>` | Path to the sensor config file | `sensors.conf` |
| `--duration <seconds>` | How long to run; `0` runs until `Ctrl+C` | `0` |
| `--tick-rate <ms>` | Delay between ticks in milliseconds | `1000` |
| `--quiet` | Suppress per-tick printing | off |
| `--help` | Print usage and exit | - |

## Sample Output

```
SEQ No: 5 ID: 1 Name: Living Room SensorType: 0 Status: 0 Reading: 24.581999 COUNT: 5 FAULT: NORMAL
SEQ No: 5 ID: 2 Name: Basement    SensorType: 1 Status: 0 Reading: 57.930000 COUNT: 5 FAULT: NORMAL
SEQ No: 5 ID: 3 Name: Kitchen     SensorType: 0 Status: 2 Reading: 61.200001 COUNT: 5 FAULT: DRIFTING
```

## Portability

The only platform-specific dependency is the inter-tick delay (`Sleep()` from `<windows.h>`). To build on Linux/macOS, swap it for a POSIX equivalent - e.g. `usleep(tick_rate_ms * 1000)` from `<unistd.h>` - and remove the `<windows.h>` include.


