# i3guesstemator
__________________
## Description
__________________
i3guesstemator is a simple status bar generator for i3bar. 
The primary objective is to improve the accuracy of estimating the remaining battery life of a laptop.

## Dependencies
_________________
The only dependency is yaml-cpp.
On arch-based systems the following line will install yaml-cpp.
```bash
pacman -S yaml-cpp
```

## Compilation
_______________
First clone the repository. Next use the following commands:
```bash
cd i3guesstemator
mkdir build
cd build
cmake ..
make all
```

## Setup for i3bar
__________________
To use i3guesstemator with i3bar add 
```yaml
bar {
    status_command path/to/i3guesstemator path/to/i3guesstemator.yaml
}
```
to your i3 config file. An example config file is contained in the config folder of the repository.
