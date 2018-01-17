My implementation/walkthrough of this huge SDL tutorial.

I type this shit.

[lazyfoo SDL tutorial](http://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php)

You'll need for lesson6
* `sudo apt-get install libsdl2-image`
* `sudo apt-get install libsdl2-image-dev`

And for lesson16
* `sudo apt-get install libsdl2-ttf-2.0-0 libsdl2-ttf-dev`

Some other useful linker flags, also managed by apt
* `-lSDL2_image`
* `-lSDL2_ttf`
* `-lSDL2_mixer`

** Note on loading PNGs**
If you get a warning
```
libpng warning: iCCP: known incorrect sRGB profile
```

Newer versions of libpng (1.6) don't want that partial iCCP data in your PNGs any more. If
you have ImageMagick installed on your system do `mogrify your-file.png` to remove
the blank chunk.
