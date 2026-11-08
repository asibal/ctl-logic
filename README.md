# CTL-Logic

CTL-Logic is an additional UI layer built on top of the [CTL-1 custom firmware (by charlesvestal)](https://github.com/charlesvestal/fm1-ctl). I added features to make it more usable as a portable midi controller/control surface for audio/mixing (DAW's like Logic Pro, Reaper, etc) and video editing. Midi output is sent via USB midi and/or BLE midi.

Everything from the original CTL-1 custom firmware is still there. Press the Home button to switch between the CTL-Logic and CTL-1 interface.

[Try it in the browser](https://asibal.github.io/ctl-logic/emu/)

[Install from the browser](https://asibal.github.io/ctl-logic/install/)

**Install at your own risk.** I have only tried this on my own device. Install the official v15 firmware before installing this. I suggest using [FM-1 Backup & Restore](https://github.com/workeffortwaste/fm1-backup-restore/) (Android) to make sure you can backup and restore your own device before installing any custom firmware.

![CTL-Logic on the M-Vave FM-1](docs/img/ctl-logic-small.png)

**Main Features**
- Main screen displays the current channel name in large font and custom color. You will know your current channel by just glancing.
- Channel names and colors are editable (16 channels) . No need to remember channel numbers.

**Maximize the rotary encoders**
- Knobs 1-4 have editable CC
- 4 banks with quick bank switcher buttons
- Select & Algorithm knob have hardcoded CC's.
- Preset knob as channel switcher (1-16)
- All knobs send relative values (2's complement format)

**All buttons send CC's**
- All buttons send CC's, including the keyboard midi buttons (hardcoded CC's). Keyboard midi notes still work on CTL-1 Play Mode.

**Tips**
- Use one app or plugin per channel. You have 16 channels with 4 banks each.
- Use keys 21, 23, 25 (the last black keys) to quickly switch banks A, B, C. Key 25 will only toggle bank C & D which is useful if you only need 2 banks.
- Whenever you switch channels, it will go back to bank C as default. This is because bank A & B is mainly used by the CTL-1 keyboard, but you can still use it.
- To edit the channel names and colors, press Home > Global Settings > Use Select Knob to navigate to Channel Names. Use Preset Knob to select channel. Use Knob 1 as cursor, Knob 2 as character selector, Knob 3 as color selector.
- Since Logic 12, you can now control multiple instances of the same plugin and control them using different CC's. You will have to map each instances (e.g. first instance, second instance as needed). Use a different channel for each instance. That's why the default channel names have EQ 1, EQ 2, etc.
- I suggest using the default CC's so you won't lose all your Logic controller assignments if you have to re-flash the firmware. Remember to also save your Logic controller assignments to a file.

For the complete CTL-1 manual, please visit the [CTL-1 Github Page](https://github.com/charlesvestal/fm1-ctl)

## Credits

[CTL-1](https://github.com/charlesvestal/fm1-ctl) by Charles Vestal. 
The platform (hardware layer, USB, update loader, storage, installer, Bluetooth) is
[fm1-base](https://github.com/charlesvestal/fm1-base) on [felucca-bsp](https://github.com/hugelton/felucca-bsp),
from [Felucca](https://github.com/hugelton/Felucca) by Leo Kuroshita (Hügelton Instruments), by way of
[X0X](https://github.com/charlesvestal/fm1-x0x) and [FoMni-1](https://github.com/charlesvestal/fm1-fomni);
the top bar and knob row are X0X's. Bluetooth runs on JieLi's Bluetooth libraries (Apache-2.0). The General MIDI sounds are Sonivox EAS (Apache-2.0). The Euclidean
rhythms are Godfried Toussaint's reading of Bjorklund's algorithm. The idea of running FM-1 firmware in
the browser comes from [Groove OS](https://www.groove-os.com/). MPL-2.0; see [LICENSING.md](LICENSING.md).
