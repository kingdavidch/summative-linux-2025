# Statistical Functions C Extension for Python

## Overview
This project implements a C extension module for Python that provides statistical operations on arrays of floating-point numbers. The extension is designed to offer improved performance over pure Python implementations while maintaining ease of use through Python integration.

## Files
- `stats.c`: Core C implementation of the statistical functions
- `stats_example.py`: Example Python script demonstrating usage
- `setup.py`: Build script for compiling the extension
- `README.md`: This documentation file

## Requirements
- Python 3.x
- C compiler (e.g., gcc)
- Python development headers (typically included in python-dev or python3-dev packages)
- setuptools package

## Functionality
The extension provides five statistical functions:
1. `sum`: Calculates the sum of all values
2. `average`: Computes the arithmetic mean
3. `stddev`: Calculates the population standard deviation
4. `mode`: Determines the most frequent value
5. `count`: Returns the number of elements

## Implementation Details
### C Extension (stats.c)
- Uses Python C API for integration
- Implements memory management with malloc/free
- Handles edge cases:
  - Empty arrays
  - Insufficient data for standard deviation
- Returns Python-compatible objects (PyFloat, PyLong)
- Includes error handling via Python exceptions

### Key Features
- Efficient array processing in C
- Type conversion between Python lists and C double arrays
- Proper memory management
- Comprehensive error checking

## Build Process
1. Ensure all requirements are installed
2. Navigate to the project directory
3. Run the build command:
```bash
python setup.py build_ext --inplace
```
4. This will generate:
   - A compiled extension file (e.g., `stats.cpython-39-x86_64-linux-gnu.so`)
   - Build temporary files

## Usage Example
```python
import stats

numbers = [1.0, 2.0, 2.0, 3.0, 4.0, 5.0]
print(stats.sum(numbers))      # 17.0
print(stats.average(numbers))  # 2.833...
print(stats.stddev(numbers))   # ~1.329...
print(stats.mode(numbers))     # 2.0
print(stats.count(numbers))    # 6
```

## Running the Example
```bash
python stats_example.py
```

## Error Handling
- Empty array: Raises ValueError
- Standard deviation with < 2 values: Raises ValueError
- Invalid input: Returns NULL with appropriate Python exception

## Performance Considerations
- Single-pass algorithms where possible
- Minimal memory allocation
- Direct C array operations instead of Python object manipulation

## Limitations
- Mode function returns first occurrence if multiple modes exist
- Assumes input can be converted to double precision floats
- Population standard deviation (n-1 denominator)

## Compilation Notes
- Windows users may need Visual Studio Build Tools
- MacOS users need Xcode command line tools
- Linux users need gcc and python-dev packages

## Academic Context
This implementation serves as an example of:
- C/Python integration
- Performance optimization through native code
- Statistical computation implementation
- Memory management in C
- Error handling across language boundaries
