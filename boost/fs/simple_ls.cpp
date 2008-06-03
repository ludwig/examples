// http://www.boost.org/doc/libs/1_35_0/libs/filesystem/example/simple_ls.cpp
//  See http://www.boost.org/libs/filesystem for documentation.

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/progress.hpp>
#include <iostream>

namespace fs = boost::filesystem;

int main(int argc, char *argv[])
{
    boost::progress_timer t(std::clog);

    fs::path full_path(fs::initial_path<fs::path>());

    if (argc > 1)
    {
        full_path = fs::system_complete(fs::path(argv[1], fs::native));
    }
    else
    {
        std::cout << "\nusage: simple_ls [path]" << std::endl;
    }

    unsigned long file_count = 0;
    unsigned long dir_count = 0;
    unsigned long other_count = 0;
    unsigned long err_count = 0;

    if (!fs::exists(full_path))
    {
        std::cout << "\nNot found: "
                  << full_path.native_file_string()
                  << std::endl;
        return 1;
    }

    if (fs::is_directory(full_path))
    {
        std::cout << "\nIn directory: "
                  << full_path.native_directory_string()
                  << "\n\n";

        fs::directory_iterator end_iter;
        for (fs::directory_iterator dir_iter(full_path);
             dir_iter != end_iter;
             ++dir_iter)
        {
            try
            {
                if (fs::is_directory(dir_iter->status()))
                {
                    ++dir_count;
                    std::cout << dir_iter->leaf() << " [directory]\n";
                }
                else if (fs::is_regular(dir_iter->status()))
                {
                    ++file_count;
                    std::cout << dir_iter->leaf() << "\n";
                }
                else
                {
                    ++other_count;
                    std::cout << dir_iter->leaf() << " [other]\n";
                }
            }
            catch (const std::exception &ex)
            {
                ++err_count;
                std::cout << dir_iter->leaf() << " " << ex.what() << std::endl;
            }
        }

        std::cout << "\n" 
                  << file_count << " files\n"
                  << dir_count << " directories\n"
                  << other_count << " others\n"
                  << err_count << " errors\n";
    }
    else // must be a file
    {
        std::cout << "\nFound: " << full_path.native_file_string() << "\n";
    }

    return 0;
}
