// #include <catch2/catch_test_macros.hpp>
// #include <iostream>
// #include "CampusCompass.h"
// using namespace std;

// // Andria Gonzalez Lopez
// // 3572-4957

// TEST_CASE("1 - Incorrect Commands","[insert]"){
// 	AVL t;

// 	SECTION("ID - Invalid Size - Too Short"){
// 		// < 8 digits
// 		REQUIRE(!t.insert("fldksjfl", "12"));
// 	};
// 	SECTION("ID - Invalid Size - Too Long"){
// 		// > 8 digits
// 		REQUIRE(!t.insert("fldksjfl", "100000000"));
// 	};
// 	SECTION("ID - Not Unique"){		
// 		t.insert("name","12345678");
// 		REQUIRE(!t.insert("name","12345678"));
// 	};
// 	SECTION("Name - Invalid Characters"){		
// 		// not digits
// 		REQUIRE(!t.insert("A!@#$%^B","87654321"));
// 	};
// 	SECTION("Name - Digits"){
// 		// not a-z, A-Z, and spaces
// 		REQUIRE(!t.insert("A239487938B","87654311"));
// 	};
// 	SECTION("Valid Input"){
// 		// not a-z, A-Z, and spaces
// 		REQUIRE(t.insert("A","35724957"));
// 	};
// }

// TEST_CASE("2 - Edge Cases","[remove][remove_inorder][level_count]"){
// 	AVL t;
// 	t.insert("Andria","12345678");

// 	SECTION("Removing Nonexistent Node"){
// 		REQUIRE(!t.remove("11111111"));
// 	};
// 	SECTION("Out of Bounds Inorder Removal"){
// 		REQUIRE(!t.remove_inorder(10));
// 	};
// 	SECTION("Empty Tree Level Count"){
// 		AVL t1;
// 		REQUIRE(t1.level_count()==0);
// 	}
// }

// TEST_CASE("3 - Rotation Cases", "[insert][inorder]"){
// 	SECTION("Insert"){
// 		// valid input
// 		AVL t;
// 		REQUIRE(t.insert("Andria G","12345678"));
// 	};

// 	vector<string> rr = {"12345678","22345678","32345678"};
// 	vector<string> ll = {"32345678","22345678","12345678"};
// 	vector<string> rl = {"12345678","32345678","22345678"};
// 	vector<string> lr = {"32345678","12345678","22345678"};
// 	vector<string> expected = {"12345678", "22345678", "32345678"};

// 	SECTION("RR Case"){
// 		AVL t;
// 		for (string i : rr){
// 			t.insert("a", i);
// 		}
// 		vector<string> actual = t.inorder();
// 		REQUIRE(actual == expected);
// 	};
// 	SECTION("LL Case"){
// 		AVL t;
// 		for (string i : ll){
// 			t.insert("a", i);
// 		}
// 		vector<string> actual = t.inorder();
// 		REQUIRE(actual == expected);
// 	};
// 	SECTION("RL Case"){
// 		AVL t;
// 		for (string i : rl){
// 			t.insert("a", i);
// 		}
// 		vector<string> actual = t.inorder();
// 		REQUIRE(actual == expected);
// 	};
// 	SECTION("LR Case"){
// 		AVL t;
// 		for (string i : lr){
// 			t.insert("a", i);
// 		}
// 		vector<string> actual = t.inorder();
// 		REQUIRE(actual == expected);
// 	};
// }

// TEST_CASE("4 - Deletions", "[insert][remove]"){
// 	SECTION("No Children"){
// 		vector<string> ids = {"00000004","00000002","00000001","00000003","00000005"};
// 		AVL t;
// 		for (string i : ids){
// 			t.insert("a", i);
// 		}
// 		t.remove("00000003");
// 		vector<string> actual = t.inorder();
// 		vector<string> expected = {"00000001","00000002","00000004","00000005"};
// 		REQUIRE(actual == expected);
// 		REQUIRE(t.level_count() == 3);
// 	};
// 	SECTION("One Child"){
// 		vector<string> ids = {"00000004","00000002","00000001","00000005"};
// 		AVL t;
// 		for (string i : ids){
// 			t.insert("a", i);
// 		}
// 		t.remove("00000002");
// 		vector<string> actual = t.inorder();
// 		vector<string> expected = {"00000001","00000004","00000005"};
// 		REQUIRE(actual == expected);
// 		REQUIRE(t.level_count() == 2);
// 	};
// 	SECTION("Two Children"){
// 		vector<string> ids = {"00000004","00000002","00000006","00000001",
// 								"00000003","00000005","00000007"};
// 		AVL t;
// 		for (string i : ids){
// 			t.insert("a", i);
// 		}
// 		t.remove("00000004");
// 		vector<string> actual = t.inorder();
// 		vector<string> expected = {"00000001","00000002","00000003","00000005",
// 									"00000006","00000007"};
// 		REQUIRE(actual == expected);
// 		REQUIRE(t.level_count() == 3);
// 	};
// }

// TEST_CASE("5 - Large Insertions","[insert][remove_inorder][inorder]"){
// 	AVL tree;
// 	vector<string> expected, actual;

// 	// insert 100 nodes
// 	for (int i=0;i<100;i++){
// 		int input = (rand() % 90000000) + 10000000;	//random 8-digit number
// 		string id = to_string(input);
// 		if (count(expected.begin(),expected.end(),id) == 0){	//not already in vector
// 			if (tree.insert("a", id)) {   //successful insert
// 				expected.push_back(id);
// 			}
// 		}
// 	}

// 	// remove 10 nodes
// 	sort(expected.begin(), expected.end());
// 	for (int i=0;i<10;i++){
// 		tree.remove_inorder(0);
// 		expected.erase(expected.begin());
// 	}

// 	// check equivalence
// 	actual = tree.inorder();
// 	REQUIRE(expected.size() == actual.size());
// 	REQUIRE(expected == actual);
// }
