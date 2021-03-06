#include <catch.hpp>
#include <sstream>
#include <iostream>
#include <limits>

using namespace std;

TEST_CASE( "operator >>", "[std] [istream] [input]" ) {
    
    SECTION( "read all values from the stream" ) {
        istringstream sin{"1 2   5  6 \n8 \n\n\n10 "};
        
        int x = 0;
        int counter = 0;
        while (sin >> x) ++counter;

        REQUIRE( 6 == counter );
        REQUIRE( sin.eof() );
        REQUIRE( sin.fail() );
        REQUIRE_FALSE( sin.bad() );
    }

    SECTION( "read two int values from istream" ) {
        istringstream sin{"1 2"};

        int x = 0;

        REQUIRE( sin >> x );
        REQUIRE( 1 == x );

        REQUIRE( sin >> x );
        REQUIRE( 2 == x );

        REQUIRE_FALSE ( sin >> x );
        REQUIRE( sin.eof() );
    } 

    SECTION( "try read two int values from istream, first value is not an int" ) {
        istringstream sin{"a 2"};

        int x = 0;

        REQUIRE_FALSE( sin >> x );
        
        bool recoverable_error = sin.fail();
        REQUIRE( recoverable_error );

        // recover
        sin.clear();
        sin.ignore(1);

        REQUIRE( sin >> x );
        REQUIRE( 2 == x );

        REQUIRE_FALSE( sin >> x );
        REQUIRE( sin.eof() );
    }

}

TEST_CASE( "gcount", "[std] [istream] [input]" ) {
    
    SECTION( "number of characters extracted after calling get" ) {
        istringstream sin{"12345"};
        
        constexpr streamsize max_characters_to_extract = 20;
        char buffer[max_characters_to_extract]; 
        
        sin.get(buffer, max_characters_to_extract);

        auto characters_extracted = sin.gcount();

        REQUIRE( 5 == characters_extracted );
    }

    SECTION( "number of characters extracted after calling getline" ) {
        istringstream sin{"12345\n1234567"};
        const streamsize max_characters_to_extract = 20;
        char buffer[max_characters_to_extract]; 
        
        sin.getline(buffer, max_characters_to_extract);

        auto characters_extracted = sin.gcount();
        const streamsize expected_extracted_5_plus_new_line = 6; 

        REQUIRE( expected_extracted_5_plus_new_line == characters_extracted );

        sin.getline(buffer, max_characters_to_extract);

        characters_extracted = sin.gcount();

        REQUIRE( 7 == characters_extracted );
    }

    SECTION( "number of characters extracted after calling ignore with count" ) {
        istringstream sin{"12345"};
       
        const streamsize ignore_count = 3; 
        sin.ignore(ignore_count);

        auto characters_extracted = sin.gcount();

        REQUIRE( ignore_count == characters_extracted );
    }

    SECTION( "number of characters extracted after calling ignore with delimiter" ) {
        istringstream sin{"12345A more text"};

        const char delimiter = 'A'; 
        sin.ignore(20, delimiter);

        auto characters_extracted = sin.gcount();

        const streamsize ignored_including_delimiter = 6; 
        REQUIRE( ignored_including_delimiter == characters_extracted );
    }

    SECTION( "number of characters extracted after calling operator >>" ) {
        istringstream sin{"1 2 3"};
        int x = 0;

        sin >> x;

        REQUIRE( 1 == x );

        auto characters_extracted = sin.gcount();

        REQUIRE( 0 == characters_extracted );
    }

}

