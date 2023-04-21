// #define FXA_MFX_QUERY "F0q00q00q0Bq00q41q59q42q00q30q00q01q01q00q32qF7q"
// #define FXB_MFX_QUERY "F0q00q00q0Bq00q45q59q42q00q30q00q01q01q00q32qF7q"
// #define FXC_MFX_QUERY "F0q00q00q0Bq00q49q59q42q00q30q00q01q01q00q32qF7q"
// #define FXD_MFX_QUERY "F0q00q00q0Bq00q4Dq59q42q00q30q00q01q01q00q32qF7q"
// #define MFX_QUERY_BASE "F0q00q00q0Bq00q%1c%2cq59q42q00q30q00q01q01q00q32qF7q"
// Here we have put in an X to be changed, depending on what slot we are querying.
// #define MFX_QUERY_BASE "F0q00q00q0Bq00q4Xq59q42q00q30q00q01q01q00q32qF7q"

// #define FXA_MFX_REPLY "F0q00q00q0Bq00q41q58q00qF7q"
// #define FXB_MFX_REPLY "F0q00q00q0Bq00q45q58q00qF7q"
// #define FXC_MFX_REPLY "F0q00q00q0Bq00q49q58q00qF7q"
// #define FXD_MFX_REPLY "F0q00q00q0Bq00q4Dq58q00qF7q"
// FXA_QUERY_UFX = "F0q00q00q50q00q40q59qF7q"
// FXB_QUERY_UFX = "F0q00q00q50q04q44q59qF7q"
// FXC_QUERY_UFX = "F0q00q00q50q08q48q59qF7q"
// FXD_QUERY_UFX = "F0q00q00q50q0Cq4Cq59qF7q"
// FXA_UFX_REPLY = "F0q00q00q50q00q40q58q05q00q00q00qF7q"
// FXB_UFX_REPLY = "F0q00q00q50q04q44q58q05q00q00q00qF7q"
// FXC_UFX_REPLY = "F0q00q00q50q08q48q58q05q00q00q00qF7q"
// FXD_UFX_REPLY = "F0q00q00q50q0Cq4Cq58q05q00q00q00qF7q"
// FXA_UFX_CMD1 = "F0q00q00q50q00q40q4FqF7q"
// FXA_UFX_CMD2 = "F0q00q00q50q02q42q4FqF7q"
// FXB_UFX_CMD1 = "F0q00q00q50q04q44q4FqF7q" 
// FXB_UFX_CMD2 = "F0q00q00q50q06q46q4FqF7q"
// FXC_UFX_CMD1 = "F0q00q00q50q08q48q4FqF7q" 
// FXC_UFX_CMD2 = "F0q00q00q50q0Aq4Aq4FqF7q"
// FXD_UFX_CMD1 = "F0q00q00q50q0Cq4Cq4FqF7q"
// FXD_UFX_CMD2 = "F0q00q00q50q0Eq4Eq4FqF7q"
// #define FXA_GOT_MFX "C3u4Dv46v58v"
// #define FXB_GOT_MFX "CDu4Dv46v58v"
// #define FXC_GOT_MFX "D7u4Dv46v58v"
// #define FXD_GOT_MFX "E1u4Dv46v58v"
// #define FXA_GOT_UFX "C3u55v46v58v"
// #define FXB_GOT_UFX "CDu55v46v58v"
// #define FXC_GOT_UFX "D7u55v46v58v"
// #define FXD_GOT_UFX "E1u55v46v58v"
// #define FXA_EMPTY "C2u65v6Dv70v74v79v"
// #define FXB_EMPTY "CCu65v6Dv70v74v79v"
// #define FXC_EMPTY "D6u65v6Dv70v74v79v"
// #define FXD_EMPTY "E0u65v6Dv70v74v79v"


#include "FXSlotClass.h"
#include "CommandStrings.h"
#include <sstream>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#include <map>

// The queries and responses are almost identical, with only 1 or 2 bytes being different.
// Construct the strings from these bases.
#define MFX_QUERY_BASE "F0q00q00q0Bq00q4Xq59q42q00q30q00q01q01q00q32qF7q"
#define UFX_QUERY_BASE "F0q00q00q50q0Yq4Yq59qF7q"
#define MFX_REPLY_BASE "F0q00q00q0Bq00q4Xq58q00qF7q"
#define UFX_REPLY_BASE "F0q00q00q50q0Yq4Yq58q05q00q00q00qF7q"
#define UFX_INIT_BASE "F0q00q00q50q00q4Zq4FqF7q"
#define MFX_DISPLAY_BASE "u4Dv46v58v"
#define UFX_DISPLAY_BASE "u55v46v58v"
#define EMPTY_DISPLAY_BASE "u65v6Dv70v74v79v"


