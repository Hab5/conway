#include "render.hpp"

bool PAUSED = false;
bool EXIT = false;

ImVec4 ColorFromBytes(uint8_t r, uint8_t g, uint8_t b) {
    return ImVec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f);
}

void Render::setImguiStyle() {
    auto& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    const ImVec4 bgColor           = ColorFromBytes(37, 37, 38);
    const ImVec4 lightBgColor      = ColorFromBytes(82, 82, 85);
    const ImVec4 veryLightBgColor  = ColorFromBytes(90, 90, 95);

    const ImVec4 panelColor        = ColorFromBytes(51, 51, 55);
    const ImVec4 panelHoverColor   = ColorFromBytes(29, 151, 236);
    const ImVec4 panelActiveColor  = ColorFromBytes(0, 119, 200);

    const ImVec4 textColor         = ColorFromBytes(255*.80, 255*.80, 255*.80);
    const ImVec4 textDisabledColor = ColorFromBytes(151, 151, 151);
    const ImVec4 borderColor       = ColorFromBytes(78, 78, 78);

    colors[ImGuiCol_Text]                 = textColor;
    colors[ImGuiCol_TextDisabled]         = textDisabledColor;
    colors[ImGuiCol_TextSelectedBg]       = panelActiveColor;
    colors[ImGuiCol_WindowBg]             = bgColor;
    colors[ImGuiCol_ChildBg]              = bgColor;
    colors[ImGuiCol_PopupBg]              = bgColor;
    colors[ImGuiCol_Border]               = borderColor;
    colors[ImGuiCol_BorderShadow]         = borderColor;
    colors[ImGuiCol_FrameBg]              = panelColor;
    colors[ImGuiCol_FrameBgHovered]       = lightBgColor;//panelHoverColor;
    colors[ImGuiCol_FrameBgActive]        = lightBgColor;//panelActiveColor;
    colors[ImGuiCol_TitleBg]              = bgColor;
    colors[ImGuiCol_TitleBgActive]        = bgColor;
    colors[ImGuiCol_TitleBgCollapsed]     = bgColor;
    colors[ImGuiCol_MenuBarBg]            = panelColor;
    colors[ImGuiCol_ScrollbarBg]          = panelColor;
    colors[ImGuiCol_ScrollbarGrab]        = lightBgColor;
    colors[ImGuiCol_ScrollbarGrabHovered] = veryLightBgColor;
    colors[ImGuiCol_ScrollbarGrabActive]  = veryLightBgColor;
    colors[ImGuiCol_CheckMark]            = panelActiveColor;
    colors[ImGuiCol_SliderGrab]           = panelHoverColor;
    colors[ImGuiCol_SliderGrabActive]     = panelActiveColor;
    colors[ImGuiCol_Button]               = panelColor;
    colors[ImGuiCol_ButtonHovered]        = panelHoverColor;
    colors[ImGuiCol_ButtonActive]         = panelHoverColor;
    colors[ImGuiCol_Header]               = panelColor;
    colors[ImGuiCol_HeaderHovered]        = panelHoverColor;
    colors[ImGuiCol_HeaderActive]         = panelActiveColor;
    colors[ImGuiCol_Separator]            = borderColor;
    colors[ImGuiCol_SeparatorHovered]     = borderColor;
    colors[ImGuiCol_SeparatorActive]      = borderColor;
    colors[ImGuiCol_ResizeGrip]           = bgColor;
    colors[ImGuiCol_ResizeGripHovered]    = panelColor;
    colors[ImGuiCol_ResizeGripActive]     = lightBgColor;
    colors[ImGuiCol_PlotLines]            = panelActiveColor;
    colors[ImGuiCol_PlotLinesHovered]     = panelHoverColor;
    colors[ImGuiCol_PlotHistogram]        = panelActiveColor;
    colors[ImGuiCol_PlotHistogramHovered] = panelHoverColor;
    // colors[ImGuiCol_ModalWindowDarkening] = bgColor;
    colors[ImGuiCol_DragDropTarget]       = bgColor;
    colors[ImGuiCol_NavHighlight]         = bgColor;
    // colors[ImGuiCol_DockingPreview]       = panelActiveColor;
    colors[ImGuiCol_Tab]                  = bgColor;
    colors[ImGuiCol_TabActive]            = panelActiveColor;
    colors[ImGuiCol_TabUnfocused]         = bgColor;
    colors[ImGuiCol_TabUnfocusedActive]   = panelActiveColor;
    colors[ImGuiCol_TabHovered]           = panelHoverColor;

    style.WindowRounding    = 0.0f;
    style.ChildRounding     = 0.0f;
    style.FrameRounding     = 0.0f;
    style.GrabRounding      = 0.0f;
    style.PopupRounding     = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.TabRounding       = 0.0f;
}

