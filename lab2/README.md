# Lab 2

## Part 1
* Part 1 of the lab involves polling `PIND` for switch changes. 
* Debouncing is done by ensuring consistent reads of the pin.

## Part 2
* We write digits 0-9 in a loop to the display.
* An `LCDNumber` is an array of 2-byte row encodings that will be looped and masked via the `colList` in the function `writeNumber` which in turn uses `writeLED` and then `shiftOut`

## Part 3
* The same as Part 2, except now we additionally listen for button presses with an interrupt handler in order to increment the display digits.
* The handler is called `handleButton`
* Debouncing is done on the rising edge of the button press, in which we just consistently write the current number state.
