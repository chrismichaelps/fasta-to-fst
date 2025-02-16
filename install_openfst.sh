#!/bin/bash

# --------------------------------------
# OpenFST Installation Script for M1/M2 Mac
# --------------------------------------

# Exit on error
set -e

# Step 4: Configure for Apple Silicon
echo "[4/6] Configuring for ARM64..."
./configure \
  --enable-shared \
  --enable-compact-fsts \
  --enable-const-fsts \
  --enable-far \
  --enable-lookahead-fsts \
  --enable-pdt \
  CXX="clang++ -arch arm64" \
  CXXFLAGS="-std=c++17 -stdlib=libc++"

# Step 5: Compile and install
echo "[5/6] Compiling (this may take 5-10 minutes)..."
make -j $(sysctl -n hw.ncpu)

echo "[6/6] Installing..."
sudo make install

# Set library path (for immediate use)
export DYLD_LIBRARY_PATH=/usr/local/lib:$DYLD_LIBRARY_PATH

echo "--------------------------------------"
echo "Installation complete! Verify with:"
echo "fstcompile --version"