void Render::mainMenuBar() {
    static bool show_pattern_explorer = false;
    if (show_pattern_explorer) UI_PatternExplorer.show(&show_pattern_explorer);
    
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Modules")) {
            ImGui::MenuItem("Pattern Explorer", NULL, &show_pattern_explorer);
            ImGui::EndMenu();
        }

        ImGui::Separator();
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetWindowWidth()*0.10);
        ImGui::SliderFloat("", &engine_delay, 0.0f, 1.0f, "Delay: %.2f sec");
        ImGui::Separator();
        if (ImGui::Button("Clear"))
            Cells.initCells();
        ImGui::Separator();
        
        char pop_iter_pos[1024];
        sprintf(pop_iter_pos, "Population: %d | Iteration: %d | %d,%d", 
                Cells.population, Cells.iteration, user.rel_x, user.rel_y);
        ImGui::SameLine(ImGui::GetWindowWidth()-ImGui::CalcTextSize(pop_iter_pos).x-15);
        ImGui::Text("%s", pop_iter_pos);
        ImGui::EndMainMenuBar();
    }
}

void Render::updateCamera() {
    int nCellsInViewportWidth  = (int)(Window.getSize().x) / cell_size;
    int nCellsInViewportHeight = (int)(Window.getSize().y) / cell_size;
    
    camera = {
        .x        = (Cells.width - nCellsInViewportWidth) / 2 + camera.offset_x,
        .y        = (Cells.height - nCellsInViewportHeight) / 2 + camera.offset_y,
        .w        = nCellsInViewportWidth,
        .h        = nCellsInViewportHeight,
        .offset_x = camera.offset_x,
        .offset_y = camera.offset_y
    };

    // printf("%d, %d, %d, %d\n", camera.x, camera.y, camera.offset_x, camera.offset_y);
    
    if (camera.x + camera.w > Cells.width) {
        camera.offset_x -= camera.x + camera.w - Cells.width;
        camera.x = (Cells.width - nCellsInViewportWidth) / 2 + camera.offset_x;
    }
    if (camera.y + camera.h > Cells.height) {
        camera.offset_y -= camera.y + camera.h - Cells.height;
        camera.y = (Cells.height - nCellsInViewportHeight) / 2 + camera.offset_y;
    }
    if (camera.x < 0) {
        camera.offset_x -= camera.x;
        camera.x = 0;
    }
    if (camera.y < 0) {
        camera.offset_y -= camera.y;
        camera.y = 0;
    }
}

void Render::updateMouse(int mouseX, int mouseY) {
    user.x = mouseX;
    user.y = mouseY;
    user.rel_x = mouseX / cell_size + camera.x;
    user.rel_y = mouseY / cell_size + camera.y;
}

void Render::selectionCopy() {   
    std::string tmp;

    int box_w = (user.selection_box.w > 0) ?
        user.selection_box.w :
        -user.selection_box.w;

    int box_h = (user.selection_box.h > 0) ?
        user.selection_box.h :
        -user.selection_box.h;

    int box_x = (user.selection_box.w > 0) ? 
        user.selection_box.x : 
        user.selection_box.x - box_w;

    int box_y = (user.selection_box.h > 0) ? 
        user.selection_box.y : 
        user.selection_box.y - box_h;

    for (int y = 0; y <= box_h; y++) {
        for (int x = 0; x <= box_w; x++) {
            uint8_t *cell = Cells.cells + (box_x + x) + ((box_y + y) * Cells.width);
            if (Cells.getState(cell) == 1)
                tmp.append("1");
            else
                tmp.append("0");
        }
        tmp.append("\n");
    }
    clipboard.clear();
    std::replace(tmp.begin(), tmp.end(), '\n', ' ');
    std::stringstream ss(tmp);
    std::string row;
    while(ss >> row)
        clipboard.push_back(row);
}

void Render::selectionCut() {
    selectionCopy();

    int box_w = (user.selection_box.w > 0) ?
        user.selection_box.w :
        -user.selection_box.w;

    int box_h = (user.selection_box.h > 0) ?
        user.selection_box.h :
        -user.selection_box.h;

    int box_x = (user.selection_box.w > 0) ? 
        user.selection_box.x : 
        user.selection_box.x - box_w;

    int box_y = (user.selection_box.h > 0) ? 
        user.selection_box.y : 
        user.selection_box.y - box_h;

    user.selection_box = {0};
    user.select_mode = false;

    for (int i = 0; i <= box_h; i++)
        for (int j = 0; j <= box_w; j++)
            if (Cells.getState(Cells.cells+(box_x+j)+(box_y+i)*Cells.width))
                Cells.setCell(false, box_x+j, box_y+i);
}

