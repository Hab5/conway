#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>
#include <string>
#include <random>
#include <unistd.h>
#include <thread>
#include <time.h>
#include <bitset>
#include <stdlib.h>
#include <filesystem>

#include <SFML/Graphics.hpp>

#include "../lib/imgui/imgui.h"
#include "../lib/imgui/imgui-SFML.h"

#include "engine.hpp"
#include "render.hpp"

const int CELL_SIZE = 1;
const int CELL_MARGIN = 0;
const int SCREEN_WIDTH = 1500;
const int SCREEN_HEIGHT = 1000;

std::vector<std::string> rleParser(char const *filename) {
    std::ifstream in_file(filename);
    // if (!in_file.is_open())
    //     throw "Couldn't find pattern file. Verify name.";


    std::vector<std::string> pattern;
    std::string to_parse;
    int width, height;

    for(std::string line; getline(in_file, line);) {
        int i = 0;

        if (line[0] == '#') // Skip comments.
            continue;

        else if (line[0] == 'x') { // Get width and height of pattern.
            std::string tmp = line.substr(line.find("x"), line.find(","));
            for (; i < tmp.length(); i++){ if ( isdigit(tmp[i]) ) break; }
            width = atoi(tmp.substr(i, tmp.length() - i).c_str());

            tmp = line.substr(line.find("y"), line.substr(line.find("y")).find(","));
            for (i = 0; i < tmp.length(); i++){ if ( isdigit(tmp[i]) ) break; }
            height = atoi(tmp.substr(i, tmp.length() - i).c_str());
        }

        else { // Append RLE lines.
            if (line.back() == 13) // Carriage return (windows' newline).
                to_parse.append(line.substr(0, line.length()-1));
            else if (line.back() == '\n')
                to_parse.append(line.substr(0, line.length()-1));
            else
                to_parse.append(line);
        }   
    }

    int n_repeat;
    std::string tmp;

    for (int i = 0; to_parse[i]; i++) { // RLE string parsing.
        if (to_parse[i] == '!') {tmp.push_back('\n'); break;};
        if (to_parse[i] == '$')
            tmp.push_back('\n');
        if (to_parse[i] == 'o' || to_parse[i] ==  'b')
                tmp.push_back((to_parse[i] == 'o') ? '1' : '0');
        if (isdigit(to_parse[i])) {
            int nb_len = 0;
            for (int j=0; isdigit(to_parse[i+j]); j++) nb_len++;
            int nb = atoi(to_parse.substr(i, nb_len).c_str());
            if (to_parse[i+1] == '$') {
                for(int j = 0; j<nb; j++) {
                    tmp.push_back('\n');
                    tmp.append(std::string((nb-1), '0'));
                }
            }
            else
                tmp.append(std::string(nb-1, (to_parse[i+nb_len] == 'o') ? '1' : '0'));
            i += nb_len-1;
        }
    }

    std::replace(tmp.begin(), tmp.end(), '\n', ' ');
    std::stringstream ss(tmp);
    std::string row;
    while(ss >> row) { // Pad implied dead cells for early newline.
        int i = 0;
        for (i = 0; i < row.length() ; i++) ;
        if (i < width) row.append(std::string(width-i, '0'));
        pattern.push_back(row);
    }
    return pattern;
}

int main(int argc, char **argv)
{
    Engine Cells;
    Render GameOfLife(Cells);

    while (GameOfLife.Window.isOpen())
       GameOfLife.run();

    ImGui::SFML::Shutdown();
}