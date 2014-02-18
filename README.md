As a graphics programmer, I often made small projects to test a new technique or a new effect. And I always have a lot of settings to expose, resulting in a huge mess of GUI code. 

To avoid that I’m using a midi controller (a Korg NanoKontrol2), and I’ve made a small manager to make it easy to use.
I am using RtMidi (http://www.music.mcgill.ca/~gary/rtmidi/) to receive and send inputs.

The input midi manager is designed to be easy to use. You first have to call the InputMidiManager::Instance().Update() function in your in your own update function and you can now get the fader 1 value using InputMidiManager::Instance().GetMidiInput(NKI_F1).

By default, values are in the range 0-127. To change that, you can initialize a fader or a knob using InputMidiManager::Instance().SetMinAndMaxValues(NKI_F1, 50, 500);
You can also define the default value with InputMidiManager::Instance().SetDefaultValue(NKI_F1, 200).
It’s just a first draft, since I’m using it I’ll add feature as soon as I need them. For now it’s only working with the Nanokontrol2, but it can easily be modified in order to be used with any midi controller.
You just have to add the files to your project to use it, and follow the RtMidi instructions (http://www.music.mcgill.ca/~gary/rtmidi/index.html#compiling).

You can find more informations on my website : www.alexandre-pestana.com .
