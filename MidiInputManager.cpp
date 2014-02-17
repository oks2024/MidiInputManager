#include "stdafx.h"
#include "MidiInputManager.h"


MidiInputManager::MidiInputManager(void)
{
    // Initialize midi sates
    for (int i = 0; i < 128; i++)
    {
        // Default value.
        m_MidiState[i][0] = 0;

        // Default min.
        m_MidiState[i][1] = 0;

        // Default max.
        m_MidiState[i][2] = 127;
    }

    // Midi
    m_pMidiIn = new RtMidiIn();
    m_pMidiOut = new RtMidiOut();

    // Check available ports
    unsigned int nPorts = m_pMidiIn->getPortCount();
    if ( nPorts == 0 ) 
    {
        return;
    }
    m_pMidiIn->openPort( 0 );

    nPorts = m_pMidiOut->getPortCount();
    if ( nPorts == 0 ) 
    {
        return;
    }
    m_pMidiOut->openPort( 1 );

    // Don't ignore sysex, timing, or active sensing messages.
    m_pMidiIn->ignoreTypes( false, false, false );

    // Initialize out message.
    m_OutMessages.push_back(176);
    m_OutMessages.push_back(0);
    m_OutMessages.push_back(0);

    TurnOffAllButtons();
}


MidiInputManager::~MidiInputManager(void)
{
    TurnOffAllButtons();
    Update();
    delete m_pMidiIn;
}

void MidiInputManager::Update()
{
    //// Midi inputs

    // Clean the previous messages
    m_Messages.clear();
    m_pMidiIn->getMessage( &m_Messages );
    
    // Update midi state.
    int nByte= m_Messages.size();
    for (int i = 0; i < nByte; ++i)
    {
        ++i; //ignore the first byte.

        int input = m_Messages[i];
        int value = m_Messages[i + 1];

        // Check if it's a button
        if (IsMidiButton(input))
        {
            if (value == 0)
            {
                m_MidiState[input][0] = !m_MidiState[input][0];

                if ( m_MidiState[input][0])
                {
                    m_OutMessages[1] = input;
                    m_OutMessages[2] = 127;
                    m_pMidiOut->sendMessage(&m_OutMessages);

                    UpdateButtonState(input);
                }
                else
                {
                    m_OutMessages[1] = input;
                    m_OutMessages[2] = 0;
                    m_pMidiOut->sendMessage(&m_OutMessages);
                }
            }
        }
        else
        {
            m_MidiState[input][0] = (float)value;
        }
        ++i;
    }
}

bool MidiInputManager::IsMidiButton(int p_Input)
{
    return p_Input >= 32;
}

float MidiInputManager::GetMidiValue(NanoKontrolInputs p_Input)
{
    // If it's a button, just return the value.
    if ( IsMidiButton(p_Input))
        return m_MidiState[p_Input][0];

    // Return the value in the wanted range.
    float rangeFactor = (m_MidiState[p_Input][2] - m_MidiState[p_Input][1]) / 127.0f;
    return m_MidiState[p_Input][0] * rangeFactor + m_MidiState[p_Input][1];
}

void MidiInputManager::UpdateButtonState(int p_Input)
{
    // Record buttons.
    if ((p_Input >= NKI_R1) && (p_Input <= NKI_R8))
    {
        TurnOffButton(p_Input + (NKI_M1 - NKI_R1));
        TurnOffButton(p_Input + (NKI_S1 - NKI_R1));
        return;
    }

    // Mute buttons.
    if ((p_Input >= NKI_M1) && (p_Input <= NKI_M8))
    {
        TurnOffButton(p_Input + (NKI_R1 - NKI_M1));
        TurnOffButton(p_Input + (NKI_S1 - NKI_M1));
        return;
    }

    // Solo buttons.
    if ((p_Input >= NKI_S1) && (p_Input <= NKI_S8))
    {
        TurnOffButton(p_Input + (NKI_M1 - NKI_S1));
        TurnOffButton(p_Input + (NKI_R1 - NKI_S1));
        return;
    }
}

void MidiInputManager::TurnOffButton(int p_Input)
{
    m_OutMessages[1] = p_Input;
    m_OutMessages[2] = 0;
    m_pMidiOut->sendMessage(&m_OutMessages);
}

void MidiInputManager::TurnOnButton(int p_Input)
{
    m_OutMessages[1] = p_Input;
    m_OutMessages[2] = 127;
    m_pMidiOut->sendMessage(&m_OutMessages);
}

void MidiInputManager::TurnOffAllButtons()
{
    // Turn off all leds.
    for (int i = 1; i < 127; i ++)
    {
        m_OutMessages[1] = i;
        m_OutMessages[2] = 0;
        m_pMidiOut->sendMessage(&m_OutMessages);
    }
}

void MidiInputManager::SetDefaultValue(NanoKontrolInputs p_Input, float p_Value)
{
    // Store the value in the 0-255 midi range.
    p_Value -= m_MidiState[p_Input][1];
    float range = m_MidiState[p_Input][2] - m_MidiState[p_Input][1];
    m_MidiState[p_Input][0] = (p_Value / range) * 255.0f;
}

void MidiInputManager::SetMinAndMaxValues(NanoKontrolInputs p_Input, float p_Min, float p_Max)
{
    m_MidiState[p_Input][1] = p_Min;
    m_MidiState[p_Input][2] = p_Max;
}


MidiInputManager& MidiInputManager::Instance()
{
    //if (m_Instance == NULL)
    //{
    //    m_Instance = new MidiInputManager();
    //}

    //return m_Instance;

    static MidiInputManager instance;

    return instance;
}

//MidiInputManager* MidiInputManager::m_Instance = NULL;