TEST_CASE( "get", "[std] [istream] [input]" ) {
    
    SECTION( "get() single character" ) {
        istringstream sin{"ABC"};
        
        auto ch = sin.get();

        REQUIRE( 'A' == ch );
    }

    SECTION( "get() new line single character" ) {
        istringstream sin{"\nABC"};
        
        auto ch = sin.get();

        REQUIRE( '\n' == ch );
    }

    SECTION( "get() eof" ) {
        istringstream sin;
        
        auto ch = sin.get();

        REQUIRE( EOF  == ch );
    }

    SECTION( "get(char_type& ch) single character" ) {
        istringstream sin{"ABC"};
        char ch = 0;

        REQUIRE( sin.get(ch) );

        REQUIRE( 'A' == ch );
    }

    SECTION( "get(char_type* buffer, streamsize count)  3 characters" ) {
        istringstream sin{"123456"};

        const streamsize count_including_null = 4;
        char buffer[count_including_null];

        // getline would return false
        REQUIRE( sin.get(buffer, count_including_null) );

        const string expected = "123"; 
        REQUIRE( expected == buffer );
        REQUIRE( 3 == sin.gcount() );
    }

    SECTION( "get(char_type* buffer, streamsize count)  eof" ) {
        istringstream sin;

        const streamsize count_including_null = 4;
        char buffer[count_including_null];
        buffer[0] = 'Z';

        REQUIRE_FALSE( sin.get(buffer, count_including_null) );
        // TODO check why behaviour is inconsistent for get(char_type&) returning eof
        REQUIRE( 0 == buffer[0] );
    }

    SECTION( "get(char_type* buffer, streamsize count)  up to new line" ) {
        istringstream sin{"123\n456"};

        const streamsize count_including_null = 20;
        char buffer[count_including_null];

        REQUIRE( sin.get(buffer, count_including_null) );

        const string expected = "123"; 
        REQUIRE( expected == buffer );
        // after calling getline gcount would return 4
        REQUIRE( 3 == sin.gcount() );
    }

    SECTION( "get(char_type* buffer, streamsize count, char_type delim)  up to new line" ) {
        istringstream sin{"123\n456"};

        const streamsize count_including_null = 20;
        char buffer[count_including_null];
        
        char delim = '\n';
        REQUIRE( sin.get(buffer, count_including_null, delim) );

        const string expected = "123"; 
        REQUIRE( expected == buffer );
        // after calling getline gcount would return 4
        REQUIRE( 3 == sin.gcount() );
    }

    SECTION( "get(basic_streambuf& streambuf)" ) {
        istringstream     sin{"1234567\n456"};
        ostringstream    sout;
        const string expected{"1234567"};

        sin.get(*sout.rdbuf());

        REQUIRE( expected == sout.str() );
    }

    SECTION( "get(basic_streambuf& streambuf, char_type delim)" ) {
        istringstream     sin{"1234567 A 456"};
        ostringstream    sout;
        const string expected{"1234567 "};

        const char delim = 'A';
        sin.get(*sout.rdbuf(), delim);

        REQUIRE( expected == sout.str() );
    }

}

TEST_CASE( "getline", "[std] [istream] [input]" ) {

     SECTION( "getline(char_type* buffer, streamsize count)  3 characters" ) {
        istringstream sin{"123456"};

        const streamsize count_including_null = 4;
        char buffer[count_including_null];

        // get would return true
        REQUIRE_FALSE( sin.getline(buffer, count_including_null) );

        const string expected = "123"; 
        REQUIRE( expected == buffer );
        REQUIRE( 3 == sin.gcount() );
    }

    SECTION( "getline(char_type* buffer, streamsize count)  eof" ) {
        istringstream sin;

        const streamsize count_including_null = 4;
        char buffer[count_including_null];
        buffer[0] = 'Z';

        REQUIRE_FALSE( sin.getline(buffer, count_including_null) );
        REQUIRE( 0 == buffer[0] );
    }

    SECTION( "get(char_type* buffer, streamsize count)  up to new line" ) {
        istringstream sin{"123\n456"};

        const streamsize count_including_null = 20;
        char buffer[count_including_null];

        REQUIRE( sin.getline(buffer, count_including_null) );

        const string expected = "123"; 
        REQUIRE( expected == buffer );
        // after calling get gcount would return 3
        REQUIRE( 4 == sin.gcount() );
    }

    SECTION( "get(char_type* buffer, streamsize count, char_type delim)  up to new line" ) {
        istringstream sin{"123\n456"};

        const streamsize count_including_null = 20;
        char buffer[count_including_null];
        
        char delim = '\n';
        REQUIRE( sin.getline(buffer, count_including_null, delim) );

        const string expected = "123"; 
        REQUIRE( expected == buffer );
        // after calling get gcount would return 3
        REQUIRE( 4 == sin.gcount() );
    }
   
}

