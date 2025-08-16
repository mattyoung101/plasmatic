# Plasmatic
**UQCS Hackathon 2025**

by Matt Young

as Team "The Opps Took My Protected Mode Addressing, Can't Have Nothing in St Lucia"

## Goal
The goal is to make a plasma demo for MS-DOS (technically FreeDOS) in 2.5 days using a PC in a cardboard box.

If that works out, I'm also going to try write a software renderer and spin some text around. _(update: this did not
work out)_

## Purpose
The main purpose is to have a bit of fun and try out DOS programming, which is something I've always wanted to
try. It's hard work, but a lot of fun!

The secondary purpose of this project is to sort of act as the antithesis to what modern operating systems
and user experiences have come: slow, privacy-invading messes riddled with ads and malware. Comparatively,
FreeDOS boots in under a second, and simply lets you poke VGA registers and make pretty demos!

In addition, since all of the PC components I used are salvaged from an actual skip bin or second hand, I'd
like to think of this as a statement against e-waste and pro-upcycling. It proves that "junk" can live on in
another life, and that you don't need an M3 Mac and a paid Copilot subscription to have fun and make art at a
hackathon.

## References used
- OpenWatcom manuals
    - https://open-watcom.github.io/open-watcom-v2-wikidocs/clib.html
    - https://open-watcom.github.io/open-watcom-v2-wikidocs/ctools.pdf
- How to Program FreeDOS in Graphics Mode: https://www.youtube.com/watch?v=uhxKXdZKCeM
- Let's Code a Plasma Demo: https://www.youtube.com/watch?v=X6BpW2N_Sf0
    - only used for VGA blitting routine inspo, plasma not copied directly :)
- Setting up OpenWatcom: http://nuclear.mutantstargoat.com/articles/retrocoding/dos01-setup/#openwatcom-setup
- Perlin noise implementation: https://github.com/czinn/perlin
- Blender text: https://www.wikihow.com/Make-3D-Text-with-Blender
- obj2header: https://github.com/ichlubna/obj2header
- Bayer dither: https://stackoverflow.com/a/68192472/5007892
- Dither matrices: http://www.edenwaith.com/blog/index.php?p=157 and
https://en.wikipedia.org/wiki/Ordered_dithering#Threshold_map
- TODO sine approx

AI use was minimal, I was planning entirely not to at first, but ran out of time and got extremely lazy. Sorry :sob:
Alas, AI was used to generate some of the scripts in the "old_crap" directory, the text mask to C array
script, and the util function to lookup a bit in an array of uint64s. I also used it to assist with Bayer
dithering because applying the dither matrix didn't make sense to me at first (it turned out I was doing the
quantizing wrong).

