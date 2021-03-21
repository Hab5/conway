#ifndef PATTERN_EXPLORER_H
#define PATTERN_EXPLORER_H

#include <vector>
#include <algorithm>
#include <string>
#include <filesystem>

#include "../lib/imgui/imgui.h"
#include "../lib/imgui/imgui-SFML.h"

// #include "render.h"

class PatternExplorer
{
    public:
        std::vector<std::string> *clipboard;
        PatternExplorer(){
            fetchPatternsFiles("./patterns");
        }
    
        void fetchPatternsFiles(const char *path);
        void show(bool *p_open);
    private:
        std::vector<std::string> paths;
};

std::vector<std::string> rleParser(char const *filename);
#endif