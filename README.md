# hdr2png

convert radiance file (.hdr) to png.
Radiance RGBE component are wrote "as-is" as RGBA png data


## build instruction

Build project using cmake.

First install dependencies

```
$ cd <hdr2png_project>
$ git submodule init
$ git submodule update
```

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

