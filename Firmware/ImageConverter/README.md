# Recolor

This is a quick GIF converter that will convert a standard 888 RGB GIF to a 565 RGB GIF.

This is needed due to GIF animator code that I found on the interwebs uses this type of image color.

I think it has to do with the limited color availibility that exists on the LCD screen which requires the shift from 24 bit color to 16 bit.




## Usage

Run the following:

```python
./recolor.py ./originalImages
```

A folder named `recolor` will be made in the current directory where the script is being ran from.