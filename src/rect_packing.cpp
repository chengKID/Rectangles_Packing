#include <bits/stdc++.h>
#include <math.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "include/RectLoader.h"

using namespace std;

// Overload compare operator for sorting algorithms
bool compareRects(Rect rec_1, Rect rec_2) { return rec_1.getHeight() > rec_2.getHeight(); }


// Find the correct regrion to place the rectangle & return a pointer to found region.
shared_ptr<Rect> findNode(shared_ptr<Rect> root, shared_ptr<Rect> curr) {
    if (root->getUsed()) {
        // We first try with the down-node.
        shared_ptr<Rect> node = findNode(root->getUnderNode(), curr);

        // If the region is found, return the pointer. Otherwise try with left-node.
        return node? node : findNode(root->getLeftNode(), curr);
    }
    else {
        // Based on the dimension to decide whether it can be inserted into this region.
        if (root->getHeight() >= curr->getHeight() && root->getWidth() >= curr->getWidth())
            return root;
        else
            return NULL;
    }
}


// Compute a layout for packing all rectangles inside one square
double packingRects(vector<Rect>& rectangles) {
    double mini_square = 0;

    // Sort rectangles in decreasing order of size, so that the biggest object is first and the smallest last.
    sort(rectangles.begin(), rectangles.end(), compareRects);

    /*
     * We start with a square, which ist just big enough for the first rectangle,
     * and then grow the target as needed whenever there is not enough froom for the next block.
     * 
     * We set the coordinates to the upper left corner of the square, and x-axis points to right & y-axis points down,
     * the same manner as opencv image.
    */
    // So, first initial the square with the largest rectangle.
    double size = rectangles[0].getHeight();
    Rect square(size, size);
    shared_ptr<Rect> root = square.getHeadNode();

    // Counter
    int i = 0;
    while (i < rectangles.size()) {
        // Set the first rectangle
        if (i == 0) {
            // Set up root
            double root_height = rectangles[i].getHeight();
            double root_width = rectangles[i].getWidth();
            root = make_shared<Rect> (Rect(root_height, root_width));
            root->setUsed(true);
            root->setXY(0.0, 0.0);
            rectangles[i].setXY(0.0, 0.0);

            // Set up left child node
            double left_width = size - rectangles[i].getWidth();
            root->setLeftNode(make_shared<Rect> (Rect(root_height, left_width)) );
            double left_x = root->getX() + root->getWidth();
            double left_y = root->getY();
            root->getLeftNode()->setXY(left_x, left_y);

            // Set up under child node
            double under_higth = size - rectangles[i].getHeight();
            root->setUnderNode(make_shared<Rect> (Rect(under_higth, size)) );
            double down_x = root->getX();
            double down_y = root->getY() + root->getHeight();
            root->getUnderNode()->setXY(down_x, down_y);

            i++;
            continue;
        }

        // Check whether the empty region of the square could cover the rectangle
        shared_ptr<Rect> node = findNode(root, make_shared<Rect> (rectangles[i]));
        if (node) {
            /*
             * Current rectangle can be inserted into free space
             * Based on current rectangle, set the head of the node
            */
            double root_higth = rectangles[i].getHeight();
            double root_width = rectangles[i].getWidth();
            node->setHeadNode(make_shared<Rect> (Rect(root_higth, root_width)) );
            node->getHeadNode()->setUsed(true);
            node->setUsed(true);
            
            // Store the position of the rectangle
            double head_x = node->getX();
            double head_y = node->getY();
            rectangles[i].setXY(head_x, head_y);
            node->getHeadNode()->setXY(head_x, head_y);

            // Based on current rectangle, split the left side of the node
            double left_higth = rectangles[i].getHeight();
            double left_width = node->getWidth() - rectangles[i].getWidth();
            shared_ptr<Rect> left_node(new Rect(left_higth, left_width));
            double left_x = node->getX() + rectangles[i].getWidth();
            double left_y = node->getY();
            left_node->setXY(left_x, left_y);
            node->setLeftNode(left_node);

            // Based on current rectangle, split the under side of the node
            double down_higth = node->getHeight() - rectangles[i].getHeight();
            double down_width = node->getWidth();
            shared_ptr<Rect> down_node(new Rect(down_higth, down_width));
            double down_x = node->getX();
            double down_y = node->getY() + rectangles[i].getHeight();
            down_node->setXY(down_x, down_y);
            node->setUnderNode(down_node);
        }
        else {
            /* 
             * Current rectangle can't be inserted into free space
             * We should increase the square & make sure that increase square as small as possible
            */
            // Find the length, when set the rectangle to the left
            shared_ptr<Rect> tmp_left = root->getLeftNode();
            while (tmp_left->getLeftNode()) {
                tmp_left = tmp_left->getLeftNode();
            }
            double left_increasing = tmp_left->getX() + tmp_left->getWidth() + rectangles[i].getWidth();

            // Find the length, when set the rectangle to the bottom
            shared_ptr<Rect> tmp_down = root->getUnderNode();
            while (tmp_down->getUnderNode()) {
                tmp_down = tmp_down->getUnderNode();
            }
            double down_increase = tmp_down->getY() + tmp_down->getHeight() + rectangles[i].getHeight();

            // Choice the smallest one as new size of the square
            size = left_increasing <= down_increase? left_increasing : down_increase;
            square.setHeight(size);
            square.setWidth(size);

            // Because the size has been changed, so we shoul rollback and redo from the begin
            i = -1;
        }

        // Try with next rectangle
        i++;
    }

    return size;
}

int main(int argc, char* argv[]) {
    // Load the rectangles.
    string config_path = argv[1];
    RectLoader read_rec(config_path);
    vector<Rect> rects = read_rec.load();

    // Packing the input rectangles.
    double sq_size = packingRects(rects);

    // Record the area of all rectangles.
    double sum = 0.0;

    // Test the load function
    for (auto it : rects) {
        cout << it << endl;
        sum += it.getArea();
    }
    
    // Print the size of minimum square
    cout << "The size of minimum square:" << sq_size << endl;

    // Compute the filled rate of the square
    cout << "The area of square is:" << (double) sq_size*sq_size << " and the area of all rectangles is:" << sum 
         << ". The filled rate is:" << (double) sum/(sq_size*sq_size)*100 << "%" << endl;

    /*
     * Show the packing result in image
     * The outside contour is the square
     * Free spaces are painted with white
     * Each rectangle is colored with blue.
    */
    // Some result may be too small, so convert the image to const. size: 620x620
    double scalar = 620.0 / sq_size;
    cv::Mat img = cv::Mat::zeros(620, 620, CV_8UC3);

    for (auto it : rects) {
        double x1 = scalar * it.getX();
        double y1 = scalar * it.getY();
        double x2 = scalar * (it.getX() + it.getWidth());
        double y2 = scalar * (it.getY() + it.getHeight());
        // Plus 1 to avoid 0
        //float color = 255.0 * (it.getID() + 1) / rectangles.size() + it.getID();
        cv::rectangle(img, cv::Point2d(x1, y1), cv::Point2d(x2, y2), 
                      cv::Scalar(255, 255, 0));
    }

    cv::namedWindow("Rectangles Packing Result", cv::WINDOW_AUTOSIZE);
    cv::imshow("Rectangles Packing Result", img);
    cv::waitKey(0);

    return 0;
}