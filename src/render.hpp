#ifndef RENDER_H
#define RENDER_H

#include <iostream>

#include <SFML/Graphics.hpp>

#include "../lib/imgui/imgui.h"
#include "../lib/imgui/imgui-SFML.h"

#include "engine.hpp"
#include "pattern_explorer.hpp"

class Render
{
    public:
        sf::RenderWindow            Window;
        sf::Event                   Event;
        sf::Clock                   Clock;
        sf::RectangleShape          cell_rect;
        sf::RectangleShape          selection_rect;
        sf::Time                    elapsed;
        float                       engine_delay;

        Engine&                     Cells;
        std::vector<std::string>    clipboard;
        

        Render(Engine& Cells_): Cells(Cells_) {
            Window.create(sf::VideoMode(1500, 1000), "Game of Life", sf::Style::Close);
            ImGui::SFML::Init(Window);
            
            cell_size = 4;
            cell_margin = 1;
            engine_delay = 0;
            
            cell_rect.setSize(sf::Vector2f(cell_size,cell_size));
            cell_rect.setFillColor(sf::Color(22, 160, 154, 150));

            selection_rect.setSize(sf::Vector2f(0,0));
            selection_rect.setFillColor(sf::Color(22, 160, 154, 40));
            UI_PatternExplorer.clipboard = &clipboard;
        }

        void updateCamera();
        void updateMouse(int mouseX, int mouseY);
        void selectionCopy();
        void selectionCut();
        void loadPattern(std::vector<std::string> pattern, int x, int y);
        void drawErase();
        void zoom();
        void handleEvent();
        void setImguiStyle();
        void mainMenuBar();
        
        void renderCells();
        void renderGrid();
        void renderSelectionBox();
        void renderUI();
        void renderAll();

        void run();
        

    private:
        int cell_size;
        int cell_margin;

        struct Area {
            int x;
            int y;
            int w;
            int h;
            int offset_x;
            int offset_y;
        };
        Area camera {0};

        struct Interactivity {
            int x;
            int y;
            int last_x;
            int last_y;
            int rel_x;
            int rel_y;
            bool draw_mode;
            bool erase_mode;
            bool select_mode;
            bool lclick_down;
            bool rclick_down;
            bool mclick_down;
            bool lshift_down;
            Area selection_box;
        };
        Interactivity user {0};
        
        PatternExplorer UI_PatternExplorer;
};
#endif