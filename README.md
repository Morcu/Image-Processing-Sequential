# Image-Processing-Sequential
Image processing (Histogram, Max and min, Mask, Rotation, Black Filter) sequential

An aplication to make image processing to Hexadecimal-based images
**There are 2 examples of images**


## Utilities

0. Black and White Histogram
1. Maximus and minimous
2. Mask aplication (**There are 2 examples of masks**)
3. Rotation
4. Black and White filter

## Compilation

**g++ -std=c++11 ARC_seq.cPP -o ARCfmut_seq**

## Execution 

**./ARCfmut_seq -u _NUMBER OF THE UTILITY_ - _FLAG OF THE UTILITY_ -i _INPUT IMAGE_ -o _OUTPUT FILE_

### Histogram

-t _Integer number_ (Number of sections of the histogram)

### Max and Min
**NO FLAG**

### Mask
-f _mask file_

### Rotation
-a _Decimal Number_ (Sexagesimal degrees in which the image is rotated.)

### Black and White filter

-r _Positive Decimal Number_ (Radius in pixels of the circle that selects the part which is not filtered.)

## Image composition 

This format encodes uncompressed images with RGB color space.
The specifications of the format are:
- The first four bytes specify the height of the image in pixels.
- The second four bytes specify the width of the image in pixels.
- All R (Red), all G (Green) and B (Blue) values are stored.
  Each value occupies an unsigned byte (possible values: 0 to 255)