TEST_CASE( "std::getline", "[std] [istream] [input]" ) {

     SECTION( "retrieve full string" ) {
        istringstream      sin{"123456"};
        const string  expected{"123456"}; 
              string       str;

        REQUIRE( getline(sin, str) );

        REQUIRE( expected == str );
    }

    SECTION( "eof" ) {
        istringstream sin;
        string        str;

        REQUIRE_FALSE( getline(sin, str) );
    }

    SECTION( "up to new line" ) {
        istringstream sin{"123\n456"};
        string        str;

        REQUIRE( getline(sin, str) );
        REQUIRE( "123" == str );

        REQUIRE( getline(sin, str) );
        REQUIRE( "456" == str );

        const string before_fail = str;
        REQUIRE_FALSE( getline(sin, str) );
        REQUIRE( before_fail == str );

    }

    SECTION( "up to delimiter" ) {
        istringstream sin{"ABC|DEF"};
        string        str;

        const char delim = '|';
        
        REQUIRE( getline(sin, str, delim) );
        REQUIRE( "ABC" == str );

        REQUIRE( getline(sin, str, delim) );
        REQUIRE( "DEF" == str );

        const string before_fail = str;
        REQUIRE_FALSE( getline(sin, str, delim) );
        REQUIRE( before_fail == str );
    }
   
}

TEST_CASE( "ignore", "[std] [istream] [input]" ) {

     SECTION( "ignore one character" ) {
        istringstream sin{"123456"};

        REQUIRE( sin.ignore(1) ); 

        REQUIRE( '2' == sin.get() );
    }

    SECTION( "ignore three character" ) {
        istringstream  sin{"123456"};

        REQUIRE( sin.ignore(3) ); 

        REQUIRE( '4' == sin.get() );
    }

    SECTION( "ignore three character" ) {
        istringstream sin{"123456"};

        REQUIRE( sin.ignore(3) ); 

        REQUIRE( '4' == sin.get() );
    }

    SECTION( "ignore all character" ) {
        istringstream sin{"123456"};

        REQUIRE( sin.ignore(numeric_limits<int>::max()) ); 

        REQUIRE( EOF == sin.get() );

        REQUIRE_FALSE ( sin.ignore(1) );
    }

}

TEST_CASE( "peek", "[std] [istream] [input]" ) {

     SECTION( "peek one character" ) {
        istringstream sin{"ABCDEFG"};

        REQUIRE( 'A' == sin.peek() ); 
    }

    SECTION( "peek when next character is new line" ) {
        istringstream sin{"\nABCDEFG"};

        REQUIRE( '\n' == sin.peek() ); 
    }

    SECTION( "peek when next character is eof" ) {
        istringstream sin;

        REQUIRE( EOF == sin.peek() ); 
    }
    
}

TEST_CASE( "putback", "[std] [istream] [input]" ) {

    SECTION( "fails for input only stream" ) {
        istringstream sin{"ABCDEFG"};

        sin.get();

        REQUIRE_FALSE( sin.putback('a') );
    }

    SECTION( "succeedes for input only stream when put back character is not different from character extracted" ) {
        istringstream sin{"ABCDEFG"};

        REQUIRE( 'A' == sin.get() );
        REQUIRE( sin.putback('A') );
        REQUIRE( 'A' == sin.get() );
    }

    SECTION( "replace first character in the stream" ) {
        stringstream sin{"ABCDEFG"};

        sin.get();

        REQUIRE( sin.putback('a') );
        REQUIRE( 'a' == sin.get() );
        REQUIRE( 'B' == sin.get() );
    }

    SECTION( "fails if no character was retrieved prior" ) {
        stringstream sin{"ABCDEFG"};

        REQUIRE_FALSE( sin.putback('a') );
    }
    
}

