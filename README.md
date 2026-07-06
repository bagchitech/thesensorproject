# The Sensor Project

A small practice project in **C** built to revise and demonstrate core C data structures - `enum`, `struct`, and `union` - through a simple, practical use case: reading and displaying data from multiple sensor types.

## Overview

The project simulates three types of sensors:

-  **Temperature**
-  **Humidity**
-  **Pressure**

Each sensor produces a reading, and the program processes and displays these readings using a memory-efficient design built around C's `struct`, `union`, and `enum` constructs.

## Goals

This project is about a hands-on refresher on:

- How `enum` can be used to represent a fixed set of sensor types cleanly and readably.
- How `struct` can group related fields (sensor ID, type, reading value, timestamp, etc.) into a single logical unit.
- How `union` can be used to store different reading types (e.g. `float` for temperature/humidity, `int` for pressure) in the same memory space - since only one sensor type is relevant at a time - to reduce memory footprint.
- How combining these constructs (a **tagged union**, using an `enum` to track which `union` member is active) leads to safer and more memory-conscious code than using separate variables or over-allocated structs.

## Key Concepts Demonstrated

| Concept | Where it's used |
|---|---|
| `enum` | Represents the sensor type (`TEMPERATURE`, `HUMIDITY`, `PRESSURE`) |
| `struct` | Bundles a sensor's metadata and reading together |
| `union` | Shares memory across different reading value types |
| Tagged union pattern | Combines `enum` + `union` inside a `struct` to safely interpret the correct field |
| Memory optimization | Fixed memory allocation, conscious decision for sequencing the variables in struct to optimize padding |

## Getting Started

### Prerequisites
- A C compiler (GCC/Clang)

### Build & Run
```bash
gcc -o sensor_project main.c
./sensor_project
```

## Why This Project Exists

This is a learning exercise to build intuition for:
- Choosing the right data structure for the job
- Understanding memory layout and alignment in C
- Practicing the tagged-union pattern, a common technique in systems programming for representing "one of several types" safely and compactly
- Practicing git work flow.
