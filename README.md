# This is just for me to try and learn DCTL's

If you are looking for the actual source of all this stuff, check out
https://github.com/JuanPabloZambrano/DCTL/tree/main/2499_DRT

This is all his stuff, I am just playing around with it.

## My ideas

So, I am playing with the idea of splitting out the math of transforming between spaces, and trying to just focus on the preferential stuff. That being said, it is assumed that you will pass into these nodes XYZ/Linear. I think you can pass in any other color space you want to, and it will be passed out. You can chain any of these together, technically in any order, but I think they are intended to be used in the order listed.

TODOS:
I have to figure out what is going on with Rec709 inside some of the functions, and I am not sure if I will just keep using that or make it selectable, or pick one that I think works. That is the last open question.
