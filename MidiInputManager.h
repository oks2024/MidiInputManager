#pragma once
#include "RtMidi\RtMidi.h"
#include <cstdlib>

// For keyboards.
enum MidiInputs
{
    C1		= 60,
    C1S		= 61,
    D1		= 62,
    D1S		= 63,
    E1		= 64,
    F1		= 65,
    F1S		= 66,
    G1		= 67,
    G1S		= 68,
    A1		= 69,
    A1S		= 70,
    B1 		= 71,
    C2 		= 72,
    C2S		= 73,
    D2 		= 74,
    D2S		= 75,
    E2 		= 76,
    F2 		= 77,
    F2S		= 78,
    G2 		= 79,
    G2S		= 80,
    A2 		= 81,
    A2S		= 82,
    B2 		= 83,
    C3 		= 84,
    C3S		= 85,
    D3 		= 86,
    D3S		= 87,
    E3 		= 88,
    F3 		= 89,
    F3S		= 90,
    G3 		= 91,
    G3S		= 92,
    A3 		= 93,
    A3S		= 94,
    B3 		= 95,
    C4 		= 96,
    C4S		= 97,
    D4 		= 98,
    D4S		= 99,
    E4		= 100,
    F4		= 101,
    F4S		= 102,
    G4		= 103,
    G4S		= 104,
    A4		= 105,
    A4S		= 106,
    B4		= 107,
};

// For the Korg Nano Kontrol2.
enum NanoKontrolInputs
{
    // Faders
    NKI_F1					= 0,
    NKI_F2					= 1,
    NKI_F3					= 2,
    NKI_F4					= 3,
    NKI_F5					= 4,
    NKI_F6					= 5,
    NKI_F7					= 6,
    NKI_F8					= 7,

    // Knobs
    NKI_K1					= 16,
    NKI_K2					= 17,
    NKI_K3					= 18,
    NKI_K4					= 19,
    NKI_K5					= 20,
    NKI_K6					= 21,
    NKI_K7					= 22,
    NKI_K8					= 23,

    // Record buttons
    NKI_R1					= 64,
    NKI_R2					= 65,
    NKI_R3					= 66,
    NKI_R4					= 67,
    NKI_R5					= 68,
    NKI_R6					= 69,
    NKI_R7					= 70,
    NKI_R8					= 71,

    // Mute buttons
    NKI_M1					= 48,
    NKI_M2					= 49,
    NKI_M3					= 50,
    NKI_M4					= 51,
    NKI_M5					= 52,
    NKI_M6					= 53,
    NKI_M7					= 54,
    NKI_M8					= 55,

    // Solo buttons
    NKI_S1					= 32,
    NKI_S2					= 33,
    NKI_S3					= 34,
    NKI_S4					= 35,
    NKI_S5					= 36,
    NKI_S6					= 37,
    NKI_S7					= 38,
    NKI_S8					= 39,

    NKI_PLAY				= 41,
    NKI_STOP				= 42,
    NKI_RECORD				= 43,
    NKI_BACKWARD			= 43,
    NKI_FORWARD				= 44,
    NKI_CYCLE				= 46,
    NKI_SET					= 60,
    NKI_PREVIOUS_MARKER		= 61,
    NKI_NEXT_MARKER			= 62,
    NKI_PREVIOUS_TRACK		= 63,
    NKI_NEXT_TRACK			= 64,

};

class MidiInputManager
{
public:
    static MidiInputManager* Instance();
    void Update();

    float GetMidiValue(NanoKontrolInputs p_Input);
    void SetDefaultValue(NanoKontrolInputs p_Input, float p_Value);
    void SetMinAndMaxValues(NanoKontrolInputs p_Inputs, float p_Min, float p_Max);

private:
    bool IsMidiButton(int input);
    void TurnOffAllButtons();
    void UpdateButtonState(int input);
    void TurnOnButton(int input);
    void TurnOffButton(int input);

    // Store the midi values, min output, max output.
    float m_MidiState[128][3];

    // Midi
    RtMidiIn* m_pMidiIn;
    RtMidiOut* m_pMidiOut;

    // Messages received from midi input during a frame.
    std::vector<unsigned char> m_Messages;
    std::vector<unsigned char> m_OutMessages;

    static MidiInputManager* m_Instance;

    MidiInputManager(void);
    ~MidiInputManager(void);
};

