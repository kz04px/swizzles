# Swizzles

A C++20 chess engine

---

## Testing
Run all tests with the following:
```
./test
```
Given the size of perft, excluding it is a lot faster:
```
./test --test-suite-exclude=DeepPerft
```

---

## Thanks
- [doctest](https://github.com/doctest/doctest) - The fastest feature-rich C++11/14/17/20 single-header testing framework 
- Roman Hartmann for creating the chess perft positions found [here](http://www.rocechess.ch/perft.html).
- Andrew Grant for creating the Chess960 perft positions found [here](https://www.chessprogramming.org/Chess960_Perft_Results).
