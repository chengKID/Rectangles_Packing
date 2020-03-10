#include "include/RectLoader.h"

using namespace std;

Rect::Rect(double len_1, double len_2) : 
    head_node(NULL), 
    left_node(NULL), 
    down_node(NULL), 
    used(false) {
    // Set the nummbers to hight and width.
    height = len_1;
    width = len_2;

    // Compute the area
    area = len_1 * len_2;
}

ostream& operator << (ostream& out, const Rect rec) {
    out << "Nr." << rec.ID << " height:" << rec.height << " width:" << rec.width << " area:" << rec.area << endl;
    return out;
}

vector<Rect> RectLoader::load() {
    // Output vector & identicator
    vector<Rect> rects;
    unsigned int id = 0;

    // Open file
    ifstream f;
    f.open(filename.c_str());

    // Check whether the txt. file is available
    if (!f.is_open()) {
        cerr << " can't open rectangles.txt!" << endl;
        return rects;
    }

    // Read in the parameters & Set them into rectangles
    while (!f.eof()) {
        string s;
        getline(f, s);

        if (!s.empty()) {
            stringstream ss;
            ss << s;

            double len_1, len_2;
            ss >> len_1;
            ss >> len_2;
            
            // Set the largest number to hight.
            if (len_1 >= len_2) {
                Rect data(len_1, len_2);
                data.setID(id);
                rects.push_back(data);
            }
            else {
                Rect data(len_2, len_1);
                data.setID(id);
                rects.push_back(data);
            }

            // The next idicator
            id++;
        }
    }
}