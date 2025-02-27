# Real-time Data Monitoring and Filtering System

A C-based real-time data monitoring system featuring data caching, filtering, and visualization capabilities. The system supports real-time data display, data filtering, smooth curve plotting, and data persistence.

## Features

1. **Data Cache Management**
   - Efficient key-value storage
   - Dynamic memory management
   - Automatic garbage collection

2. **Data Filtering System**
   - Customizable filtering rules
   - Maximum 50 historical records
   - Automatic old data removal

3. **Real-time Data Visualization**
   - ASCII art style charts
   - Smooth curve plotting
   - Dynamic coordinate axes
   - Colored terminal output

4. **Data Persistence**
   - CSV format storage
   - Separate storage for raw and filtered data
   - Automatic timestamp recording

## System Requirements

- Linux Operating System
- GCC Compiler
- Make Build Tool

## Directory Structure 
project/
├── include/ # Header files
│ ├── cache.h # Cache module header
│ ├── filter.h # Filter module header
│ └── display.h # Display module header
├── src/ # Source files
│ ├── cache.c # Cache implementation
│ ├── filter.c # Filter implementation
│ ├── display.c # Display implementation
│ └── main.c # Main program
├── Makefile # Build configuration
└── README.md # Documentation


## Build and Run

1. Compile the program:

```bash 
make 
```

2. Run the program: 

```bash 
./bin/program
```

3. Clean build files:

```bash 
make clean
```

## Usage Instructions

1. The program automatically generates and displays random data upon startup
2. Data display includes:
   - Real-time line chart
   - Current value and data range
   - Filtered data list
3. Data is recorded in two CSV files:
   - raw_data.csv: Original data
   - filtered_data.csv: Filtered data
4. Press Ctrl+C to exit

## Data Format

CSV file format:

```csv 
Timestamp,Value,IsFiltered
2024-01-20 10:30:45,42,0
2024-01-20 10:30:46,56,1
```

## Custom Configuration

System parameters can be adjusted by modifying macro definitions in header files:

- `MAX_SERIES_SIZE`: Maximum data series capacity (default 50)
- `DISPLAY_HEIGHT`: Chart display height
- `DISPLAY_WIDTH`: Chart display width
- `DATA_UPDATE_INTERVAL`: Data update interval (seconds)

## Notes

1. Ensure terminal supports ANSI color display
2. Full-screen terminal recommended for optimal display
3. CSV files are automatically created in the program's running directory

## Development Information

- Language: C
- Compiler: GCC
- Build Tool: Make
- Platform: Linux

## License

[Insert license information as appropriate]