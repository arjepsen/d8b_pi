## Update may '25
So, I may finally have overcome the brickwall mentioned below.
It seems chatgpt has finally read enough of the internet, and gotten smart enough, to move on and give me some valueable insight into what is going on with how the EQ settings construct a 20-byte value.
more to come.

# d8b_pi
This is a project that aims to replace the "cpu-unit" of the Mackie d8b mixer with a simple raspberry pi.
The mixer contains two ADSP2181 processors. One on the "Brain" board, and one on the DSP board.
The Brain handles the physical interaction with the buttons, LED's, v-pots, displays, and a bit of audio routing.
The DSP handles the audio itself.
So essentially: When the user moves a fader, Mr. Brain sends a message to the cpu-unit, which then sends a command to the DSP.
I have sniffed out about 95-99% of these messages and commands. And are in the proces of putting together a new interface using the JUCE library. The code should likely compile and run on any linux machine - including a rapsberry pi.

### I HIT A BRICK WALL

So, I have hit a roadblock with this project, in relation to the EQ.
When changing one of the three parameters for an EQ band (Gain, Frequency and Q), the cpu-unit sends a 20-byte long value to the DSP. Unfortunately - so far - I have been unable to reverse engieneer how this valuer gets calculated. I wonder if it is sort of filter coefficients of a bi-quad filter, but I am not certain.
I have sniffed out some of these 20-byte values, sweeping through one parameter, while keeping the other two constant - these are available in the docs/EQ investigations folder.

Here is what I do know:



* Each parameter (Gain, Frequency, Q) has 256 "steps" in their range on any given band. This range seems to be exponential - for example, in the lower range of Frequency there are several steps of 20Hz, whereas in the upper range there are larger and larger jumps between the values of each step.

* The 20-byte value seems to be seperated into five groups of 4 bytes.

* Change affects all five groups - When changing any one parameter, all five groups change
* Group 2 and 4 seems to be kind of "inversions" or "mirrors" of each other - they will always have the same value of change between each specific step, but with opposite sign. So when group 3 increases with an increase in step, group 4 will decrease with an equal amount.
* The first byte of group 1 seems to be a "sign bit" for Gain - when gain is positive it is 1 - when negative it is 0.
* When Gain is 0, the whole 20 byte value is 0. This makes sense, in that when gain is 0, the eq should be flat and have no impact on the audio. But, it seems like the groups does not converge predictably toward 0-gain as center.
* Gain "value" seems to be used directly. Meaning: as mentioned, the parameters have 256 steps of values. Gain goes from -15.0 up to 15.0. Since there's only 256 steps, this cannot be done all the way in steps of 0.1, so at regular intervals the value jumps 0.2 instead of 0.1.
When keeping Frequency and Q constant, and sweeping through the Gain steps, it becomes evident that these jumps are also reflected in the jumps of value in all five groups.
* Group 5 seems to always have a value-change from step to step that is very close the the value of change in group 2/4, but not exactly the same.
---
* When Sweeping through the gain range, keeping the other two constant, the following is noticed for each group:
    - Group 1:
    Values increase with gain increase.    
Jump in values increase as gain increase. The jump between each step decreases slightly from gain=-15 to 0, and increases again slightly from 0 up to +15
    - Group 2:
    Value decreases as gain increases.
    The jump in value decreases though the whole gain range.
    - Group 3:
    Value increase as gain goes from -15 to 0, and then decreases as gain goes from 0 to 15.
    The value of change decreases from gain=-15 to 0, and opposite for 0 to 15.
    - Group 4:
    As described, mirrors group 2. For each step in gain, they change with exactly the same value, except where group 2 decreases, group four increases.
    - Group 5:
    Decreases as gain increases.
    The value of change in each step decreases all the way from gain=-15 to +15.
---
* Sweeping through the Frequency range while keeping Gain and Q constant shows the following (I suspect that the change between each value in each group follows the exponential range of the frequency):
    - Group 1:
    Value of change between each step increases as Frequency increases. It could seem like the value of change is related to the exponential change in the Frequency range.
    - Group2:
    Value increase as Frequency increase, with larger values of change as frequency increase.
    - Group 3: 
    Decreases as Frequency increase.
    Values of change increases as Freqency increase.
    - Group 4: 
    Mirrors group 2
    - Group 5: increases as Frequency increase.
---
* Sweeping through Q, while keeping Gain and Frequency constant. Like the Frequency range, Q have a sort of exponential range of values over the 256 steps.
For all groups it is seen, that the value of change between each step decreases as Q increases.
    - Group 1:
    Decreases as Q increases.
    - Group 2:
    Decreases as Q increases.
    - Group 3:
    Increase as Q increases.
    - Group 4: 
    mirrors group 2
    - Group 5: 
    decreases as Q increases.
    
