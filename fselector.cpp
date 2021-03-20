#include <iostream>
#include <cmath>
#include <vector>
#include "limits.h"
#include "row.h"
#include "fselector.h"
using namespace std;

double euclidean_distance(vector<double> test, vector<double> train, vector<int> index) {
    double dist = 0.0;
    for(int i = 0; i < index.size(); ++i) {
        dist += pow(test.at(index.at(i)) - train.at(index.at(i)), 2);
    }
    return dist;
}

//constructor
FSelector::FSelector(vector<Row> v) {
    data = v;
}

//returns the dataset size
int FSelector::data_size() {
    return data.size();
}

//input: feature sets
//returns: calculated %
//Based off of lecture slides pseudocode
double FSelector::nearest_neighbor(vector<int> v) {
    double accuracy = 0;
    double correct = 0;
    double total_count = 0;

    for(int i = 0; i < data.size(); ++i) {
        Row nearest_n;
        double s_dist = INT_MAX;

        for(int j = 0; j < data.size(); ++j) {
            if(i != j) {
              double temp = euclidean_distance(data.at(i).get_features(), data.at(j).get_features(), v);
              if(temp < s_dist) {
                  s_dist = temp;
                  nearest_n = data.at(j);
              }
            }
            else {
              continue;
            }
        }
        if(data.at(i).get_type() == nearest_n.get_type()) {
            correct++;
        }
        total_count++;
    }

    accuracy = correct/total_count;
    return accuracy;
}
