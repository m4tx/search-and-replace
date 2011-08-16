#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <stdint.h>

using namespace std;

int replaceAll (string &str, string const &pattern, string const &replacement) {
    int replacedStrings = 0;
    std::string::size_type start = str.find(pattern, 0);
    while (start != str.npos) {
        str.replace(start, pattern.size(), replacement);
        start = str.find(pattern, start+replacement.size());

        replacedStrings++;
    }
    return replacedStrings;
}

int main (int argc, char *argv[]) {
    // Usage
    if (argc != 2) {
        cout << "Version: Search and Replace 1.0 17-08-2011 http://www.m4tx.pl" << endl
             << "Copyright: Copyright (C) 2011 m4tx" << endl
             << endl
             << "Usage: \"" << argv[0] << "\" file" << endl; return -1; }

    // Config file
    boost::filesystem::path configFilePath = argv[0];
    configFilePath = configFilePath.parent_path() / "config.txt";

    char linebuffer[1024];
    vector < string > patterns;
    vector < string > replacements;

    /* Config file format:
       pattern
       replacement
       pattern
       replacement
       ...                 */
    // Open config file
    ifstream ifile(configFilePath.string().c_str(), ifstream::in);

    if (ifile.good()) {
        uint8_t i = 0;
        while (ifile.getline(linebuffer, 1024)) {
            if (i == 0) {
                patterns.push_back(linebuffer);
                i++;
            } else {
                replacements.push_back(linebuffer);
                i = 0;
            }
        }
        ifile.close();
    } else {
        cerr << "Error: cannot open configuration file!" << endl; return -1; }

    if (patterns.size() != replacements.size()) {
        cerr << "Error: number of patterns does not match the number of replacements!"; return -1; }

    // Open given file
    int lenght;
    string str;
    char *filebuffer;

    ifile.open("file.txt", fstream::in | fstream::ate);

    if (ifile.good()) {
        lenght = ifile.tellg();
        filebuffer = new char[lenght];
        ifile.seekg(0);
        ifile.read(filebuffer, lenght);
        ifile.close();
    } else {
        cerr << "Error: cannot open given file for input!" << endl; return -1; }

    int replacedStrings = 0;
    str = filebuffer;
    delete[] filebuffer;
    for (unsigned i = 0; i < patterns.size(); i++)
        replacedStrings += replaceAll(str, patterns[i], replacements[i]);

    ofstream ofile("./file.txt", fstream::trunc);

    if (ofile.good()) {
        ofile << str;
        ofile.close();
    } else {
        cerr << "Error: cannot open given file for output!" << endl; return -1; }

    cout << "Operation completed successfully!" << endl << endl
         << "Patterns and replacements: " << patterns.size() << endl
         << "Replaced strings: " << replacedStrings << endl << endl
         << "Thanks for using m4tx's Search and Replace!" << endl;

    return 0;
}
