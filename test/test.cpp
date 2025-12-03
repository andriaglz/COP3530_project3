#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include "CampusCompass.h"
using namespace std;

// Andria Gonzalez Lopez
// 3572-4957

TEST_CASE("1 - Incorrect Commands"){
    // Test at least five incorrect commands. Ex: insert "A11y" 45679999 1 1 COP3530
    CampusCompass c;
    REQUIRE(c.ParseCSV("../data/edges.csv", "../data/classes.csv"));
    SECTION("1 - Insert"){
        // insert STUDENT_NAME STUDENT_ID RESIDENCE_LOCATION_ID N CLASSCODE_1 CLASSCODE_2 … CLASSCODE_N
        // The STUDENT_ID must be unique.
        // There must be N classes after reading N
        // NAME identifier will be separated by quotes for parsing, e.g., "Josh Smith."
        // CLASSCODE identifiers will always be a single word, e.g., COP3530
        
        // invalid commands 
        vector<string> invalid_inserts = {"slkdjflskdjf",
                                            "insert andria",
                                            "insert \"andria\" 11111111 0",
                                            "insert \"andria\" 11111111 1 COP3502 COP3503",
                                            "insert \"andria\" 11111111 1 SLDKFJ"};
        for (string command : invalid_inserts){
            REQUIRE(!c.ParseCommand(command));
        }
    }
    SECTION("2 - Remove"){
        REQUIRE(!c.ParseCommand("remove 222 2222 2"));
    }
    SECTION("3 - RemoveClass"){
        REQUIRE(!c.ParseCommand("removeClass LSKDFJLSKJ"));
    }
    SECTION("4 - ToggleEdgesClosure"){
        REQUIRE(!c.ParseCommand("toggleEdgesClosure 3 1 2 3"));
    }
    SECTION("5 - IsConnected"){
        REQUIRE(!c.ParseCommand("isConnected 10000 1000000"));
    }
}

TEST_CASE("2 - Edge Cases"){
    // Test at least three edge cases for various functions. Ex: removing a student that doesn’t exist 
    CampusCompass c;
    REQUIRE(c.ParseCSV("../data/edges.csv", "../data/classes.csv"));
    SECTION("1 - Insert"){
        // student id not unique
        string valid_command = "insert \"name\" 12345678 1 1 COP3530";
        REQUIRE(c.ParseCommand(valid_command));////
        REQUIRE(c.ProcessCommand(valid_command));
        REQUIRE(!c.ParseCommand(valid_command));
    }
    SECTION("2 - Remove"){
        // removing nonexistent student
        REQUIRE(!c.ParseCommand("remove 22222222"));
    }
    SECTION("3 - IsConnected"){
        string valid_command = "isConnected 1 2";
        REQUIRE(c.ParseCommand(valid_command));///////
        REQUIRE(c.ProcessCommand(valid_command));
    }
}

TEST_CASE("3 - Class Commands"){
    // Test your dropClass, removeClass, remove, and replaceClass commands
    CampusCompass c;
    REQUIRE(c.ParseCSV("../data/edges.csv", "../data/classes.csv"));
    string student_name = "andria";
    string student_id = "35724957";
    string residence_id = "1";
    set<string> class_codes;
    class_codes.insert("COP3530");
    class_codes.insert("COP3504");
    REQUIRE(c.Insert(student_name,student_id,residence_id,class_codes));

    SECTION("1 - dropClass"){
        REQUIRE(c.DropClass(student_id,"COP3530"));
        REQUIRE(c.is_in(student_id,c.GetStudentDirectory()));
        REQUIRE(c.DropClass(student_id,"COP3504"));
        REQUIRE(!c.is_in(student_id,c.GetStudentDirectory()));
    }
    SECTION("2 - removeClass"){
        REQUIRE(c.RemoveClass("COP3530") == 1);
        REQUIRE(c.Insert(student_name,"12345678",residence_id,class_codes));
        REQUIRE(c.RemoveClass("COP3504") == 2);
        REQUIRE(!c.is_in(student_id,c.GetStudentDirectory()));
    }
    SECTION("3 - remove"){
        REQUIRE(c.Remove(student_id));
        REQUIRE(!c.is_in(student_id,c.GetStudentDirectory()));
    }
    SECTION("4 - replaceClass"){
        REQUIRE(c.ReplaceClass(student_id,"COP3530","MAC2311"));
    }
}

TEST_CASE("4 - Print Shortest Edges"){
    // Test your printShortestEdges command in a case where a student can reach a class, 
    //  then one or more edges turn off, and then they cannot reach that class. 
    // (Hint: the visualizer tool can help with this a lot)
    CampusCompass c;
    REQUIRE(c.ParseCSV("../data/edges.csv", "../data/classes.csv"));
    string student_name = "andria";
    string student_id = "35724957";
    string residence_id = "19";
    set<string> class_codes;
    class_codes.insert("COP3502");
    REQUIRE(c.Insert(student_name,student_id,residence_id,class_codes));
    REQUIRE(c.IsConnected("19","41"));
    vector<pair<string,string>> location_pairs;
    location_pairs.push_back(make_pair("19","41"));
    location_pairs.push_back(make_pair("19","16"));
    REQUIRE(c.ToggleEdgesClosure(location_pairs));
    REQUIRE(!c.IsConnected("19","41"));
}