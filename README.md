# Huffman Coding
This project uses the Huffman Coding algorithm to compress and decompress large datafiles.

# Huffman Coding Algorithm
This algorithm compresses symbols by calculating the frequency each symbol appears in the file, and using those counts to build a tree. After that, codes are also assigned to each of the leaves, which are then used to encode the file. On the other hand, the decoder reads each bit of the encoded file and determines which symbol to write after landing on a leaf during the code walk. The path taken to walk down the tree while reading the code determines the symbol that is written to the decoded output.

# To Build the Encoder and Decoder
make/make all

# To Build Just the Encoder
make encode

# To Build Just the Decoder
make decode

# To Run the Encoder
./encode -p -v -i infile -o outfile (p, v, i, and o are 4 optional command-line arguments)
-p specifies that you want to print out the usage description
-v specifies that you want to print out compression statistics
-i requires you to specify the infile to which you want to encode (if none specified, then it reads from the standard input)
-o requires you to specify the outfile to which you want to write the encoded output to (if none specified, then it writes to the console/standard output)

# To Run the Decoder
./decode -p -v -i infile -o outfile (p, v, i, and o are 4 optional command-line arguments)
-p specifies that you want to print out the usage description
-v specifies that you want to print out compression statistics
-i requires you to specify the infile to which you want to read the encoded data from (if none specified, then it reads from the standard input)
-o requires you to specify the outfile to which you want to write the decoded output to (if none specified, then it writes to the console/standard output)

# Clean the Directory
make clean (removes any object file/executable from the directory)