TEST_CASE( "read", "[std] [istream] [input]" ) {

    SECTION( "read one character" ) {
        istringstream sin{"12345"};
        
        char ch = '\0';
        REQUIRE( sin.read(&ch, 1) );

        REQUIRE( 1 == sin.gcount() );
        REQUIRE( ch == '1' );
    }

    SECTION( "read exact" ) {
        istringstream sin{"12345"};
        
        const int count = 5;
        const char non_null_char = 'Z';
        char buffer[count + 1];
        buffer[count] = non_null_char;

        REQUIRE( sin.read(buffer, count) );

        REQUIRE( count == sin.gcount() );
        REQUIRE( non_null_char == buffer[count] );

        buffer[count] = '\0';
        REQUIRE( string{"12345"} == buffer );
    }

    SECTION( "try to read more than in the stream" ) {
        istringstream sin{"12345"};
        
        const int count = 10;
        char buffer[count + 1];

        REQUIRE_FALSE( sin.read(buffer, count) );

        REQUIRE( 5 == sin.gcount() );

        buffer[5] = '\0';
        REQUIRE( string{"12345"} == buffer );
    }

    SECTION( "try to read from empty stream" ) {
        istringstream sin;
        
        char buffer[1];

        REQUIRE_FALSE( sin.read(buffer, 1) );

        REQUIRE( 0 == sin.gcount() );
    }

}

TEST_CASE( "readsome", "[std] [istream] [input]" ) {

    SECTION( "read one character if available in internal buffer" ) {
        istringstream sin{"ABC"};
        REQUIRE( sin.rdbuf()->in_avail() > 0 );
        
        char ch = '\0';

        REQUIRE( 1 == sin.readsome(&ch, 1) );

        REQUIRE( ch == 'A' );
    }

    SECTION( "try to read more characters that area available" ) {
        istringstream sin{"ABC"};
        REQUIRE( sin.rdbuf()->in_avail() > 0 );
        
        const int count = 10;
        char buffer[count];

        REQUIRE( 3 == sin.readsome(buffer, count) );

        buffer[3] = '\0';

        REQUIRE( string{"ABC"} == buffer );
    }

    SECTION( "try to readsome from empty stream" ) {
        istringstream sin;
        REQUIRE( sin.rdbuf()->in_avail() == 0 );
        
        const int count = 10;
        char buffer[count];

        REQUIRE( 0 == sin.readsome(buffer, count) );
        REQUIRE( sin );

        REQUIRE( 0 == sin.readsome(buffer, count) );
        REQUIRE( sin );
    }

}

TEST_CASE( "unget", "[std] [istream] [input]" ) {

    SECTION( "unget read character" ) {
        istringstream sin{"ABCDEFG"};

        REQUIRE( 'A' == sin.get() );
        REQUIRE( sin.unget() );
        REQUIRE( 'A' == sin.get() );
    }

    SECTION( "unget fails after eof" ) {
        istringstream sin{"A"};

        REQUIRE( 'A' == sin.get() );
        REQUIRE( EOF == sin.get() );
        REQUIRE_FALSE( sin.unget() );
    }

    SECTION( "unget last character" ) {
        istringstream sin{"ABC"};

        REQUIRE( 'A' == sin.get() );
        REQUIRE( 'B' == sin.get() );

        REQUIRE( 'C' == sin.get() );
        REQUIRE( sin.unget() );
        REQUIRE( 'C' == sin.get() );
    }

}
