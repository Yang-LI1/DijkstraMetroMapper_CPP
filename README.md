
This is a simple subway station path finding program that can help users calculate the fastest path between two subway stations.

## Dependencies

- C++11 (Visual Studiio Code)
- Linux operating system
- g++ compiler (recommended version: g++ 8.2.0)

## Compilation

To compile the project, run the following command in the terminal(open the terminal in the project root directory):

```bash
g++ -Wall -g -std=c++11 MyStationParser.cpp main.cpp "Grade 2019 g++ 8.2.0 UNIX.o" -o main.exe
```

This will generate an executable file named `main.exe`.

## Execution

To run the program, run the following command in the terminal:

```bash
./main.exe
```

The program will run a series of test cases demonstrating how to use the MyStationParser class to find the fastest path between subway stations.

## Features

The MyStationParser class provides the following features:

- Read subway station information and connection information from CSV files
- Calculate the fastest path between two subway stations
- Display the calculated fastest path using subway station identities as insputs
- Find subway station IDs by subway station names
- Calculate the fastest path between two subway stations using subway station names as inputs

## Instructions for Use

In the main.cpp :
First, create a MyStationParser object and pass in the paths to the subway station information file and connection information file. For example:

```cpp
travel::MyStationParser parser("./data/s.csv", "./data/c.csv");
```

Then, you can use the methods "compute_and_display_travel" to calculate the fastest path between subway stations using subway station identities as insputs. For example:

```cpp
parser.compute_and_display_travel(1722, 2062);
```

You can also calculate the fastest path using subway station names and lines as inputs:

```cpp
parser.compute_and_display_travel("Saint-Lazare,3", "Bastille,1");
```

Note :  the input string should follow the format of "station name,  line", separated by a comma. As the same station may have different subway lines, it has different IDs. If only the station name is entered, it is not precise enough. 

If the user's input format is incorrect, the program also handles the exception.


