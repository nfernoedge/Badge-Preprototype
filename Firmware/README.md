# 2024 VETCON Badge Assembly Instructions



This is the code for the 2024 VETCON Badge.

Functions:

1. GIF Player
2. Puzzle
3. About Screen

## Step 1: Compile and Flash

1. Install Visual Studio Code on your computer (free)
2. Open Visual Studio Code and install the PlatformIO plugin
![image](https://github.com/user-attachments/assets/01458e41-9ad7-4f2a-a918-1484ab1f5019)

3. Download the firmware from the Badge-Preprototype page as a .zip
![image](https://github.com/user-attachments/assets/f9c7940c-5e46-4893-8ce6-20c0fb7a7248)

4. Extract all
5. In Visual Studio Code, select the PlatformIO icon and hit the "Open Project" button
![image](https://github.com/user-attachments/assets/b203753b-07ee-4740-a2c7-501e758477a4)
6. Open platformio.ini in the Firware folder
7. Plug the badge's esp32 in to your computer via usb port
8. Hit the build button (checkmark icon)
![image](https://github.com/user-attachments/assets/04bb9985-9fb0-4736-979d-6df2109658b8)

9. Hit the upload button (right arrow icon), your badge should now flash
![image](https://github.com/user-attachments/assets/f357828b-7623-410c-9d59-cb33a187adee)

10. Plug in your SD card to your computer
11. IF NEEDED: format the card FAT32
12. Copy all the GIFs from the "recolor" folder to the root directory of the SD card.  Do NOT create a folder on the card.
13. Insert your SD card into the badge behind the LCD screen.  Make sure you push the card all the way in, the top of the SD card should be roughly flush with the top of the "2024" text.

## Step 2: Solder the power pads
1. On the back side of the badge, there are 2 empty holes under the ESP32 chip.  If you look inside, you should be able to see the two small power pads on the inside of each hole.
![image](https://github.com/user-attachments/assets/eacb89b8-361e-44ed-95ed-ffb2cc8128af)

3. CAREFULLY solder these pads to the holes.  This is tricky, go slow and make sure the pad gets hot.  You need to make sure the solder gets onto the pad at the bottom, don't just fill the top of the hole with the solder.

4. Insert the battery (the bump goes on the right as you're looking at the battery holder, provided you're not holding it upside-down...)

5. It should power on.  You're done!

## Troubleshooting

If your computer can't connect to the board, then try placing the board into boot mode manually using the VERY small buttons on either side of the USB plug:

1. Hold down the button with a `B` next to it.
2. Hold down the button with a `R` next to it.
3. Release the button with a `R` next to it.
4. Release the button with a `B` next to it.
5. Tell PlatformIO to upload the code
6. Momentarily press the button with a `R` next to it.

## Images for the SD Card

If you want, you can upload your own GIFs to the badge, but you need to format the GIF first.

The GIFs must be in 565RGB Format. See the `ImageConverter` directory for a script.

The GIFs must be in 160x128 format.  There are free resizing tools available online.

The naming convention for the gif file should be: `<ServiceName>##.gif`

Example: `coastguard10.gif`

Service Names for when naming files:

* `coastguard`
* `airforce`
* `navy`
* `marine`
* `army`
* `spaceforce`

You must use the next available number (if you see navy01 through navy10 already, name yours "navy11")
