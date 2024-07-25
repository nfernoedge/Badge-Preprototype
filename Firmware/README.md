# VETCON Badge Code



This is the code for the 2024 VETCON Badge.

Functions:

1. GIF Player
2. Puzzle for URLs
3. About Screen

## Compiling

Just load the project in PlatformIO and VSCode.

Click the upload button... I should work.

If it can't connect to the board, then try placing the board into boot mode manually:

1. Hold down the button with a `B` next to it.
2. Hold down the button with a `R` next to it.
3. Release the button with a `R` next to it.
4. Release the button with a `B` next to it.
5. Upload the code
6. Momentarily press the button with a `R` next to it.

## Images for the SD Card

The GIFs must be in 565RGB Format. See the `ImageConverter` directory for a script.

The GIFs must be in 160x128 format.

The nameing convention for the gif file should be: `<ServiceName>##.gif`

Example: `coastguard10.gif`

Service Names for when naming files:

* `coastguard`
* `airforce`
* `navy`
* `marine`
* `army`
* `spaceforce`