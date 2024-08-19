"""
Created by: James Campbell
Last Updated: 22JUL24
"""

from PIL import Image
import numpy as np
import sys
def convert_to_rgb565(image):
    # Convert the image to RGB format
    image_rgb = image.convert('RGB')
    # Get the image data
    img_data = np.array(image_rgb)

    # Perform the conversion to RGB565
    r = (img_data[:, :, 0] >> 3).astype(np.uint16)
    g = (img_data[:, :, 1] >> 2).astype(np.uint16)
    b = (img_data[:, :, 2] >> 3).astype(np.uint16)

    rgb565 = (r << 11) | (g << 5) | b
    return rgb565

def convert_rgb565_to_image(rgb565, width, height):
    r = ((rgb565 >> 11) & 0x1F) << 3
    g = ((rgb565 >> 5) & 0x3F) << 2
    b = (rgb565 & 0x1F) << 3

    rgb888 = np.stack((r, g, b), axis=-1).astype(np.uint8)
    return Image.fromarray(rgb888, 'RGB')


def convertimg(origDir, imgName):
    # os.mkdir("recolor")
    # Load the GIF image
    image = Image.open(f'{origDir}/{imgName}')

    # Convert each frame of the GIF
    frames = []
    width, height = image.size
    try:
        while True:
            # Convert the current frame to RGB565
            frame_rgb565 = convert_to_rgb565(image)
            # Convert RGB565 back to image for saving
            frame_image = convert_rgb565_to_image(frame_rgb565, width, height)
            frames.append(frame_image)
            image.seek(image.tell() + 1)
    except EOFError:
        pass

    # Save the frames as a new GIF
    frames[0].save(f'recolor/{imgName}', save_all=True, append_images=frames[1:], loop=0)

import os

# Specify the directory and the substring to remove
directory = sys.argv[1]
# Loop through all files in the directory
for filename in os.listdir(directory):
    if('.gif' in filename):
        convertimg(directory, filename)

print('Done renaming files.')