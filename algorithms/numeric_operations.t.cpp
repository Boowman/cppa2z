#include <catch.hpp>
#include <algorithm>
#include <numeric>
#include <vector>
#include <cmath>

using namespace std;

TEST_CASE( "accumulate", "[std] [algorithm] [numeric operations]" ) {
    
    SECTION( "compute sum of all elements in the range plus the initial value" ) {
        const vector<int> vec{1, 2, 3};

        const int initial_value = 10; 
        const int expected = initial_value + 1 + 2 + 3;

        const int sum = accumulate(cbegin(vec), cend(vec), initial_value);    
                  
        REQUIRE( expected == sum  ); 
    } 

    SECTION( "compute product of all elements in the range" ) {
        const vector<int> vec{5, 10, 100};

        const int initial_value = 1;
        const int expected = initial_value * 5 * 10 * 100;

        auto multiply = [](int x, int y) { return x * y; };
    
        const int product = accumulate(
                                  cbegin(vec)
                                , cend(vec)
                                , initial_value
                                , multiply
                            );    
                  
        REQUIRE( expected == product  ); 
    } 

}

TEST_CASE( "adjacent_difference", "[std] [algorithm] [numeric operations]" ) {
    
    SECTION( "compute dfference between next element and its previous element writing the result to the different range" ) {
        const vector<int>      vec{4, 9, 15, 22, 30};
              vector<int>   result;
        const vector<int> expected{4, 5,  6,  7,  8};

        adjacent_difference(cbegin(vec), cend(vec), back_inserter(result));    
                  
        REQUIRE( expected == result ); 
    } 

    SECTION( "compute absolute dfference between next element and its previous element writing the result to the different range" ) {
        const vector<int>      vec{4, -9, 15, -22, -30};
              vector<int>   result;
        const vector<int> expected{4,  5,  6,   7,   8};


        auto diff = [](int x, int y) { return abs(abs(x) - abs(y)); };

        adjacent_difference(cbegin(vec), cend(vec), back_inserter(result), diff);    
                  
        REQUIRE( expected == result ); 
    } 

}

TEST_CASE( "inner_product", "[std] [algorithm] [numeric operations]" ) {
    
    SECTION( "compute sum(+) of products(*)" ) {
        const vector<int> vec1{1, 2, 3};
        const vector<int> vec2{4, 5, 6};
        
        const int initial_value = 10; 
        const int expected = initial_value + (1 * 4)  +  (2 * 5)  +  (3 * 6);

        const int result = inner_product(
                                 cbegin(vec1)
                               , cend(vec1) 
                               , cbegin(vec2) 
                               , initial_value
                           ); 
                  
        REQUIRE( expected == result ); 
    } 

    SECTION( "compute products(*) of sum(+)" ) {
        const vector<int> vec1{1, 2, 3};
        const vector<int> vec2{4, 5, 6};
        
        const int initial_value = 10; 
        const int expected = initial_value * (1 + 4)  *  (2 + 5)  *  (3 + 6);

        auto op1 = [](int x, int y) { return x * y; };
        auto op2 = [](int x, int y) { return x + y; };

        const int result = inner_product(
                                 cbegin(vec1)
                               , cend(vec1) 
                               , cbegin(vec2) 
                               , initial_value
                               , op1
                               , op2
                           ); 
                  
        REQUIRE( expected == result ); 
    }

}

TEST_CASE( "iota", "[std] [algorithm] [numeric operations]" ) {
    
    SECTION( "fill the range with sequentially incremented values" ) {
              vector<int>      vec(5);
        const vector<int> expected{1, 2, 3, 4, 5};
        
        const int start_value = 1;
        
        iota(begin(vec), end(vec), start_value);
                  
        REQUIRE( expected == vec ); 
    } 

}

TEST_CASE( "partial_sum", "[std] [algorithm] [numeric operations]" ) {
    
    SECTION( "compute partial sum of elements in the range and write results to another range" ) {
              vector<int>      vec{1, 2, 3,  4,  5};
              vector<int>   result;
        const vector<int> expected{1, 3, 6, 10, 15};
        
        partial_sum(cbegin(vec), cend(vec), back_inserter(result));
                  
        REQUIRE( expected == result ); 
    } 

    SECTION( "compute partial product(*) of elements in the range and write results to another range" ) {
              vector<int>      vec{1, 2, 3,  4,  5};
              vector<int>   result;
        const vector<int> expected{1, 2, 6, 24, 120};
        
        auto multiply = [](int x, int y) { return x * y; };
        
        partial_sum(cbegin(vec), cend(vec), back_inserter(result), multiply);
                  
        REQUIRE( expected == result ); 
    } 

}
