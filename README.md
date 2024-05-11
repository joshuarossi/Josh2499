# This is just for me to try and learn DCTL's

If you are looking for the actual source of all this stuff, check out
https://github.com/JuanPabloZambrano/DCTL/tree/main/2499_DRT

This is all his stuff, I am just playing around with it.

## My ideas

So, I am playing with the idea of splitting out the math of transforming between spaces, and trying to just focus on the preferential stuff. That being said, it is assumed that you will pass into these nodes XYZ/Linear. I think you can pass in any other color space you want to, and it will be passed out. You can chain any of these together, technically in any order, but I think they are intended to be used in the order listed.

TODOS:
I have to figure out what is going on with Rec709 inside some of the functions, and I am not sure if I will just keep using that or make it selectable, or pick one that I think works. That is the last open question.

## Operations

----------------- Decode -----------------------------

1. Convert to Davinci Wide Gamut Linear
2. Convert to XYZ Color Space
3. Convert to Rec709 color space
   ----------------- Step 1 -----------------------------
4. Calculate a Saturation Mask
5. Apply the Saturation Mask to the Rec709 image using a power function so it most affects the most saturated areas
   ----------------- Step 2 -----------------------------
6. Apply NonLinearGamutMapping
7. Apply an energy correction from the image before the NonLinearGamutMapping
   ----------------- Step 3 -----------------------------
8. Create a luminance Mask
9. Create 2 HueMatrixes to represent Hue rotations
10. Apply the HueMatrixes to the image, then mix that with the original image using the luminance mask
11. Apply the second HueMatrix to the image
12. Restore the energy using the image before the hue adjustments (similar to 7)
13. Clamp the values to avoid negatives
    ----------------- Step 4 -----------------------------
14. Apply the simpleInsetMatrix (line 548)
15. Clamp the values to avoid negatives
    ----------------- Step 5 -----------------------------
16. Apply the modifiedDanieleCurve
    ----------------- Step 6 -----------------------------
17. Apply the simpleInsetMatrix to provide preferential Purity
    ----------------- Encode -----------------------------
18. Scale the image using the wp_gains
19. Encode for display
