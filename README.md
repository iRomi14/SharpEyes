# SharpEyes
```
A brand new optical character recognizer in C99
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
$ python3 gen_trainset.py
$ make train
$ ./train {epochs} {number fonts}
```

* Xor Neural Network PoC

```
$ make xornn
$ ./xornn {epochs}
```

## Dependencies

* SDL2.0
* make
* gcc