# ann
Artificial Neural Network in C++ based on the video tutorial by David Miller:

[http://www.millermattson.com/dave/?p=54](http://www.millermattson.com/dave/?p=54)

# How to
Download or clone the repository. Then simply run `make` to compile it:

```
make
```

You can run `./ann -h` to show the usage and a list of the options:

```
Usage: ./ann -n topology -t training-file [-a alpha] [-e eta] [-i input-file] [-d delimiter]
Description:
	-n topology:      the topology of the network (example: 3;2;1)
	-t training-file: file with training data
	-a alpha:         the overall training rate (default: 0.5)
	-e eta:           multiplier of last weight change (default: 0.15)
	-i input-file:    the source of input data (default: stdin)
	-d delimiter:     the delimiter of input data (default: space)
	-q quiet:         silent output
```

For example, to build a neural network capable of calculating an XOR function:

```
$ cat xor-values.txt | ./ann -n 2,3,1 -d "," -t xor-training.txt -q
0,0: -0.0714217
0,1: 0.997609
1,0: 0.997613
1,1: -0.00394615
```
