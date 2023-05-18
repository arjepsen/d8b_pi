#pragma once

#include <unordered_map>
#include <string>

#define DISPLAY_LOW "30u2v"
#define DISPLAY_MID "30u1v"
#define DISPLAY_HIGH "30u0v"

// The following commands writes:
//    D8B 5.1
// MACKIE DIGITAL
// the XXu is a position, the YYv is an character from the ascii table.
// #define WELCOME_STRING1 = "90u44v38v42v"             // D8B
// #define WELCOME_STRING2 = "94u35v2Ev31v"             // 5.1
// #define WELCOME_STRING3 = "CDu4Dv41v43v4Bv49v45v"    // MACKIE
// #define WELCOME_STRING4 = "D4u44v49v47v49v54v41v4Cv" // DIGITAL

// Let's make this version 6.0 (alpha), instead of 5.1
// The string for that is: "94u36v2Ev30v20v28v61v6Cv70v68v61v29v"


// The strings put together in one line.
//#define WELCOME_STRING "90u44v38v42v94u35v2Ev31vCDu4Dv41v43v4Bv49v45vD4u44v49v47v49v54v41v4Cv"
#define WELCOME_STRING "90u44v38v42v94u36v2Ev30v20v28v61v6Cv70v68v61v29vCDu4Dv41v43v4Bv49v45vD4u44v49v47v49v54v41v4Cv"

//#define BOOST_MESSAGE "80u 42v75v63v6Bv6Cv65v 20v 55v70 20v 44v6Fv72v6Fv74v68v79v2Cv C0u 27v63v61v75v73v65v 20v 4Bv61v6Ev73v61v73v2Ev2Ev2Ev 20v 69v73v 20v 67v6F69v6E67v 20v 42v79v65v2Dv42v79v65v"
#define BOOST_MESSAGE1 "80u42v75v63v6Bv6Cv65v20v55v70v20v44v6Fv72v6Fv74v68v79v2Cv20v27v63v61v75v73v65v20v4Bv61v6Ev73v61v73v2Ev2Ev2Ev"
#define BOOST_MESSAGE2 "C6u69v73v20v67v6Fv69v6Ev67v20v42v79v65v2Dv42v79v65v21vDCu28v62v6Fv6Fv73v74v20v6Dv6Fv64v65v29v"

#define UPLOAD_FIRMWARE_MESSAGE "80u55v70v6Cv6Fv61v64v69v6Ev67v20v46v69v72v6Dv77v61v72v65v73v2Ev2Ev2Ev"
#define UPLOADING_BRAIN_MESSAGE "80u55v70v6Cv6Fv61v64v69v6Ev67v20v42v72v61v69v6Ev57v61v72v65v2Ev2Ev2EvC6u28v62v72v61v61v61v69v69v6Ev6Ev7Av7Av2Ev2Ev2Ev29v"

//"80u55v70v6Cv6Fv61v64v69v6Ev67v20v44v53v50v20v46v69v72v6Dv77v61v72v65v20v"
#define UPLOADING_DSP_MESSAGE "80u55v70v6Cv6Fv61v64v69v6Ev67v20v44v53v50v20v46v69v72v6Dv77v61v72v65v20v28v4Dv61v73v74v65v72v29v"

// This is the response from the brain, after receiving firmware.
// R0027v0129v0202v0326v042Fv053Dv0623v073Dv083Cv0918v0A3Fv0B3Cv0C3Ev0D0Cv0E1Av0F2Cv1024v1127v1205v131Fv1419v1524v163Bv1705v1804v1A31v1B2Av1C39v1D37v1E2Dv1F03v2014v
// It is 32 hex values, first to digits going from 00 to 20 in hex.

// This writes: "TAPE A   TAPE B   TAPE C   ALTIO" with a dash under each.
// (it will overwrite the welcome string text, and includes spaces)
#define TAPE_LIST "81u54v41v50v45v86u41v8Bu54v41v50v45v90u42v20v20v94u20v54v41v50v45v9Au43vA0u41v4Cv54v49v4FvC4u2DvCDu20v2Dv20v20v20v20vD4u20v20v20v20v2Dv20v20vE2u2Dv"



#define QUERY_TAPE_A "80p"
#define QUERY_TAPE_B "81p"
#define QUERY_TAPE_C "82p"
#define QUERY_ALTIO "83p"
#define QUERY_DIGI_SLOT "84p"
#define QUERY_CLOCK "80o"

