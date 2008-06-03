#include <string>
#include <vector>
#include <iostream>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

int main()
{
    string str1("hello abc-*-ABC-*-aBc goodbye");

    typedef iterator_range<string::iterator> string_iterator_range;
    typedef vector<string_iterator_range> find_vector_type;
    typedef vector<string> split_vector_type;

    // #1: search for separators
    find_vector_type find_vec;
    ifind_all(find_vec, str1, "abc");   // abc, ABC, aBc

    // #2: search for tokens
    split_vector_type split_vec;
    split(split_vec, str1, is_any_of("-*"), token_compress_on); // "hello abc", "ABC", "aBc goodbye"

    cout << "tokens: ";
    for (split_vector_type::iterator it = split_vec.begin();
         it != split_vec.end();
         ++it)
    {
        cout << '"' << (*it) << '"' << " ";
    }
    cout << endl;

    return 0;
}
