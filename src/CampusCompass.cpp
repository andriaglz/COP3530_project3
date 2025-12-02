#include "CampusCompass.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

CampusCompass::CampusCompass() {
    // constructor
    ;
}

bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath) {
    // open csv files
    ifstream edges_file(edges_filepath);
    ifstream classes_file(classes_filepath);

    if (!edges_file.is_open() || !classes_file.is_open())
        return false;

    // edges file
    string line;
    // header line
    // LocationID_1,LocationID_2,Name_1,Name_2,Time
    getline(edges_file, line);
    // data lines
    while (getline(edges_file, line)){
        // parse each column in the line
        stringstream ss(line);
        string id_1,id_2,name_1,name_2,time;
        getline(ss,id_1,',');
        getline(ss,id_2,',');
        getline(ss,name_1,',');
        getline(ss,name_2,',');
        getline(ss,time,',');

        if (!is_integer(time)){
            cout << "Error: Incorrectly parsed time" << endl;
            return false;
        }

        // initialize values in locations
        if (locations.find(id_1) == locations.end())
            locations[id_1] = name_1;
        if (locations.find(id_2) == locations.end())
            locations[id_2] = name_2;
        
        // add edge to graph
        graph[id_1].push_back(make_pair(id_2,stoi(time)));
    }

    // classes file
    // header line
    // ClassCode,LocationID,Start Time (HH:MM),End Time (HH:MM)
    getline(classes_file, line);
    // data lines
    while (getline(classes_file, line)){
        // parse each column in the line and initialize Class struct
        stringstream ss(line);
        Class class_info;
        string class_code;

        getline(ss,class_code,',');
        getline(ss,class_info.location_id,',');
        getline(ss,class_info.start_time,',');
        getline(ss,class_info.end_time,',');
        class_info.num_students_enrolled = 0;

        // validate time formatting
        if (!(class_info.start_time[2]==':') || !(class_info.end_time[2]==':')){
            cout << "Error: Incorrectly parsed start and end time" << endl;
            return false;
        }

        // add class to class directory
        class_directory[class_code] = class_info;
    }
    // return boolean based on whether parsing was successful or not
    return true;
}

bool CampusCompass::ParseCommand(const string &command) {
    // verify that the keyword for the command is valid
    vector<string> valid_keywords = {"insert","remove",
                                     "dropClass","replaceClass","removeClass",
                                     "toggleEdgesClosure","checkEdgeStatus","isConnected",
                                     "printShortestEdges","printStudentZone","verifySchedule"};
    istringstream input_stream(command);
    string keyword;
    input_stream >> keyword;
    bool keyword_is_valid = is_in(keyword,valid_keywords);
    if (!keyword_is_valid)
        return false;

    // input validation for each individual command
    string argument_1;
    if (!(input_stream >> argument_1))
        // all commands have at least 1 argument
        return false;

    // insert
    if (keyword == "insert"){
        string name,id,residence_location_id,n_str;
        name = argument_1;

        // verify number of arguments
        if(!(input_stream>>id>>residence_location_id>>n_str))
            // id,residence_location_id, and n must be provided
            return false;
        // verify n
        int n_int;
        vector<string> class_codes;
        if (is_integer(n_str))
            n_int = stoi(n_str);
        else
            return false;
        for (int i; i<n_int; i++){
            string class_code;
            if (!(input_stream >> class_code))
                // n class codes must be provided
                return false;
            class_codes.push_back(class_code);
        }

        // verify input contents
        // name
        if (name[0] != '"' && name[name.size()-1] != '"')
            return false;
        // id
        if (!(student_directory.find(id) == student_directory.end()))
            // student id must be unique
            return false;
        // residence_location_id
        if (locations.find(residence_location_id) == locations.end())
            // residence id must be present in records
            return false;
        // class codes
        for (string class_code : class_codes){
            if (class_directory.find(class_code) == class_directory.end())
                // course code must be present in records
                return false;
        }
    }

    // student id commands
    if (is_in(keyword,{"remove","dropClass","replaceClass"})){
        // validate student id
        if (student_directory.find(argument_1) == student_directory.end())
            // student id must be present in the directory
            return false;
        
        // verify specifics
        // remove
        if (keyword == "remove")
            return true;
        
        // dropClass or replaceClass
        string class_code;
        input_stream >> class_code;
        if (class_directory.find(class_code) == class_directory.end())
            // course code must be present in records
            return false;

        // dropClass
        if (keyword == "dropClass"){
            return true;
        }

        // replaceClass
        string class_code_2;
        input_stream >> class_code_2;
        if (class_directory.find(class_code_2) == class_directory.end())
            // course code must be present in records
            return false;
    }
    // removeClass
    if (keyword == "removeClass"){
        if (student_directory.find(argument_1) == student_directory.end())
            // student id must be present in the directory
            return false;
    }

    // toggleEdgesClosure
    if (keyword == "toggleEdgesClosure"){
        if (!is_integer(argument_1))
            return false;
        int n = stoi(argument_1);

        // check all n pairs
        for (int i=0;i<n;i++){
            for (int j=0;j<2;j++){
                string location;
                if (!(input_stream>>location))
                    return false;
                if (locations.find(location) == locations.end())
                    // location must be present in records
                    return false;
            }
        }
    }

    // two location commands
    if (is_in(keyword,{"checkEdgeStatus","isConnected"})){
        vector<string> location_ids;
        location_ids.push_back(argument_1);
        string location_2;
        input_stream >> location_2;
        for (string location : location_ids){
            if (locations.find(location) == locations.end())
                // location must be present in records
                return false;
        }
    }

    // id commands
    if (is_in(keyword,{"printShortestEdges","printStudentZone","verifySchedule"})){
        // validate student id
        if (student_directory.find(argument_1) == student_directory.end())
            // student id must be present in the directory
            return false;
    }

    return true;
}

// helper functions for ParseCommand
bool is_in(string target_item, vector<string> items){
    for (string item : items){
        if (item == target_item){
            return true;
        }
    }
    return false;
}

bool valid_student_id(string id){
    return true;
}

bool is_integer(string str){
    try {
        int int_version = stoi(str);
        return true;
    } catch (exception e) {
        return false;
    }
}
