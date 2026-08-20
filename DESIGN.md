## Config file format

Sensors are loaded from a plain-text file at startup. The default path is
`sensors.conf` in the current directory; a different file can be specified
with `--config <path>`.

### Line syntax

Each non-blank, non-comment line defines one sensor:

    sensor <id> <name> <TYPE> [key=value ...]

Where:
- `sensor` is a literal keyword.
- `<id>` is a positive integer, unique within the file.
- `<name>` is a string. If it contains spaces, wrap it in double quotes.
- `<TYPE>` is one of `TEMPERATURE`, `HUMIDITY`, `PRESSURE`.
- `<key>=<value>` pairs configure the sensor. Available keys depend on type.

### Required and optional keys

Every sensor requires `id`, `name`, and `<TYPE>` (the positional fields).

Per-type required keys:

| Type | Required keys |
|------|---------------|
| TEMPERATURE | `min`, `max` (integers) |
| HUMIDITY | `calibration` (float) |
| PRESSURE | `altitude` (float) |

Optional keys for any sensor:

| Key | Values | Default |
|-----|--------|---------|
| `fault` | `NORMAL`, `STUCK`, `DRIFTING`, `NOISY`, `DEAD` | `NORMAL` |
| `status` | `ACTIVE`, `PASSIVE` | `ACTIVE` |

### Comments and blank lines

Lines starting with `#` (after any leading whitespace) are ignored. Blank
lines are ignored. Comments must occupy their own line - mid-line comments
after a sensor definition are not supported.

### Quoting

Names with spaces must be enclosed in double quotes:

    sensor 1 "Living Room" TEMPERATURE min=-10 max=50

Names without spaces may be quoted or not:

    sensor 2 Kitchen HUMIDITY calibration=1.0
    sensor 2 "Kitchen" HUMIDITY calibration=1.0

Double quotes inside a name are not supported (no escape sequences).

### Error reporting

Parse errors are reported in the standard `path:line:column: message`
format. Examples:

    sensors.conf:5:32: expected 'max=' after 'min=-10', got 'flart'
    sensors.conf:8:1: unknown sensor type 'TEMPRATURE'
    sensors.conf:12:15: missing required key 'calibration' for HUMIDITY sensor

The parser aborts on the first error and does not attempt recovery.
Fixing the reported error and re-running is the intended workflow.

### Grammar (informal)

    file          = { line , newline }
    line          = comment | blank | sensor_def
    comment       = "#" , { any_char_except_newline }
    blank         = { space | tab }
    sensor_def    = "sensor" , integer , name , type , { kv_pair }
    name          = quoted_string | unquoted_word
    quoted_string = '"' , { any_char_except_quote } , '"'
    type          = "TEMPERATURE" | "HUMIDITY" | "PRESSURE"
    kv_pair       = key , "=" , value