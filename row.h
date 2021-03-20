#ifndef ROW_H
#define ROW_H
#include <iostream>
#include <vector>
using namespace std;

class Row {
    private:
        double type;                      //each row in the .txt file has a type (1 or 2)
        vector<double> features;          //and a list of features (exe. 10 for small dataset)

    public:
        Row();                            //constructor
        void append_feature(double);      //for file parsing
        void print();                     //pretty-print
        void set_type(double);            //setters and getters
        double get_type();
        vector<double> get_features();

};

#endif
