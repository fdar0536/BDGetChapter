/*
Copyright (C) 2019 fdar0536
This file is part of BDGetChapter.
BDGetChapter is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
BDGetChapter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with BDGetChapter.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <string>

#include <cstring>

#include "../common/bdgetchap.h"

using namespace std;

void help(char**);

#ifdef _WIN32
void convert_path(string&);
#endif

int main(int argc, char *argv[])
{
    if (argc > 3)
    {
        cerr << "Too many arguments!" << endl;
        return 1;
    }

    if (argc == 1)
    {
        cerr << "No input!" << endl;
        cerr << "Please use \"-h\" or \"--help\" for more detail." << endl;
        return 1;
    }

    if (argc == 2)
    {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
        {
            help(argv);
            return 0;
        }
    }

    string input("");
    string output("");
    if (argc == 2)
    {
        input = argv[1];
#ifdef _WIN32
        convert_path(input);
#endif
        output = input;
    }
    else
    {
        input = argv[1];
        output = argv[2];
#ifdef _WIN32
        convert_path(input);
        convert_path(output);
#endif
    }

    try
    {
        bdgetchap_main_process(input, output);
    }
    catch(const char *err)
    {
        cerr << err << endl;
        return 1;
    }

    return 0;
}

void help(char **argv)
{
    cout << "Usage:" << endl;
    cout << argv[0] << " [path to your BDMV or BDISO or BD Drive] [output path]" << endl;
    cout << "Default output path is same as path to your BDMV or BDISO or BD Drive." << endl;
    cout << "\"-h\" or \"--help\": print this message and exit" << endl;
}

#ifdef _WIN32
void convert_path(string &input)
{
    string::size_type i = 0;
    while((i = input.find("\\", i)) != string::npos)
    {
        input.replace(i, 1, "/");
        ++i;
    }
}
#endif
