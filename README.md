# Stock Market Candlestick Chart Generator

A C++ console application that reads historical stock market data from a CSV file and generates ASCII-based Japanese candlestick charts. The generated charts can be previewed directly in the console and saved as text files.

## Description

Japanese candlestick charts display financial stock price movements over specific time intervals. Each candle is constructed using four fundamental data points:
* Open price
* High price
* Low price
* Close price

The candle's body represents the range between the Open and Close prices:
* Bullish candle (`O`): Close price is higher than Open price (stock price increased).
* Bearish candle (`#`): Open price is higher than Close price (stock price decreased).
* Wicks/Shadows (`|`): Define the highest and lowest prices reached during the period.

## Features

* Data Input: Reads standard financial CSV datasets formatted as `Date, Open, High, Low, Close, Volume`.
* Candle Aggregation: Merge daily records into Weekly (5 trading days) or Monthly (20 trading days) timeframe candles.
* Scale Calculation: Automatically scales price values and date ranges according to user-specified chart dimensions.
* Console & File Output: Displays a responsive preview on the terminal and exports the complete formatted ASCII chart to a target `.txt` file.
* Date Range Selection: Custom start and end index configuration to analyze specific historical periods.

## Input File Format

The program expects a CSV input file formatted as follows:

```csv
Date,Open,High,Low,Close,Volume
2023-11-27,43.65,44.545,43.65,44.08,33339937
2023-11-28,43.69,44.08,43.66,44.02,1967329
```

## How to Build and Run

### Prerequisites

* C++ compiler supporting C++11 standard or later (such as GCC, Clang, or MSVC).

### Compilation

Build the executable using GCC (`g++`):

```bash
g++ -std=c++11 -O2 stock_market_2.cpp -o stock_market
```

### Execution

Run the compiled executable:

```bash
# On Linux / macOS
./stock_market

# On Windows
stock_market.exe
```

## Program Flow & Controls

1. Welcome Screen: Press `g` to proceed or `q` to quit.
2. File Configuration: Enter the input file name (e.g., `intc_us_data.csv`) and output file name (e.g., `chart.txt`).
3. Chart Setup:
   * Specify chart height (minimum value: 10 lines).
   * Select candle timeframe (1 = Daily, 2 = Weekly, 3 = Monthly).
   * Specify start and end candle indices for the target range.
4. Navigation Options:
   * Press `z` to adjust chart settings.
   * Press `g` to load a new input dataset.
   * Press `q` to exit the application.

## Project Specifications

* Course: Fundamentals of Computer Programming (Podstawy Programowania)
* Institution: Gdańsk University of Technology (WETI PG)
* Language: C++ (Standard C/C++ libraries only; STL containers and `<string>` excluded according to project guidelines)