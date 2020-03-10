# Implement a simple Rectangles Packing Algorithm

## Problem: Find a square to cover all the given rectangles and the square should be as small as possible

**Bin Packing Problem & Idea**:
It is a 2D Bin Packing Problem. So it's a NP-hard problem and there is no absolut answer. Hier the idea is the same as "packing lightmaps". We do it recursively by creating a smallest square, which can cover the first rectangle. Then we recursively divide the larger texture into three child nodes, i.e. one for filled region called head node, ohter two for empty regions called left node and down node. Then we insert the next rectangle and hope to find an empty region, which can cover this rectangle. If there is an empty region, we insert the rectangle into this region and divide this region into three child nodes as the same manner. If there is not one, we should increase the square, but we should make sure that the increased square is as small as possible. Then we repeat all of these for the rest of rectangles.

**Tree Data Structure**:
Hier we use the Tree to store data. So the root is the outside square and it has three child nodes, i.e. head-node on the up-left corner, left-node on the left and down-node on the bottom. Each node will also be divided into these there nodes, if a rectangle can be insert into this node, where head-node contains the rectangle, and left-node & down-node are free region. 

Note: this is not the best way to solve the 2D Bin Packing Problem. There is a lot of empty region, you can find it in the displayed image or there is a "filled rate" displayed on the screen. There must be other way, which are more optimal, but in the limited time, I could only implement this algorithm and for some cases, it works fine.

**Heuristics & Sorting Algorithm**:
2D Bin Packing Problem is NP-complete. Thus we are doomed to think in terms of heuristics instead of worst-case optimal algorithms. So "Analytical and empirical results suggest that 'first fit decreasing' is the best heuristic. Sort the objects in decreasing order of size, so that the biggest object is first and the smallest last. Insert each object one by one into the first bin that has room for it, otherwise increase the square."

There are a lot ways to define "size", such as with height, or with width, or with area. Subjectively, for the most pleasing suqare-ish results, the maxside (i.e. max(width, height)) choice is almost always the best choice. It also seems to almost always produce the most effective packing results.

## Installation & Required Dependencies
We use Linux Ubuntu 16.04 and C++

This program uses the OpenCV library to dispaly the result. So please install with

		sudo apt-get install libopencv-dev


The link here is very helpful for understanding:

paper: https://www.codeproject.com/script/Articles/ViewDownloads.aspx?aid=210979

## Build the Project
```bash
$> mkdir build && cd build
$> cmake ..
$> make
$> ./bin/rects_packing "pwd"/config
```
Note: always store the size of input rectangles in "rectangles.txt" file. This file is in "/config" directory.

## Displayed Image
The outside of image is the square. All rectagles are colored blue. Other area mean empty region (black space).
