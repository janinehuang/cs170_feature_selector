#include <iostream>
#include "row.h"

using namespace std;

//------constructor--------
Row::Row() {
    type = 0;
}

void Row::append_feature(double f) {
    features.push_back(f);
    return;
}

void Row::print() {
    cout << "type: " << type << endl;
    for(int i = 0; i < features.size(); ++i) {
        cout << features.at(i) << " ";
    }
    cout << endl;
    return;
}

//--------setter and getter functions---------
void Row::set_type(double i) {
    type = i;
    return;
}

double Row::get_type() {
    return type;
}

vector<double> Row::get_features() {
    return features;
}
