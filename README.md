# SharpEyes
```
A brand new optical character recognizer in C99 made by SharpEyes's team.
```

## Information

An OCR made for Epita S3 by a team of 4 students named __SharpEyes__

## How to use

* Open and Process an image

```
$ make
$ ./main
```

* Train the OCR Neural Netword

```
$ make train
$ ./train {epochs}
```

* Xor Neural Network PoC

```
$ make xornn
$ ./xornn {epochs}
```

## Dependencies

* GTK 3+
* SDL 2.0
* make
* gcc
