#pragma once
#include <string>
#include <map>
#include <vector>
using namespace std;

struct Student {
    string student_id;
    string student_name;
    string residence_location_id;
    int num_classes;
    vector<string> class_codes;
};

struct Class {
    // ClassCode,LocationID,Start Time (HH:MM),End Time (HH:MM)
    string class_code;
    string location_id;
    string start_time;
    string end_time;
    int num_students_enrolled;
};

class CampusCompass {
private:
    map<string,vector<pair<string,int>>> graph;     //adj list
    map<string,Student> student_directory;          //student_id : Student struct
    map<string,Class> class_directory;              //class_code: Class struct
    map<string,string> locations;                   //location_id : location_name

public:
    CampusCompass();                                // constructor
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    bool ParseCommand(const string &command);
};
