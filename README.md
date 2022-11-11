# RBRCountdown

This is a plugin for Richard Burns Rally that adds a countdown before the stage starts.

Big thanks to [mika-n](https://github.com/mika-n) that made the [NGPCarMenu](https://github.com/mika-n/NGPCarMenu) plugin that this plugin is based on. If it wasn't for NGPCarMenu I would never have been able to make this plugin. Big huge shoutout!!

Please also see more credits [here](https://github.com/HanaMcHanaface/RBRCountdown/blob/main/NGPCarMenu_LicenseText.txt) and [here](https://github.com/HanaMcHanaface/RBRCountdown/blob/main/NGPCarMenu_LicenseText_3rdPartyTools.txt).

![example countdown gif](https://raw.githubusercontent.com/HanaMcHanaface/RBRCountdown/main/example.gif)

## Installation

Download the latest version here: [Download](https://github.com/HanaMcHanaface/RBRCountdown/releases/download/2.2.0.0/RBRCountdownV2.2.0.0.zip)

To install, first find your RBR installation folder. Then, go to the `Plugins` folder. Open the zip file you downloaded and move all files there to the Plugins folder. That's it!!

## Configuration

In `Plugins\countdown` you will find a file called `RBRCountdown.ini`. In this file you will find the settings:

* `Centered`, True or False. If True the countdown will be centered on the screen.
* `XOffset`, Horizontal offset of the countdown. Only whole numbers. If Centered is True then the offset is from the center. Otherwise it is from the top left corner of the screen.
* `YOffset`, Same as XOffset but vertical. Up is negative.
* `Scale`, Scales the countdown. Decimal number.

## Uninstall

In the Plugins folder, remove `RBRCountdown.dll` and the `countdown` folder.

## Building

If you build debug it will probably fail because it tries to copy the DLL to the RBR plugins folder. Check `Build Events->Post-Build Events` in the project settings. You also need the `countdown` folder in the Plugins folder. Copy it from the latest build is easiest. The textures are created from `textures\countdown.psd` by doing export layers to files, and `textures\countdown-dot.psd` by quick export PNG.
