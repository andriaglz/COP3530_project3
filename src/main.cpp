#include <iostream>

#include "CampusCompass.h"
using namespace std;

int main() {
    // initialize your main project object
    CampusCompass compass;
    bool success;

    // ingest CSV data
    success = compass.ParseCSV("../data/edges.csv", "../data/classes.csv");
    if (!success) {
        cout << "Error parsing csv files" << endl;
        return 0;
    }

    // parse commandline input
    int no_of_lines;
    string command;
    cin >> no_of_lines;
    cin.ignore(); // ignore newline that first cin left over

    for (int i = 0; i < no_of_lines; i++) {
        getline(cin, command);

        // parse your commands and validate input
        success = compass.ParseCommand(command);
        if (!success){
            cout << "unsuccessful" << endl;
            continue;
        }

        // process validated command
        compass.ProcessCommand(command);
    }

    return 0;
}