// Constructor (initialization list)
FXSlot::FXSlot(int brain_descriptor, FXSlotID slot_id) : fxcard(nullptr)
{
    // Setup commands, depending on slot:
    char mfx_query[] = MFX_QUERY_BASE;
    char mfx_reply[] = MFX_REPLY_BASE;
    char slot = slot_id + 65;   // 65 = ascii 'A'

    std::map<FXSlotID, char> mfx_lookup_table
    {
        {FX_SLOT_A, '1'},
        {FX_SLOT_B, '5'},
        {FX_SLOT_C, '9'},
        {FX_SLOT_D, 'D'}
    };

    mfx_query[16] = mfx_lookup_table[slot_id];
    mfx_reply[16] = mfx_lookup_table[slot_id];

    // Query for MFX.
    printf("Querying slot %c for MFX card\n", slot);
    printf("(Query string: %s\n)", mfx_query);

    // Clear queue of l's and k's
    tcflush(brain_descriptor, TCIOFLUSH);

    write(brain_descriptor, mfx_query, strlen(mfx_query));

    // Check reply
    if (get_brainresponse(brain_descriptor) == mfx_reply)
    {
        printf("Found MFX card in slot %c\n", slot);
        
        // Create FX card obect.
        FXCard *card = new FXCard("MFX");
        fxcard = card;

        // // Set up display command.
        // char display_mfx_cmd[] = MFX_DISPLAY_BASE;
        // std::map<FXSlotID, const char*> mfx_position_cmd
        // {
        //     {FX_SLOT_A, "C3"},
        //     {FX_SLOT_B, "CD"},
        //     {FX_SLOT_C, "D7"},
        //     {FX_SLOT_D, "E1"}
        // };

        // display_mfx_cmd[0] = mfx_position_cmd[slot_id][0];
        // display_mfx_cmd[1] = mfx_position_cmd[slot_id][1];

        // //  Send the command.
        // write(brain_descriptor, display_mfx_cmd, strlen(display_mfx_cmd));
    }
    else
    {
        // Set up commands for UFX querying.
        printf("No MFX card found. Querying for UFX card.\n");
        char ufx_query[] = UFX_QUERY_BASE;
        char ufx_reply[] = UFX_REPLY_BASE;
        char ufx_init1[] = UFX_INIT_BASE;
        char ufx_init2[] = UFX_INIT_BASE;
        // "slot" is still having it's value from above.

        std::map<FXSlotID, char> ufx_lookup_table
        {
            {FX_SLOT_A, '0'},
            {FX_SLOT_B, '4'},
            {FX_SLOT_C, '8'},
            {FX_SLOT_D, 'C'}
        };

        ufx_query[13] = ufx_query[16] = ufx_lookup_table[slot_id];
        ufx_reply[13] = ufx_reply[16] = ufx_lookup_table[slot_id];
        ufx_init1[13] = ufx_init1[16] = ufx_lookup_table[slot_id];
        ufx_init2[13] = ufx_init2[16] = ufx_lookup_table[slot_id] + 2;

        // Send UFX Query.
        printf("mr. UFX.... are you there?\n");
        write(brain_descriptor, ufx_query, strlen(ufx_query));


        printf("Checking reply\n");
        // Check reply.
        if (get_brainresponse(brain_descriptor) == ufx_reply)
        {
            printf("Found UFX card in slot %c\n", slot);
            printf("Sending UFX init commands\n");
            write(brain_descriptor, ufx_init1, strlen(ufx_init1));
            write(brain_descriptor, ufx_init2, strlen(ufx_init2));

            FXCard *card = new FXCard("UFX");
            fxcard = card;
            // SEND DISPLAY COMMAND
            //return;
        }
        else
        {
            printf("No FX card in slot %c\n. Truly a sad day!\n", slot);
            FXCard *card = new FXCard("empty");
            fxcard = card;
            // SEND DISPLAY COMMAND
        }
    }

    printf("Checking get_card function:\n");
    printf("%s\n", get_card().c_str());

    // Done querying. Update display
    if (get_card() == "empty")
    {
        std::map<FXSlotID, std::string> display_lookup_table
        {
            {FX_SLOT_A, "C2"},
            {FX_SLOT_B, "CC"},
            {FX_SLOT_C, "D6"},
            {FX_SLOT_D, "E0"}
        };
        std::string display_command = display_lookup_table[slot_id] + EMPTY_DISPLAY_BASE;

        write(brain_descriptor, display_command.c_str(), display_command.length());
    }
    else
    {
        std::map<FXSlotID, std::string> display_lookup_table
        {
            {FX_SLOT_A, "C3"},
            {FX_SLOT_B, "CD"},
            {FX_SLOT_C, "D7"},
            {FX_SLOT_D, "E1"}
        };
        if (get_card() == "MFX")
        {
            std::string display_command = display_lookup_table[slot_id] + MFX_DISPLAY_BASE;
            write(brain_descriptor, display_command.c_str(), display_command.length());
        }
        else
        {
            std::string display_command = display_lookup_table[slot_id] + UFX_DISPLAY_BASE;
            write(brain_descriptor, display_command.c_str(), display_command.length());
        }

    }








}

FXSlot::~FXSlot()
{
    delete fxcard;
}

std::string FXSlot::get_card()
{
    return fxcard->get_type();
}

// void FXSlot::init()
// {
//     // Query slot.

//     // When we know what card it is - create a new object:

//     FXCard *card = new FXCard();
//     // store FXCard object as a member of this fxslot object
//     fxcard = card;

//     // membercard is a pointer to an FXCard object, so it should be initialized
//     // to "nullptr" in the constructor of this fxslot class, and deleted with the desturctor.
// }

std::string FXSlot::get_brainresponse(int brain_descriptor)
{
    char response = '\0';
    std::stringstream brain_reply_stream;

    // while (true)
    // {
    //     int result = read(brain_descriptor, &response, 1);
    //     if (response == 'l' || response == 'k')
    //     {
    //         printf("got an l or k - end of message.\n");
    //         break;
    //     }

    //     if (result == 1)
    //   