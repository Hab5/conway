#include "pattern_explorer.hpp"

void PatternExplorer::fetchPatternsFiles(const char *path) {
    for (const auto& entry: std::filesystem::recursive_directory_iterator(path))
        if(entry.path().string().find(".rle") != std::string::npos)
            paths.push_back(entry.path().string());
    std::sort(paths.begin(), paths.end());
}
#include <iostream>
#include <fstream>

void PatternExplorer::show(bool *p_open) {
        std::vector<std::string> lookup;
        ImGui::SetNextWindowSize(ImVec2(320, 480));//, ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Pattern Explorer", p_open)) {
            static int selected = 0;
            static char buffer[128] = "Search";
            ImGui::BeginChild("Left pane", ImVec2(300, 420), true); int i = 0;
            {
                for (const auto& path: paths)
                    if (path.find(buffer) != std::string::npos)
                        lookup.push_back(path);
                
                for (const auto& path: lookup) {
                    char label[128];
                    sprintf(label, "%s", path.c_str()+path.find_last_of('/')+1);
                    if (ImGui::Selectable(label, selected == i))
                        selected = i;
                    i++;
                }
                ImGui::EndChild();
            }
            ImGui::PushItemWidth(250);
            ImGui::InputText("", buffer, IM_ARRAYSIZE(buffer));
            
            std::string target = (lookup.size() > selected) ? lookup[selected]:"None";
            std::vector<std::string> parsed_target = rleParser(target.c_str());
            
            ImGui::SameLine();
            if (ImGui::Button("Copy"))
                    *clipboard = parsed_target;
            ImGui::End();
        }
    }
