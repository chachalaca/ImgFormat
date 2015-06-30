Image compression based on Huffman coding
====================
This program makes compression and decompression of PPM P3 image files to/from custom lossless format based on Huffman coding.
##Example usage
Suppose that *hufppm* is the compiled binary.
Compression:
```sh
hufppm encode blocks.ppm blocks.huf
```
Decompression:
```sh
hufppm decode blocks.huf decodedBlocks.ppm
```