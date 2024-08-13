/*
  ==============================================================================

    faderCalibration.h
    Created: 13 Aug 2024 5:44:50pm
    Author:  anders

  ==============================================================================
*/

#pragma once

#include "SharedDataStructures.h"


void calibrationPrep()
{
    // Turn off led's for the EQ (or other active buttons in that section)
    // turn off Select1-4 buttons, and any ring leds on EQ vpots. Also the ON button led.
    // Memory A is on - if memory B is on, turn it off, and turn memorry A on. (just leds....)

    // Clear display. (one command, or spaces??)
    // Set fourth select button led to blink ("47Ek") (cancel command).
    // Turn on the third select button LED ("47Di") (OK command)

    // Write to display: 
    // "Calibrate fader motors? Are you sure?"
    // "                       OK      Cancel"
    // 80u43v61v6Cv69v62v72v61v74v65v8Au66v61v64v65v72v90u6Dv6Fv74v6Fv72v73v3Fv99u41v72v65v9Du79v6Fv75vA1u73v75v72v65v3FvD8u4Fv4BvE0u43v61v6Ev63v65v6Cv
                    
}


void calibrateFader(ChStripID stripID)
{
    // Somehow we have to get the program to ignore fader move messages (and others?) while calibrating.
    // This might be done by somehow pausing the brain receiver thread?
    // However, originally it is clear that the messaging is still running throughout the calibration,
    // because we do get heartbeats in there.
}