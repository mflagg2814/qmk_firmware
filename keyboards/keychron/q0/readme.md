# Keychron Q0

![Keychron Q0](https://i.imgur.com/cLbEiZ0h.jpg)

A customizable number keypad.

* Keyboard Maintainer: [Keychron](https://github.com/keychron)
* Hardware Supported: Keychron Q0
* Hardware Availability: [Keychron Q0 QMK Custom Number Pad](https://www.keychron.com/products/keychron-q0-qmk-custom-number-pad)

Make example for this keyboard (after setting up your build environment):

    >> This one: qmk compile -kb keychron/q0/base -km keychron
	Flash: Hold esc while plugging in. QK_BOOTLOADER seems unreliable? It flashes but doesn't take, even with reboot

	
	
    make keychron/q0/plus:default

Flashing example for this keyboard:

    make keychron/q0/base:default:flash
    make keychron/q0/plus:default:flash

**Reset Key**: Hold down the key located at *K00*, commonly programmed as *Esc* while plugging in the keyboard.

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).