void Render::loadPattern(std::vector<std::string> pattern, int x, int y) {
    int w = Cells.width;
    int h = Cells.height;
    int pattern_w = (x+pattern[0].length() > w) ? w-x : pattern[0].length();
    int pattern_h = (y+pattern.size() > h) ? h-y : pattern.size();

    for (int i = 0; i < pattern_h; i++)
        for (int j = 0; j < pattern_w; j++)
            if (pattern[i][j] == '1' && !(Cells.getState(Cells.cells+(x+j)+(y+i)*w)))
                Cells.setCell(true, x+j, y+i);
}


void Render::drawErase() {
    if (!Cells.getState(Cells.cells + user.rel_x + 
            user.rel_y * Cells.width) && !user.erase_mode) {
        user.draw_mode = true;
        Cells.setCell(true, user.rel_x, user.rel_y);
    }
    if (Cells.getState(Cells.cells + user.rel_x + 
            user.rel_y * Cells.width) && !user.draw_mode) {
        user.erase_mode = true;
        Cells.setCell(false, user.rel_x, user.rel_y);
    }
}

void Render::zoom() {
    if (Event.mouseWheelScroll.delta > 0) {
        if (cell_size == 2) cell_margin = 1;
        cell_size = (cell_size < 32 && cell_size!=1) ? cell_size*2 : cell_size;
        if (cell_size == 1) cell_size++;
    }
    if (Event.mouseWheelScroll.delta < 0) {
        cell_size = (cell_size != 1) ? cell_size/2 : cell_size;
        cell_margin = (cell_size <= 2) ? 0 : 1;
    }

    int oldX = user.rel_x;
    int oldY = user.rel_y;
    int nCellsInViewportWidth  = (int)(Window.getSize().x) / cell_size; 
    int nCellsInViewportHeight = (int)(Window.getSize().y) / cell_size;
    int cam_x = (Cells.width - nCellsInViewportWidth) / 2 + camera.offset_x;
    int cam_y = (Cells.height - nCellsInViewportHeight) / 2 + camera.offset_y;
    int rel_x = sf::Mouse::getPosition(Window).x / cell_size + cam_x;
    int rel_y = sf::Mouse::getPosition(Window).y / cell_size + cam_y;
    
    camera.offset_x += oldX - rel_x; // Pan camera.
    camera.offset_y += oldY - rel_y;
}

void Render::handleEvent() {
    ImGuiIO &io = ImGui::GetIO();
    while (Window.pollEvent(Event)) {
        ImGui::SFML::ProcessEvent(Event);
        updateMouse(sf::Mouse::getPosition(Window).x,
                    sf::Mouse::getPosition(Window).y);
        switch (Event.type) {
            case (sf::Event::Closed):
                Window.close();

            case (sf::Event::MouseButtonPressed): {
                if (!io.WantCaptureMouse) {
                    if (Event.mouseButton.button == sf::Mouse::Left) {   
                        if (user.select_mode == true)
                            user.selection_box = {0};
                        if(!user.lclick_down) {
                            user.lclick_down = true;
                            PAUSED = true;
                        }
                    }
                    if (Event.mouseButton.button == sf::Mouse::Middle)
                        user.mclick_down = true;
                    if (Event.mouseButton.button == sf::Mouse::Right)
                        user.rclick_down = true;
                }
                break;
            }

            case (sf::Event::MouseButtonReleased): {
                if (Event.mouseButton.button == sf::Mouse::Left) {
                    if (!io.WantCaptureMouse && !user.select_mode)
                        drawErase();
                    user.lclick_down = false;
                    user.draw_mode = false;
                    user.erase_mode = false;
                    user.select_mode = false;
                }
                if (Event.mouseButton.button == sf::Mouse::Middle)
                    user.mclick_down = false;
                if (Event.mouseButton.button == sf::Mouse::Right)
                    user.rclick_down = false;
                
                break;
            }

            case (sf::Event::KeyPressed): {
                if (Event.key.code == sf::Keyboard::Escape)
                    Window.close();
                if (Event.key.code == sf::Keyboard::Space && !io.WantCaptureKeyboard)
                    PAUSED = !PAUSED;
                if (Event.key.code == sf::Keyboard::Return && PAUSED && !io.WantCaptureKeyboard)
                    Cells.multiThreadStep();
                if (Event.key.code == sf::Keyboard::X && user.select_mode == true)
                    selectionCut();
                if (Event.key.code == sf::Keyboard::C && user.select_mode == true)
                    selectionCopy();
                if (Event.key.code == sf::Keyboard::V)
                    loadPattern(clipboard, user.rel_x, user.rel_y);
                break;
            }

            case (sf::Event::MouseMoved): {
                if (user.lclick_down == true)
                    drawErase();
                if (user.mclick_down == true) { // Pan
                    camera.offset_x += user.last_x/cell_size - user.x/cell_size;
                    camera.offset_y += user.last_y/cell_size - user.y/cell_size;
                }
                if (user.rclick_down == true) { // Selection Box
                    if (user.select_mode == false) {
                        user.select_mode = true;
                        user.selection_box.x = user.rel_x;
                        user.selection_box.y = user.rel_y;
                        user.selection_box.w = 0;
                        user.selection_box.h = 0;
                    } 
                    else {
                        user.selection_box.w = user.rel_x - user.selection_box.x;
                        user.selection_box.h = user.rel_y - user.selection_box.y;
                    }

                }
                user.last_x = user.x;
                user.last_y = user.y;
                break;
            }

            case (sf::Event::MouseWheelScrolled): {
                if (!io.WantCaptureMouse)
                    zoom();
                break;
            }
            default:
                break;
        }
    }
}

