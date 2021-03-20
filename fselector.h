#ifndef FSELECTOR_H
#define FSELECTOR_H

#include <iostream>
#include <vector>
#include "row.h"
using namespace std;

class FSelector {
    private:
        vector<Row> data;                           //holds the dataset in this vector (after parsing

    public:
        FSelector(vector<Row>);                     //constructor
        int data_size();                            //returns size of the data for printing to console later
        double nearest_neighbor(vector<int>);       //returns % based on features used, algorithm from slides
};

#endif
