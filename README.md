# hdr2png

convert radiance file (.hdr) to png.
Radiance RGBE component are wrote "as-is" as RGBA png data


## build instruction

Build project using cmake.

### OSX

```
$ cd <hdr2png_project>
$ mkdir _build
$ cd _build
$ cmake ../
$ make
```


## usage

```
$ hdr2png -i sources/tex.hdr -o bin/tex.png
```

