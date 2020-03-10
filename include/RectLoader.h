# pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <vector>
#include <memory>

using namespace std;

class Rect {
    private:
    unsigned int ID;                        // The indicator of a specific rectangle.
    double height;                          // The largest dimension of a rectangle.
    double width;                           // The smallest dimension of a rectangle.
    double area;                            // The area of a rectangle.

    shared_ptr<Rect> head_node;             // Store the first rectangle.
    shared_ptr<Rect> left_node;             // Use a tree data structure, point to left node.
    shared_ptr<Rect> down_node;             // Point to bottom node.

    bool used;                              // Determinante whether this node has been used.
    double x;                               // Store the coordinates of the upper right corner.
    double y;                               // x, y coordinates.

    public:
    // ************************************************************************************************************************
    // * Analytical and empirical results suggest that "first fit decreasing" is the best heuristic.
    // * Sort objects in decreasing order of size, so that the biggest object is first and the smallest last.
    // *
    // * There are variety of sorting algorithms, which are based on width or height or area or max. side.
    // * But subjectively, for the most pleasing square-ish results, the max. side choice is almost always the best choice.
    // *
    // * Insert each object one by one into the first bin that has room for it.

    // Default constructor: we always set the biggest nummber to height, so that the rectangle can be sorted based on max. side.
    Rect(double len_1, double len_2);

    /*
     * Some get function to access the private member.
     * Use share point to avoid memory leak.
    */
    // Return ID
    unsigned int getID() { return ID; };
    // Return height
    double getHeight() { return height; };
    // Return width
    double getWidth() { return width; };
    // Return area
    double getArea() { return area; };
    // Return head node
    shared_ptr<Rect> getHeadNode() { return head_node; };
    // Return left node
    shared_ptr<Rect> getLeftNode() { return left_node; };
    // Return underlying node
    shared_ptr<Rect> getUnderNode() { return down_node; };
    // Return whether the node has been used
    bool getUsed() { return used; };
    // Return x, y coordinates
    double getX() { return x; };
    double getY() { return y; };

    /*
     * Some set function to set up the private member.
    */
    // Set the ID
    void setID(unsigned int id) { this->ID = id; };
    // Set the height
    void setHeight(double height_) { this->height = height_; };
    // Set the width
    void setWidth(double width_) { this->width = width_; };
    // Set the head node
    void setHeadNode(shared_ptr<Rect> next) { this->head_node = next; };
    // Set the left node
    void setLeftNode(shared_ptr<Rect> next) { this->left_node = next; };
    // Set the underlying node
    void setUnderNode(shared_ptr<Rect> next) { this->down_node = next; };
    // Set the x, y-coordinates
    void setXY(double x_, double y_) { this->x = x_; this->y = y_; };
    // Set whether this node has been used
    void setUsed(bool used_) { this->used = used_; };

    /*
     * Display function to output the result on screen.
    */
    // Output the rectangle
    friend ostream& operator << (ostream& out, const Rect rec);

    ~Rect() {};
};  


class RectLoader {
    private:
    string filename;                         // The path and filename, Note: always store yours data with "rectangles.txt" and put it in "config" directory

    public:
    RectLoader(const string& filePath) { filename = filePath + "/rectangles.txt"; };

    vector<Rect> load();

    ~RectLoader() {};
};


