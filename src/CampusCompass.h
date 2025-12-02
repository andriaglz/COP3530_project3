#pragma once
#include <string>
#include <map>
#include <vector>
#include <set>
using namespace std;

struct Student {
    // string student_id;
    string student_name;
    string residence_location_id;
    set<string> class_codes;
};

struct Class {
    // ClassCode,LocationID,Start Time (HH:MM),End Time (HH:MM)
    // string class_code;
    string location_id;
    string start_time;
    string end_time;
    set<string> students_enrolled;  //set of student ids
};

class CampusCompass {
private:
    map<string,vector<pair<string,int>>> graph;     //adj list (nodes: locations | edges: walking time)
    map<string,Student> student_directory;          //student_id : Student struct
    map<string,Class> class_directory;              //class_code: Class struct
    map<string,string> locations;                   //location_id : location_name

public:
    CampusCompass();                                // constructor
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    bool ParseCommand(const string &command);

    // additional functions

    // command processing
    bool ProcessCommand(const string &command);
    bool Insert(string student_name,string student_id,string residence_id,set<string> class_codes);
    bool Remove(string student_id);
    bool DropClass(string student_id,string class_code);
    bool ReplaceClass(string student_id, string class_code_1, string class_code_2);
    bool RemoveClass(string class_code);
    int FindEdgeIndex(string from,string to);       //helper
    bool ToggleEdgesClosure(vector<pair<string,string>> location_pairs);
    bool CheckEdgeStatus(string location_1,string location_2);
    bool IsConnected(string location_1,string location_2);
    int ShortestPath(string start, string end);     //helper
    bool PrintShortestEdges(string student_id);
    bool PrintStudentZone(string student_id);
    bool VerifySchedule(string student_id);
};
