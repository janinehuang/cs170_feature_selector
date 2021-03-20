#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <fstream>
#include <chrono>
#include "limits.h"
#include "row.h"
#include "fselector.h"
using namespace std;

//https://levelup.gitconnected.com/8-ways-to-measure-execution-time-in-c-c-48634458d0f9
template <typename T>
void print_time(T begin) {
    // Stop measuring time and calculate the elapsed time
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    cout << endl;
    printf("Time measured: %.3f seconds.\n", elapsed.count() * 1e-9);
}

// reads the small or large data file and enters the entries by row into a vector of entries
vector<Row> parser(string data_file) {
    ifstream infile;
    vector<Row> v;

    infile.open(data_file.c_str());
    while(infile.good()) {
        double temp;
        double type = INT_MAX;
        Row instance;
        vector<double> features;
        string row;

        getline(infile, row);
        istringstream parser(row);
        parser >> type;

        if(type == INT_MAX) {
            break;
        }
        instance.set_type(type);
        while(parser >> temp) {
            instance.append_feature(temp);
        }
        //now contains all of the rows
        v.push_back(instance);
    }
    return v;
}

//returns if an index is in the vector of indexes to avoid checking twice
bool index_exists(int index, vector<int> v) {
  bool exists = false;
    for(int i = 0; i < v.size(); ++i) {
        if(index == v.at(i)) {
            exists = true;
            return exists;
        }
    }
    return exists;
}

//keeps track of the accuracy for each feature set tracked
struct feature_set {
    double accuracy;
    vector<int> feat_in;

    //increments the features by 1 (stored as indexes)
    const vector<int> inc_print(vector<int> x) const {
        vector<int> v = x;
        for(int i = 0; i < v.size(); ++i) {
            v.at(i) += 1;
        }
        return v;
    }

    //prints out feature set with accuracy %
    const void print() const {
        vector<int> v = inc_print(feat_in);
        ostringstream oss;

        if (!v.empty()) {
            copy(v.begin(), v.end() - 1, ostream_iterator<int>(oss, ","));
            oss << v.back();
        }
        cout << "using feature(s){" << oss.str() <<"} accuracy is: " << accuracy << "%" << endl;
    }

    //override < operator for < to compare accuracy attributes
    const bool operator < (const feature_set& rhs) const {
      return accuracy < rhs.accuracy;
    }
};

void forward_selection(FSelector fs, int size) {
    priority_queue<feature_set> prioq;                    //priority queue for the features set
    feature_set max;
    feature_set temp;
    vector<int> v;
    max.accuracy = 0;
    bool warning = true;

    cout << "This dataset has " << size << " features (not including the class attribute), with " << fs.data_size() << " instances." << endl << endl;
    //cout << "normalize?" << endl; <-- not needed
    cout << "Running nearest neighbor on all " << size << " features, using \"leaving-one-out\" evaluation..." << endl;
    temp.feat_in = v;
    temp.accuracy = fs.nearest_neighbor(v);
    cout << "I get an accuracy of " << temp.accuracy << "%." << endl << endl;
    cout << "Beginning search." << endl << endl;
    temp.print();

    for(int i = 0; i < size; ++i) {
        for(int j = 0; j < size; ++j) {
            feature_set set;
            vector<int> selected_features = v;
            if(index_exists(j, selected_features) == true) {               // found a feature that has been already added -- skip to next iter
                continue;
            }
            selected_features.push_back(j);                                //feature hasn't been added yet, add it to the selected features vector
            set.feat_in = selected_features;
            set.accuracy = fs.nearest_neighbor(selected_features);         //compute accuracy
            prioq.push(set);
        }
        temp = prioq.top();
        if(temp.accuracy > max.accuracy) {                                  //keep track of the feature set with highest %
            max = temp;
        }
        if(temp.accuracy && warning < max.accuracy) {
            warning = false;
            cout << endl << "Warning, accuracy has decreased! Continuing search in case of local maxima." << endl << endl;
        }
        temp.print();
        v = temp.feat_in;
        while(!prioq.empty()) {                                             //search is complete, clear the prioq
            prioq.pop();
        }
    }
    cout << "Finished search. " << endl;
    cout << "the best feature subset is ";
    max.print();

    return;
}


void backward_selection(FSelector fs, int size) {             //largely the same as forward_selection, but with a few modifications
    priority_queue<feature_set> prioq;
    feature_set max;
    feature_set temp;
    vector<int> v;
    bool warning = true;
    max.accuracy = 0;

    cout << "This dataset has " << size << " features with " << fs.data_size() << " Rows:" << endl << endl;

    for(int i = 0; i < size; ++i) {
        v.push_back(i);
    }
    temp.feat_in = v
    temp.accuracy = fs.nearest_neighbor(v);
    temp.print();

    for(int j = 0; j < size; ++j) {
        for(int i = 0; i < size; ++i) {
            feature_set set;
            vector<int> selected_features = v;
            if(index_exists(i, selected_features) == false) {
                continue;
            }
            selected_features.erase(remove(selected_features.begin(), selected_features.end(), i), selected_features.end());
            set.feat_in = selected_features;
            set.accuracy = fs.nearest_neighbor(selected_features);
            prioq.push(set);
        }
        temp = prioq.top();
        if(temp.accuracy > max.accuracy) {
            max = temp;
        }
        if(warning && temp.accuracy < max.accuracy) {
            warning = false;
            cout << endl << "Warning, accuracy has decreased! Continuing search in case of local maxima." << endl << endl;
        }
        temp.print();
        v = temp.feat_in;
        while(!prioq.empty()) {
            prioq.pop();
        }
    }
    cout << "Finished search. " << endl;
    cout << "the best feature subset is ";
    max.print();

    return;
}


int main (int argc, char*argv[]) {                          //main driver program
    if(argc == 3) {
      int input = 0;                                        //assuming program is run as a.out [arg1] [arg2]
      string trace_write(argv[1]);                          //implement printing for trace? TODO leave this for now
      string data_file(argv[2]);
      vector<Row> v = parser(data_file);

      FSelector fs(v);
      int size = v.at(0).get_features().size();

      cout << "Welcome to Janine Huang's Feature Selection Algorithm!" << endl;
      cout << "Select a selection algorithm: " << endl;
      cout << "\t1. Forward Selection" << endl;
      cout << "\t2. Backward Elimination" << endl;

      cout << "Enter your selection here: ";
      cin >> input;

      while (input != 1 && input != 2) {
        cout << "Invalid input. Please try again (enter 1 or 2): ";
        cin >> input;
      }

      if(input == 1) {
        cout << endl << "----------------------------------------" << endl;
        cout << "Forward Selection selected. beginning search ..." << endl;
        auto begin = std::chrono::high_resolution_clock::now();
        forward_selection(fs, size);
        print_time(begin);
      }
      else {
        cout << endl << "----------------------------------------" << endl;
        cout << "Backward Selection selected. beginning search ..." << endl;
        auto begin = std::chrono::high_resolution_clock::now();
        backward_selection(fs, size);
        print_time(begin);
      }
      return 0;
    }
    else {
      cout << "wrong number of arguments. please try again." << endl;
    }

    return 0;
}
