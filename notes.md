The formula for converting a 2-digit hex value (a byte (0 - 255)) to a corresponding value between -90 and +10 on the logarithmic scale used by the ui fader is shown here.
x is the input hex value, converted to an integer, and y is the value that the UI would show.

y = log10(x * 9/255 + 1) * 100 - 90


The formula for converting the UI fader value to an integer between 0 and 255 then is:

x = (28.33 * 10^(0.01*y + 0.9) ) - 28.33

which is an approximation.

