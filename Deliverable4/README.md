How to run:

T1: clang++ -std=c++17 predictiveImageCoding.cpp ../Deliverable2/Bitstream.cpp ./Golomb.cpp  -o predictiveImageCoding `pkg-config --cflags --libs opencv4`
T2: clang++ -std=c++17 predictiveVideoCoding.cpp ../Deliverable2/Bitstream.cpp ./Golomb.cpp  -o predictiveVideoCoding `pkg-config --cflags --libs opencv4`
T3: clang++ -std=c++17 interframeVideoCoding.cpp ../Deliverable2/Bitstream.cpp ./Golomb.cpp  -o interframeVideoCoding `pkg-config --cflags --libs opencv4`