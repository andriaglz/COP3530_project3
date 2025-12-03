#include "CampusCompass.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stack>
using namespace std;

const int INF = 1000000000;

// helper functions
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
        stoi(str);
        return true;
    } catch (exception e) {
        return false;
    }
}


// CampusCompass functions
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
        
        // add edge to graph in both directions
        graph[id_1].push_back(make_pair(id_2,stoi(time)));
        graph[id_2].push_back(make_pair(id_1,stoi(time)));
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

        // cout << class_code << ", " << class_info.location_id << ", " << class_info.start_time << ", " << class_info.end_time << endl;

        // validate time formatting
        if (class_info.start_time[2]!=':' || class_info.end_time[2]!=':'){
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
        for (int i=0; i<n_int; i++){
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
        if (class_directory.find(argument_1) == class_directory.end())
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

// command processing
bool CampusCompass::Insert(string student_name,string student_id,string residence_id,set<string> class_codes){
    /*
    Adds a student to the class(es) with the specified name: STUDENT_NAME, STUDENT_ID,   
        RESIDENCE_LOCATION_ID, and N classes with Class Codes: CLASSCODE_1 CLASSCODE_2  … CLASSCODE_N
    The STUDENT_ID must be unique.
    The STUDENT_ID and STUDENT_NAME must satisfy the constraints stated below.
    There must be N classes after reading N; otherwise, the entry is invalid.
    Also, prints “successful” if insertion is successful and prints “unsuccessful” otherwise.
    NAME identifier will be separated by quotes for parsing, e.g., "Josh Smith."
    CLASSCODE identifiers will always be a single word, e.g., COP3530
    */
    // add student to student_directory
    Student s;
    s.class_codes = class_codes;
    s.residence_location_id = residence_id;
    s.student_name = student_name;
    student_directory[student_id] = s;
    // add student to each of the classes
    for (string class_code : class_codes){
        class_directory[class_code].students_enrolled.insert(student_id);
    }
    return true;
}
bool CampusCompass::Remove(string student_id){
    /*
    Find and remove a specific student with UFID number: STUDENT_ID from the data structure.
    If deletion is successful, print “successful.”
    If the STUDENT_ID does not exist, print “unsuccessful.”
    */
    // remove from classes
    set<string> classes = student_directory[student_id].class_codes;
    for (string class_code : classes){
        class_directory[class_code].students_enrolled.erase(student_id);
    }
    // remove from student directory
    student_directory.erase(student_id);
    return true;
}
bool CampusCompass::DropClass(string student_id,string class_code){
    /*
    Find and drop the class, CLASSCODE, for a given student.
    Note: If we remove a CLASSCODE from a student with only that CLASSCODE,    
        the student has 0 classes and should be dropped from the data structure.
    Still prints “successful” in this case
    Fails if a student with STUDENT_ID does not exist or if CLASSCODE does not exist.
    Fails if a student with STUDENT_ID does not have CLASSCODE
    prints “successful” if dropping is successful and prints “unsuccessful” otherwise.
    */
    // remove student from class
    class_directory[class_code].students_enrolled.erase(student_id);
    // remove class from student schedule
    student_directory[student_id].class_codes.erase(class_code);
    // remove student from directory if the student now has 0 classes
    if (student_directory[student_id].class_codes.empty()){
        student_directory.erase(student_id);
    }
    return true;
}
bool CampusCompass::ReplaceClass(string student_id, string class_code_1, string class_code_2){
    /*
    Find a student with UFID number: ID and replace CLASSCODE_1 with CLASSCODE_2 
    Should fail if the student does not exist, does not have CLASSCODE_1, or already has CLASSCODE_2; 
        or if there is no class with code CLASSCODE_2
    prints “successful” if the replacement is successful and prints “unsuccessful” otherwise.
    */
    // remove student from class 1
    DropClass(student_id,class_code_1);
    // add student to class 2
    student_directory[student_id].class_codes.insert(class_code_2);
    class_directory[class_code_2].students_enrolled.insert(student_id);
    return true;
}
bool CampusCompass::RemoveClass(string class_code){
    /*
    Removes a class CLASSCODE from the schedule for all students.
    Note: If we remove a CLASSCODE from a student with only that CLASSCODE and the student has 0 classes, 
        then the student should be dropped from the data structure.
    Prints the number of students the class was dropped from.
    E.g., if 4 students were enrolled in COP3530 then “removeClass COP3530” would print "4".
    */
    // remove all students from this class
    int students_dropped = class_directory[class_code].students_enrolled.size();
    set<string> students_enrolled = class_directory[class_code].students_enrolled;
    for (string student_id : students_enrolled){
        DropClass(student_id,class_code);
    }
    // print number of students the class was dropped from
    cout << students_dropped << endl;
    // remove class from class directory
    class_directory.erase(class_code);
    return true;
}
// helper
int CampusCompass::FindEdgeIndex(string from,string to){
    // find the "to" node and update the weight to be negative
    vector<pair<string,int>> outgoing = graph[from];
    int idx = 0;
    for (pair<string,int> p : outgoing){
        if (p.first == to){
            return idx;
        }
        idx++;
    }
    return -1;  //edge not found
}
bool CampusCompass::ToggleEdgesClosure(vector<pair<string,string>> location_pairs){
    /*
    Toggles the closure (soft delete) of N different edges.
    Note: These edges are not being removed from the graph. It is simply closed until toggled back on.
    Consecutive LOCATION_IDs will correspond to the same edges in this command:
    E.g., "toggleEdgesClosure 2 5 6 9 10" would close 2 edges, the edge between 5 to 6 and 9 to 10.
    If a given edge is already closed, this should open it, and if a given edge is open, this should close it.
    You will always be given valid edges in this method.
    prints  “successful” on completion.
    */
    // close edges that are already open and open edges that are already closed
    for (pair<string,string> p : location_pairs){
        // find the "to" node and update weight to negative
        int edge_idx = FindEdgeIndex(p.first,p.second);
        graph[p.first][edge_idx].second *= -1;
    }
    return true;
}
bool CampusCompass::CheckEdgeStatus(string location_1,string location_2){
    /*
    Checks whether a given edge is open, closed, or does not exist.
    Prints “open” “closed” or “DNE” accordingly.
    */
    int edge_idx = FindEdgeIndex(location_1,location_2);
    if (edge_idx == -1){
        // edge not found
        cout << "DNE" << endl;
        return true;
    }

    int weight = graph[location_1][edge_idx].second;
    if (weight < 0)
        cout << "close" << endl;
    else
        cout << "open" << endl;
    return true;
}

bool CampusCompass::IsConnected(string location_1,string location_2){
    /*
    Checks if it's possible to travel between LOCATION_ID_1 and LOCATION_ID_2 
        using any of the available edges.
    Prints “successful” if there is a path and “unsuccessful” if not.
    */
    // DFS for s-t path (determining if location_2 is reachable from location_1)
    set<string> visited;
    stack<string> stk;
    visited.insert(location_1);
    stk.push(location_1);
    while(!stk.empty()){
        string u = stk.top();
        stk.pop();
        for (auto v : graph[u]){
            string v_node = v.first;
            if (v_node == location_2)
                return true;
            if(visited.find(v_node) == visited.end()){
                visited.insert(v_node);
                stk.push(v_node);
            }
        }
    }
    return false;
}

pair<int,stack<string>> CampusCompass::ShortestPath(string s, string dest){
    // Dijkstra's algorithm for shortest path problem
    // initialize S with the start vertex, s, and V-S with the remaining vertices.
    set<string> S,V_S;      //visited and nonvisited sets
    map<string,string> p;   //predecessors
    map<string,int> d;      //distances from source

    S.insert(s);
    d[s] = 0;
    p[s] = "";
    for (auto l : locations){
        V_S.insert(l.first);
    }
    V_S.erase(s);

    // for all v in V-S
    for (string v : V_S){
        // set p[v] to s
        p[v] = s;
        // if there is an edge (s,v)
        int edge_idx = FindEdgeIndex(s,v);
        if (edge_idx != -1)
            // set d[v] to w(s,v)
            d[v] = graph[s][edge_idx].second;
        // else
        else
            // set d[v] to infinity 
            d[v] = INF;

    }
    // while V-S is not empty
    while (!V_S.empty()){
        // for all u in V-S, find the smallest d[u]
        int shortest_distance = INF;
        string u = "";
        for (string node : V_S){
            if (d[node] < shortest_distance){
                shortest_distance = d[node];
                u = node;
            }
        }
        // if all nodes are unreachable
        if (shortest_distance == INF){
            break;
        }
        // remove u from V-S and add u to S
        V_S.erase(u);
        S.insert(u);
        // for all v adjacent to u in V-S
        for (auto pair : graph[u]){
            string v = pair.first;
            int w = pair.second;
            // if d[u]+w(u,v) is less than d[v]
            if (w >= 0 && d[u] + w < d[v]){     //w>=0 only considers open paths
                // set d[v] to d[u] + w(u,v)
                d[v] = d[u] + w;
                // set p[v] to u
                p[v] = u;
            }  
        }
    }
    // shortest path
    stack<string> shortest_path;
    string prev = dest;
    while (p[prev] != ""){
        shortest_path.push(prev);
        prev = p[prev];
    }
    shortest_path.push(s);

    // shortest distance
    int shortest_distance;
    if (d[dest] == INF)
        shortest_distance = -1;  //unreachable
    else
        shortest_distance = d[dest];
    
    return make_pair(shortest_distance,shortest_path);
}

bool CampusCompass::PrintShortestEdges(string student_id){
    /*
    Prints the shortest walking time from a student's residence to each of their classes using only 
        currently available edges.
    You will be given a valid student ID
    Note: This does not take into account the start/end time of the classes. 
        Just the time it takes to get to them using available edges.
    Output should include the route for each class:
    Name: [Student Name]
    [ClassCode1] | Total Time: [Time]
    [ClassCode2] | Total Time: [Time]
    If a class is currently unable to be reached, then total time should equal -1.
    Name: Manav Sanghvi
    COP3530 | Total Time: -1
    The classes should be sorted in lexographical order. (E.g., COP3502 would come before COP3503).
    */
    Student s = student_directory[student_id];
    cout << "Name: " << s.student_name << endl;
    for (string class_code : s.class_codes){
        string class_location_id = class_directory[class_code].location_id;
        int shortest_distance = ShortestPath(s.residence_location_id, class_location_id).first;
        cout << class_code << " | " << "Total Time: " << shortest_distance << endl;
    }
    return true;
}

map<string, vector<pair<string,int>>> CampusCompass::GetMST(map<string, vector<pair<string,int>>> subgraph, set<string> vertices){
    // Prim's algorithm for minimum spanning tree
    /*
    pseudocode
    create an empty set
    put an arbitrary starting node in the set
    while your set does not contain all vertices of the graph
        add an adjacent vertex
            with least edge weight to any edge in the set
            is not in the set already
    */
    set<string> V,V_S;  
    map<string,vector<pair<string,int>>> mst; 

    // add all vertices to V_S
    V_S = vertices;

    // add an arbitrary node to S
    V.insert(*V_S.begin());

    while (!V_S.empty()){
        // find least edge weight that is open and not in S
        int least_weight = INF;
        string from,to;
        for (string v : V){
            for (auto p : subgraph[v]){
                int w = p.second;
                if (w >= 0 && w < least_weight){
                    least_weight = w;
                    from = v;
                    to = p.first;
                }
            }
        }
        // add to mst and update sets
        mst[from].push_back(make_pair(to,least_weight));
        V_S.erase(to);
        V.insert(to);
    }
    return mst;
}

bool CampusCompass::PrintStudentZone(string student_id){
    /*
    A student’s “zone” is the minimum-cost set of edges required to connect their residence and all their classes' locations, based on their shortest-path routes.
    You will be given a valid student ID
    To calculate this, you must:
    First: find the shortest path from the student’s residence to each of their classes using only currently accessible edges 
        (Note: this is the same functionality as your printShortestEdges command). 
    Second, create a sub-graph that contains all the vertices from all of those paths. Remaining edges must connect two nodes within this set of vertices.
    Third, get the minimum spanning tree of this sub-graph. This is the “Student Zone.”
    The output should be the total cost of this MST.
    We will only test this on a valid subgraph (i.e., we will not test this when a student has no reachable classes).
    Output Format:
    Student Zone Cost For [Student Name]: X
    */
    Student s = student_directory[student_id];

    // find the shortest path from residence to each of the classes using only currently accessible edges
    vector<stack<string>> shortest_paths;
    for (string class_code : s.class_codes){
        string class_location_id = class_directory[class_code].location_id;
        stack<string> shortest_path = ShortestPath(s.residence_location_id,class_location_id).second;
        shortest_paths.push_back(shortest_path);
    }
    
    // create a sub-graph that contains all vertices from all the shortest paths
    map<string,vector<pair<string,int>>> subgraph;
    set<string> vertices;
    for (stack<string> stk : shortest_paths){
        while (!stk.empty()){
            string from = stk.top();
            vertices.insert(from);
            stk.pop();
            if (!stk.empty()){
                string to = stk.top();
                int edge_idx = FindEdgeIndex(from,to);
                int w = graph[from][edge_idx].second;
                subgraph[from].push_back(make_pair(to,w));
            }
        }
    }

    // get MST of the sub-graph
    map<string,vector<pair<string,int>>> mst = GetMST(subgraph,vertices);

    // print the total cost of the MST
    int total_cost = 0;
    set<string> added_edges;
    string from,to;
    for (auto p_1 : mst){
        from = p_1.first;
        for (auto p_2 : mst[from]){
            to = p_2.first;
            if (added_edges.find(from+to) != added_edges.end()){
                total_cost+=p_2.second;
                added_edges.insert(from+to);
                added_edges.insert(to+from);
            }
        }
    }
    // Student Zone Cost For [Student Name]: X
    cout << "Student Zone Cost For " << s.student_name << ": " << total_cost << endl;

    return true;
}

bool CampusCompass::VerifySchedule(string student_id){
    /*
    A student's schedule is only feasible if they can physically get from one class to the next in the allotted time.
    For example, if COP3530 is from 10:40-11:30 and CDA3101 is from 11:45-12:35, but the shortest path from 
        COP3530 to CDA3101 takes longer than 15 minutes, then the schedule is not feasible.
    Note: the shortest path must use only available edges.
    If the Time Gap between the classes is longer than or equal to the shortest path, we output “Can make it!”
    If the Time Gap between the classes is shorter than the shortest path, we print “Cannot make it!”
    In the output, the classes should be organized from the earliest start time to the latest start time, 
        and you are checking the Time Gap between consecutive classes.
    If a student has only 1 class, print “unsuccessful” otherwise, print:
    Schedule Check for [Student Name]:
    [ClassCode1] - [ClassCode2] "Can make it!"
    [ClassCode2] - [ClassCode3] "Cannot make it!"
    */
    return true;
}


bool CampusCompass::ProcessCommand(const string &command){
    // process the validated command
    istringstream input_stream(command);
    string keyword, student_id, class_code;
    input_stream >> keyword;
    // extract necessary arguments and call respective functions
    bool success;
    if (keyword == "insert"){
        // insert STUDENT_NAME STUDENT_ID RESIDENCE_LOCATION_ID N CLASSCODE_1 CLASSCODE_2 … CLASSCODE_N
        string name,id,residence_id,n_str;
        input_stream >> name >> id >> residence_id >> n_str;
        int n = stoi(n_str);
        set<string> class_codes;
        for (int i=0;i<n;i++){
            string class_code;
            input_stream >> class_code;
            class_codes.insert(class_code);
        }
        success = Insert(name,id,residence_id,class_codes);
    } else if (keyword == "remove"){
        // remove STUDENT_ID
        input_stream >> student_id;
        success = Remove(student_id);
    } else if (keyword == "dropClass"){
        // dropClass STUDENT_ID CLASSCODE
        input_stream >> student_id >> class_code;
        success = DropClass(student_id,class_code);
    } else if (keyword == "replaceClass"){
        // replaceClass STUDENT_ID CLASSCODE_1 CLASSCODE_2	
        string class_code_2;
        input_stream >> student_id >> class_code >> class_code_2;
        success = ReplaceClass(student_id, class_code, class_code_2);
    } else if (keyword == "removeClass"){
        // removeClass CLASSCODE
        input_stream >> class_code;
        return RemoveClass(class_code);
    } else if (keyword == "toggleEdgesClosure"){
        // toggleEdgesClosure N LOCATION_ID_X LOCATION_ID_Y ... LOCATION_ID_A LOCATION_ID_B	
        string n_str;
        input_stream >> n_str;
        int n = stoi(n_str);
        // extract all n pairs
        vector<pair<string,string>> location_pairs;
        for (int i=0;i<n;i++){
            string location_1,location_2;
            input_stream >> location_1 >> location_2;
            location_pairs.push_back(make_pair(location_1,location_2));
        }
        success = ToggleEdgesClosure(location_pairs);
    } else if (keyword == "checkEdgeStatus"){
        // checkEdgeStatus LOCATION_ID_X LOCATION_ID_Y	
        string location_1,location_2;
        input_stream >> location_1 >> location_2;
        return CheckEdgeStatus(location_1,location_2);
    } else if (keyword == "isConnected"){
        // isConnected LOCATION_ID_1 LOCATION_ID_2	
        string location_1,location_2;
        input_stream >> location_1 >> location_2;
        success = IsConnected(location_1,location_2);
    } else if (keyword == "printShortestEdges"){
        // printShortestEdges ID
        input_stream >> student_id;
        return PrintShortestEdges(student_id);
    } else if (keyword == "printStudentZone"){
        // printStudentZone ID
        input_stream >> student_id;
        return PrintStudentZone(student_id);
    } else if (keyword == "verifySchedule"){
        // verifySchedule ID
        input_stream >> student_id;
        return VerifySchedule(student_id);
    } else {
        // invalid keyword
        return false;
    }
    if (success){
        cout << "successful" << endl;
        return true;
    } else {
        cout << "unsuccessful" << endl;
        return false;
    }
}