// These are the possible IO card replies to queries.
// Please note, there are other cards than these, but I do not have their id's
// (C) 1997 Mackie Designs {Mackie 8-channel 20-bit Analog interface}
#define AIO8_REPLY "28p43p29p20p31p39p39p37p20p4Dp61p63p6Bp69p65p20p44p65p73p69p67p6Ep73p20p7Bp4Dp61p63p6Bp69p65p20p38p2Dp63p68p61p6Ep6Ep65p6Cp20p32p30p2Dp62p69p74p20p41p6Ep61p6Cp6Fp67p20p69p6Ep74p65p72p66p61p63p65p7Dp0Dp"

// (C) 2000 Mackie Designs {Mackie Opt-8 24-Bit ADAT Light Pipe I/O}
#define OPT8_REPLY "28p43p29p20p32p30p30p30p20p4Dp61p63p6Bp69p65p20p44p65p73p69p67p6Ep73p20p7Bp4Dp61p63p6Bp69p65p20p4Fp70p74p2Dp38p20p32p34p2Dp42p69p74p20p41p44p41p54p20p4Cp69p67p68p74p20p50p69p70p65p20p49p2Fp4Fp7Dp0Dp"

// (C) 1997 Mackie Designs {Mackie Stereo AES/EBU interface}
#define AES_REPLY "28p43p29p20p31p39p39p37p20p4Dp61p63p6Bp69p65p20p44p65p73p69p67p6Ep73p20p7Bp4Dp61p63p6Bp69p65p20p53p74p65p72p65p6Fp20p41p45p53p2Fp45p42p55p20p69p6Ep74p65p72p66p61p63p65p7Dp0Dp"

// (C) 1997 Mackie Designs {Mackie Clock Card}
#define MACKIE_CLOCK_REPLY "28o43o29o20o31o39o39o37o20o4Do61o63o6Bo69o65o20o44o65o73o69o67o6Eo73o20o7Bo4Do61o63o6Bo69o65o20o43o6Co6Fo63o6Bo20o43o61o72o64o7Do0Do"

// (C) 1997 Mackie Designs {Apogee Low Jitter Sync Card}
#define APOGEE_CLOCK_REPLY "28o43o29o20o31o39o39o37o20o4Do61o63o6Bo69o65o20o44o65o73o69o67o6Eo73o20o7Bo41o70o6Fo67o65o65o20o4Co6Fo77o20o4Ao69o74o74o65o72o20o53o79o6Eo63o20o43o61o72o64o7Do0Do"

// These commands are used to write the card ID under the specific TAPE in list.
// Locations:
#define TAPE_A_LOC "C2u"
#define TAPE_B_LOC "CCu"
#define TAPE_C_LOC "D6u"
#define ALTIO_LOC "E0u"
#define DIGICARD_LOC "92u"
#define CLOCK_LOC "D2u"

// I/O Card names.
#define OPT8_STRING "4Fv50v54v2Dv38v"            // "OPT-8"
#define AIO8_STRING "41v49v4Fv2Dv38v"            // "AIO-8"
#define AES_STRING "41v45v53v2Fv45v42v55v"       // "AES/EBU"
#define MACKIE_CLOCK_STRING "4Dv41v43v4Bv49v45v" // "MACKIE"
#define APOGEE_CLOCK_STRING "41v50v4Fv47v45v45v" // "APOGEE"
#define EMPTY_SLOT_STRING "65v6Dv70v74v79v"      // "empty"

// DIGITAL I/O CARD:
#define DIGITAL_IO_STRING "80u44v49v47v49v54v41v4Cv20v49v2Fv4Fv20v43v41v52v44v3Av20v"

// CLOCK CARD:
#define CLOCK_STRING "C6u43v4Cv4Fv43v4Bv20v43v41v52v44v3Av"

// Response from Brain on "s" command
#define BRAIN_S_RESPONSE "000001E65288c"

// Overwriting previous text (both rows), and writing "LOADING DSP PLUGINS..."
#define LOADING_DSP "80u4Cv6Fv61v64v69v6Ev67v20v44v53v50v20v50v6Cv75v67v69v6Ev73v2Ev2Ev2Ev20v20v20vC6u20v20v20v20v20vB4u20v20v20v20v20vD2u20v20v20v20v20v20v"

// Display the FX card list
#define FX_CARD_LIST "80u20v46v58v43v61v72v64v88u41v20v20v46v58v43v61v72v64v20v42v20v20v46v58v43v61v72v64v9Cu43v9Fu46v58v43v61v72v64vA6u44vC4u2DvCEu2DvD8u2DvE2u2Dv"


