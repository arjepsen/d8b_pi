

#include "FXSlotClass.h"
#include "CommandStrings.h"
#include <sstream>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <map>

// UNCOMMENT TO ENABLE DEBUG MESSAGES.
#define FX_INIT_DEBUG_MESSAGES

#ifdef FX_INIT_DEBUG_MESSAGES
#define DEBUG_MSG(format, ...) printf("FX_INIT_DBG: " format, ##__VA_ARGS__)
#else
#define DEBUG_MSG(format, ...) ((void)0) // do {} while (0)
#endif


// The queries and responses are almost identical, with only 1 or 2 bytes being different.
// Construct the strings from these bases.
#define MFX_QUERY_BASE "F0q00q00q0Bq00q4Xq59q42q00q30q00q01q01q00q32qF7q"
#define UFX_QUERY_BASE "F0q00q00q50q0Yq4Yq59qF7q"
#define MFX_REPLY_BASE "F0q00q00q0Bq00q4Xq58q00qF7q"
#define UFX_REPLY_BASE "F0q00q00q50q0Yq4Yq58q05q00q00q00qF7q"
#define UFX_INIT_BASE "F0q00q00q50q00q4Zq4FqF7q"


// Constructor (initialization list)
FXSlot::FXSlot(int brainDescriptor, FXSlotID slotID)  // : fxcard(nullptr)
{
    DEBUG_MSG("\n============= FXSlot Constructor ================\n");

    // Setup commands, depending on slot:
    char mfxQuery[] = MFX_QUERY_BASE;
    char mfxReply[] = MFX_REPLY_BASE;
    char slot = slotID + 65;   // 65 = ascii 'A'

    std::map<FXSlotID, char> mfxLookupTable
    {
        {FX_SLOT_A, '1'},
        {FX_SLOT_B, '5'},
        {FX_SLOT_C, '9'},
        {FX_SLOT_D, 'D'}
    };

    mfxQuery[16] = mfxLookupTable[slotID];
    mfxReply[16] = mfxLookupTable[slotID];

    // Query for MFX.
    DEBUG_MSG("Querying slot %c for MFX card\n", slot);

    // Clear queue of l's and k's
    tcflush(brainDescriptor, TCIOFLUSH);

    // Send the query string.
    write(brainDescriptor, mfxQuery, strlen(mfxQuery));

    // Check reply
    if (getBrainResponse(brainDescriptor) == mfxReply)
    {
        DEBUG_MSG("Found MFX card in slot %c\n", slot);
        
        fxCardID = MFX_CARD;
    }
    else
    {
        // Set up commands for UFX querying.
        DEBUG_MSG("No MFX card found. Querying for UFX card.\n");
        char ufxQuery[] = UFX_QUERY_BASE;
        char ufxReply[] = UFX_REPLY_BASE;
        char ufxInit1[] = UFX_INIT_BASE;
        char ufxInit2[] = UFX_INIT_BASE;
        // "slot" is still having it's value from above.

        std::map<FXSlotID, char> ufxLookupTable
        {
            {FX_SLOT_A, '0'},
            {FX_SLOT_B, '4'},
            {FX_SLOT_C, '8'},
            {FX_SLOT_D, 'C'}
        };

        ufxQuery[13] = ufxQuery[16] = ufxLookupTable[slotID];
        ufxReply[13] = ufxReply[16] = ufxLookupTable[slotID];
        ufxInit1[13] = ufxInit1[16] = ufxLookupTable[slotID];
        ufxInit2[13] = ufxInit2[16] = ufxLookupTable[slotID] + 2;

        // Send UFX Query.
        write(brainDescriptor, ufxQuery, strlen(ufxQuery));

        // Check reply.
        if (getBrainResponse(brainDescriptor) == ufxReply)
        {
            DEBUG_MSG("Found UFX card in slot %c\n", slot);
            DEBUG_MSG("Sending UFX init commands\n");
            write(brainDescriptor, ufxInit1, strlen(ufxInit1));
            write(brainDescriptor, ufxInit2, strlen(ufxInit2));

            fxCardID = UFX_CARD;
        }
        else
        {
            DEBUG_MSG("No FX card in slot %c\n. Truly a sad day!\n", slot);

            fxCardID = FX_EMPTY;
        }
    }

    DEBUG_MSG("\n       ==== End of FXSlot Constructor ==== \n\n");
}


std::string FXSlot::getBrainResponse(int brainDescriptor)
{
    char response = '\0';
    std::stringstream brainReplyStream;

    do
    {
        int result = read(brainDescriptor, &response, 1);
        if (result < 1)
        {
            printf("Read returned: %d\n", result);
            printf("Which long calculations have shown is less than 1!");
            exit(1);
        }

        if (response != 'l' && response != 'k')
            brainReplyStream.put(response);

    } while (response != 'l' && response != 'k');

    return brainReplyStream.str();
}

FXCardID FXSlot::getCardID()
{
    return fxCardID;
}


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
