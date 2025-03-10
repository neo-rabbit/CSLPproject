# CSLPproject

How to run:

    g++ encoder.cpp BitStream.cpp -o encoder (or clang++ -std=c++11 encoder.cpp Bitstream.cpp -o encoder for macOS arm64)
    ./encoder <input_file> <output_file>

    g++ decoder.cpp BitStream.cpp -o decoder (or clang++ -std=c++11 decoder.cpp Bitstream.cpp -o decoder for macOS arm64)
    ./decoder <input_file> <output_file>