void Render::renderCells() {
    cell_rect.setSize(sf::Vector2f(cell_size-cell_margin, cell_size-cell_margin));
    
    for (int y = 0; y < camera.h; y++) {
        for (int x = 0; x < camera.w; x++) {
            if (Cells.getState(Cells.cells+(camera.x+x)+((camera.y+y)*Cells.width))) {
                cell_rect.setPosition(sf::Vector2f(x*cell_size, y*cell_size));
                Window.draw(cell_rect);
            }
        }
    }
}

void Render::renderGrid() {
    if (cell_size >= 3) {
        sf::RectangleShape line;
        line.setFillColor(sf::Color(55, 55, 55));
        
        line.setSize(sf::Vector2f(1, Window.getSize().y));
        for (int x = 0; x < camera.w; x++) {
            if (x*cell_size % cell_size == 0) {
                line.setPosition(sf::Vector2f(x*cell_size-1, 0));
                if (((x + camera.x) % (10) == 0)) {
                    line.setFillColor(sf::Color(70, 70, 70));
                    line.setPosition(sf::Vector2f(x*cell_size-1, 0));
                    Window.draw(line);
                    line.setFillColor(sf::Color(55, 55, 55));
                }
                else
                    Window.draw(line);
            }
        }

        line.setSize(sf::Vector2f(Window.getSize().x, 1)); 
        for (int y = 0; y < camera.h; y++) {
            if ((y*cell_size) % cell_size == 0) {
                line.setPosition(sf::Vector2f(0, y*cell_size-1));
                if (((y + camera.y) % (10) == 0)) {
                    line.setFillColor(sf::Color(70, 70, 70));
                    line.setPosition(sf::Vector2f(0, y*cell_size-1));
                    Window.draw(line);
                    line.setFillColor(sf::Color(55, 55, 55));
                }
                else 
                    Window.draw(line);
            }
        }
    }
}

void Render::renderSelectionBox() { 
    int draw_x = (user.selection_box.w < 0) ? 1 : 0;
    int draw_y = (user.selection_box.h < 0) ? 1 : 0;
    int draw_w = (user.selection_box.w < 0) ? -1 : 1;
    int draw_h = (user.selection_box.h < 0) ? -1 : 1;
    
    draw_x += user.selection_box.x - camera.x;
    draw_y += user.selection_box.y - camera.y;
    draw_w += user.selection_box.w;
    draw_h += user.selection_box.h;

    selection_rect.setPosition(sf::Vector2f(
        draw_x * cell_size,
        draw_y * cell_size
    ));
    selection_rect.setSize(sf::Vector2f(
        draw_w * cell_size,
        draw_h * cell_size
    ));
    Window.draw(selection_rect);
}

void Render::renderUI() {
    setImguiStyle();
    // ImGui::ShowDemoWindow();
    mainMenuBar();
}

void Render::renderAll() {
    Window.clear(sf::Color(44, 48, 54));
    updateCamera();
    renderGrid();
    renderSelectionBox();
    renderCells();
    renderUI();
}


void Render::run() {
    handleEvent();
    elapsed += Clock.getElapsedTime();
    ImGui::SFML::Update(Window, Clock.restart());
    renderAll();
    ImGui::SFML::Render(Window);
    Window.display();
    if (!PAUSED && elapsed.asSeconds() > engine_delay) {
        Cells.multiThreadStep();
        elapsed = sf::seconds(0);
    }
}