// Commands for checking if a slot has an MFX card
#define FXA_MFX_QUERY "F0q00q00q0Bq00q41q59q42q00q30q00q01q01q00q32qF7q"
#define FXB_MFX_QUERY "F0q00q00q0Bq00q45q59q42q00q30q00q01q01q00q32qF7q"
#define FXC_MFX_QUERY "F0q00q00q0Bq00q49q59q42q00q30q00q01q01q00q32qF7q"
#define FXD_MFX_QUERY "F0q00q00q0Bq00q4Dq59q42q00q30q00q01q01q00q32qF7q"

// Commands for checking if slot has a UFX card
#define FXA_UFX_QUERY "F0q00q00q50q00q40q59qF7q"
#define FXB_UFX_QUERY "F0q00q00q50q04q44q59qF7q"
#define FXC_UFX_QUERY "F0q00q00q50q08q48q59qF7q"
#define FXD_UFX_QUERY "F0q00q00q50q0Cq4Cq59qF7q"

// std::unordered_map<std::string, std::string> fx_query
// {
//     {"mfxa", "F0q00q00q0Bq00q41q59q42q00q30q00q01q01q00q32qF7q"},
//     {"mfxb", "F0q00q00q0Bq00q45q59q42q00q30q00q01q01q00q32qF7q"},
//     {"mfxc", "F0q00q00q0Bq00q49q59q42q00q30q00q01q01q00q32qF7q"},
//     {"mfxd", "F0q00q00q0Bq00q4Dq59q42q00q30q00q01q01q00q32qF7q"},
//     {"ufxa", "F0q00q00q50q00q40q59qF7q"},
//     {"ufxb", "F0q00q00q50q04q44q59qF7q"},
//     {"ufxc", "F0q00q00q50q08q48q59qF7q"},
//     {"ufxd", "F0q00q00q50q0Cq4Cq59qF7q"}
// };


// Responses on MFX queries, in case slot holds that card.
#define FXA_MFX_REPLY "F0q00q00q0Bq00q41q58q00qF7q"
#define FXB_MFX_REPLY "F0q00q00q0Bq00q45q58q00qF7q" 
#define FXC_MFX_REPLY "F0q00q00q0Bq00q49q58q00qF7q"
#define FXD_MFX_REPLY "F0q00q00q0Bq00q4Dq58q00qF7q"

// Responses, on UFX queries, in case slot holds that card.
#define FXA_UFX_REPLY "F0q00q00q50q00q40q58q05q00q00q00qF7q"
#define FXB_UFX_REPLY "F0q00q00q50q04q44q58q05q00q00q00qF7q"
#define FXC_UFX_REPLY "F0q00q00q50q08q48q58q05q00q00q00qF7q"
#define FXD_UFX_REPLY "F0q00q00q50q0Cq4Cq58q05q00q00q00qF7q"

// If a UFX card is detected, two more commands are sent. Unsure exactly what they are... maybe some init stuff?
#define FXA_UFX_CMD1 "F0q00q00q50q00q40q4FqF7q"
#define FXA_UFX_CMD2 "F0q00q00q50q02q42q4FqF7q"

#define FXB_UFX_CMD1 "F0q00q00q50q04q44q4FqF7q" 
#define FXB_UFX_CMD2 "F0q00q00q50q06q46q4FqF7q"

#define FXC_UFX_CMD1 "F0q00q00q50q08q48q4FqF7q" 
#define FXC_UFX_CMD2 "F0q00q00q50q0Aq4Aq4FqF7q"

#define FXD_UFX_CMD1 "F0q00q00q50q0Cq4Cq4FqF7q"
#define FXD_UFX_CMD2 "F0q00q00q50q0Eq4Eq4FqF7q"

// Display strings for "emtpy" fx slot:
#define FXA_EMPTY "C2u65v6Dv70v74v79v"
#define FXB_EMPTY "CCu65v6Dv70v74v79v"
#define FXC_EMPTY "D6u65v6Dv70v74v79v"
#define FXD_EMPTY "E0u65v6Dv70v74v79v"

// Display strings for having an MFX
#define FXA_GOT_MFX "C3u4Dv46v58v"
#define FXB_GOT_MFX "CDu4Dv46v58v"
#define FXC_GOT_MFX "D7u4Dv46v58v"
#define FXD_GOT_MFX "E1u4Dv46v58v"

// Display strings for having an UFX
#define FXA_GOT_UFX "C3u55v46v58v"
#define FXB_GOT_UFX "CDu55v46v58v"
#define FXC_GOT_UFX "D7u55v46v58v"
#define FXD_GOT_UFX "E1u55v46v58v"