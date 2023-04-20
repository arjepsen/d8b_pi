
#include <stdint.h>
#include <string>


// We probably do need both a channel class, and a channelstrip class, because a channelstrip can be linked to different channels.
class Channel
{
    private:

        // Probably most of these variables, are just a "local" saved reference of what the given channels parameter is set at.
        // So it is not directly tied to the "physical" value, and can thus end up reporting something wrong.

        uint8_t channelID; //unique ID
        uint8_t volume;
        //uint8_t fader_position; // Maybe fader position is just deduced from volume. - FADER IS IN CHANNELSTRIP.
        uint8_t pan_value;  // maybe this should be an int8_t (signed) instead?

        char channel_name[6];   // The channel name - something like Ch. 16 initially. CHECK NEEDED AMOUNT OF CHARS

        bool assignment[9];     // This array should hold the list of which assignments the channel has

        bool mute;  // Muting is done by setting volume to 0, so there should be some mechanism to return to previous volume, when unmuting.
        bool solo;  // maybe there should be a general list of soloed channels somewhere?
        // A channel probably does not need a "select"... this is something in channelstrip.
        // Record and Write, are probably also just a channel-strip thing.
        uint8_t auxes[12];  // saved send volume for the auxes (1-8, plus 9-10, 11-12 and their pans.)
        bool pre_post_aux_send; // saved reference of whether aux send is set to pre- or post-fader.SHOULD THIS BE SOMEHOW COMBINED WITH THE AUX-ARRAY?

        // Should these be classes also??
        int pre_dsp_insert; // somehow must ID channel-, bus-, aux- taps or 2-track inputs or plugins.
        int post_dsp_insert;    // only plugins possible as post insert. this variable must ID.
        int key_input;  // ID for what is set as key for key-eq/dynamics.. Default is channel iteself. Others: ch.tap, alt-return, bus-tap, aux-tap, 2-track input.

        
        // Channel DSP:
        int delay;
        int digital_trim;   // Need to sniff this one. This one is shown as the blue bar
        int level_to_tape;  // the red bar.
        int phase;
        // Gate-in meter???
        int Gate; // this might have to be a separate class?
        int Compressor; // Might also have to be a class.
        // Compressor-out meter??
        int hi_pass;    // Unsure how much is in this - is it just a boolean or??
        //int EQ; // This should probably also be a class.
        float eqLow;
        float eqMid;
        float eqHigh;
        float eqLowQ;
        float eqMidQ;
        float eqHighQ;


    public:

        bool set_volume;
        bool get_volume;
        bool get_channel_number;
        bool set_pan;
        bool get_pan;
        bool get_mute;
        bool set_mute;
        bool set_assignment;
        bool get_assignment;

        // Constructor
        Channel(uint8_t id) : channelID(id)
        {
            // Set default values for parameters
        }

}

class morechan {
public:
    int channelID;
    std::string inputSource;    // Is this relevant? I think each channel has that one specific input.....
    //float gain;
    float eqLow;
    float eqMid;
    float eqHigh;
    float eqLowQ;
    float eqMidQ;
    float eqHighQ;
    float pan;
    bool mute;
    bool solo;
    std::vector<float> auxSends;
    float faderLevel;
    // Dynamics processing parameters
    // ...

    Channel(int id) : channelID(id) {
        // Initialize default values for other parameters
    }

    // Add member functions to set and get parameters
};




#include "../JuceLibraryCode/JuceHeader.h"
#include "Channel.h"

class AudioEngine : public juce::AudioAppComponent {
public:
    AudioEngine() {
        // Instantiate your channel objects here
        for (int i = 0; i < numberOfChannels; ++i) {
            channels.emplace_back(i);
        }
        
        // Other initialization code...
    }

    // Implement other necessary functions like prepareToPlay, getNextAudioBlock, releaseResources
    // ...

private:
    static const int numberOfChannels = 56;
    std::vector<Channel> channels;
};



class CommandRegistry
{
    private: 
    std::unordered_map<std::string, std::function<std::unique_ptr<Command>() >>commandMap;

    public:
    void registerCommand(const std::string& commandName, std::function<std::unique_ptr<Command>()> commandFactory)
    {
        commandMap[commandName] = commandFactory;
    }

    std::unique_ptr<Command> createCommand(const std::string& commandName)
    {
        auto it = commandMap.find(commandName);
        if (it != commandMap.end())
        {
            return it->second();
        }
        return nullptr;
    }
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

std::string FXSlot::get_brainresponse(int brain_descriptor)
{
    char response = '\0';
    std::stringstream brain_reply_stream;

    do
    {
        int result = read(brain_descriptor, &response, 1);
        if (result < 1)
        {
            printf("Read returned: %d\n", result);
            printf("Which long calculations have shown is less than 1!");
            exit(1);
        }

        if (response != 'l' && response != 'k')
            brain_reply_stream.put(response);

    } while (response != 'l' && response != 'k');

    return brain_reply_stream.str();
}

