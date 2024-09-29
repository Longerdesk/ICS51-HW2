#include "catch_amalgamated.hpp"
#include "SkipList.hpp"
#include <vector>
#include <string>

/*
NOTE:  these are not intended as exhaustive tests.
	 You should make your own additional tests
	 VERY IMPORTANT:  if your SkipList is not templated, or if 
			it is not built as a linked structure, your score for this project
			will be zero.  Be sure your SkipList compiles and runs 
	 		with non-numeric data types. 


NOTE ALSO:  "Not Required" does not mean "optional" or "extra credit."  
"Not required" only means that we will still grade your project, even 
if these exact tests do not pass.  
*/

namespace{

	TEST_CASE("SimpleNextTest", "[SampleTests]")
	{
		SkipList<std::string, std::string> s1;
		s1.insert("apple","red");
		s1.insert("banana", "yellow");
		s1.insert("lime","green");
		s1.insert("lemon", "yellow");
		s1.insert("grape", "purple");
		bool test = s1.isSmallestKey("apple");
		REQUIRE( test == true );
	}

	TEST_CASE("SimpleStringTest", "[SampleTests]")
	{
		SkipList<std::string, std::string> s1;
		s1.insert("apple","red");
		s1.insert("banana", "yellow");
		s1.insert("lime","green");
		s1.insert("lemon", "yellow");
		s1.insert("grape", "purple");
		std::string test = s1.nextKey("apple");
		REQUIRE( test == "banana" );
	}


	TEST_CASE("SimpleInsertTest", "[SampleTests]")
	{
		SkipList<unsigned, unsigned> s1;
		//s1.insert(5,10);
		s1.insert(5,1);
		//s1.insert(7,15);
		bool test = s1.isSmallestKey(5);
		//unsigned test = s1.height(3);
		REQUIRE( test == true );
	}

	TEST_CASE("SimpleSizeTest", "[SampleTests]")
	{
		SkipList<unsigned, unsigned> s1;
		s1.insert(5,10);
		s1.insert(3,1);
		s1.insert(7,15);
		unsigned test = s1.size();
		REQUIRE( test == 3 );
	}

	TEST_CASE("SimpleHeightsTest", "[SampleTests]")
	{
		SkipList<unsigned, unsigned> sl;
		std::vector<unsigned> heights;
		for(unsigned i=0; i < 10; i++)
		{
			sl.insert(i,i);
			heights.push_back( sl.height(i) );
		}
		std::vector<unsigned> expectedHeights = {1, 2, 1, 3, 1, 2, 1, 4, 1, 2};
		REQUIRE( heights == expectedHeights );
	}

	TEST_CASE("SimpleFindTest", "[SampleTests]")
	{
		SkipList<unsigned, unsigned> sl;
		for(unsigned i=0; i < 10; i++)
		{
			sl.insert(i, (100 + i) );
		}
		for(unsigned i=0; i < 10; i++)
		{
			REQUIRE((i+100) == sl.find(i));
		}
	}

	TEST_CASE("SimpleLargestAndSmallest", "[SampleTests]")
	{
		SkipList<unsigned, unsigned> sl;
		for(unsigned i=0; i < 10; i++)
		{
			sl.insert(i, (100 + i) );
		}
		REQUIRE( sl.isSmallestKey( 0 ) );
		REQUIRE( sl.isLargestKey( 9 ) );

	}

	TEST_CASE("InvolvedHeightsTest", "[SampleTests]")
	{
		SkipList<unsigned, unsigned> sl;
		std::vector<unsigned> heights;
		for (unsigned i = 0; i < 10; i++)
		{
			sl.insert(i, i);
			heights.push_back(sl.height(i));
		}

		// The coinFlip function will always return heads
		// for 255 regardless of the current layer.
		// You can use this value to test your threshold for halting
		// the insertion procedure. If this results in an infinite loop inside
		// of your insert function you have not implemented a cutoff threshold.
		unsigned const MAGIC_VAL = 255;
		sl.insert(MAGIC_VAL, MAGIC_VAL);

		heights.push_back(sl.height(MAGIC_VAL));

		// The expected height for 255 is 12 because there are fewer than 16 nodes in
		// the skip list when 12 is added.
		std::vector<unsigned> expectedHeights = {1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 12};
		REQUIRE(heights == expectedHeights);

		// At this point there should be 13 layers
		// (because the fast lane is not included in the height calculation).
		REQUIRE(sl.numLayers() == 13);
	}

	TEST_CASE("Capacity17Test", "[SampleTests]")
	{
		SkipList<unsigned, unsigned> sl;
		std::vector<unsigned> heights;

		// First insert 16 values into the skip list [0, 15].
		for (unsigned i = 0; i < 16; i++)
		{
			sl.insert(i, i);
			heights.push_back(sl.height(i));
		}

		// Same value used in SimpleHeightsTest for testing the threshold.
		unsigned const MAGIC_VAL = 255;
		sl.insert(MAGIC_VAL, MAGIC_VAL);

		heights.push_back(sl.height(MAGIC_VAL));

		// The expected height for 255 is 15 because 3 * ceil(log_2(17)) = 15
		// meaning the maximum height of any node should be 15 for a skip list with 17 nodes.
		std::vector<unsigned> expectedHeights = {1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, 5, 15};
		REQUIRE(heights == expectedHeights);

		// At this point there should be 16 layers
		// (because the fast lane is not included in the height calculation).
		REQUIRE(sl.numLayers() == 16);
	}



}
