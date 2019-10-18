# Adafruit-870-and-3152
An Arduino sketch showing how to combine a scrolling message using multiple Adafruit mini 8x8 matrix (PID 870) and multiple Adafruit 8x16 mini matrix (PID 3152).

I use the Adafruit mini 8x8 LED matrix (PID 870) and the Adafruit mini 8x16 LED matrix (PID 3152) for quite a few projects.  Trouble was, I needed to create an 8x48 led matrix array.  The 870 would only let me create an 8x32.  While the 3152 could conceivably create an 8x48 (using 3 modules), due to the way the board is laid out, they cannot be set side by side to make one continual matrix.  But you can do a little finagling with the pins and soldering to make (1) 3152 fit next to the 870's to make one long 8x48 matrix.

This sketch is me combining the 870's and the 3152's to be able to scroll one message across all of the modules.  During my testing, I was able to scroll a single message across an 8x80 matrix display (using 4 870's and 3 3152's).

I stripped down the code to its bare minimums to make it work so anyone else needing to try this has something to work with.
