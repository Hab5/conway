#include "engine.hpp"

int Engine::getState(uint8_t *cells_) {
    return (*cells_ & 0x01);
}

void Engine::setCell(bool state, int x, int y) { 
    uint8_t *cell_ptr = cells + x + (y * width);

    // Neighbors coordinates, including wrapping around edges.
    int top = (y == 0) ? n_bytes-width : -width;
    int bottom = (y == (height-1)) ? -(n_bytes-width) : width;
    int left = (x == 0) ? width-1 : -1;
    int right = (x == (width-1)) ? -(width-1) : 1;
    
    // LSB holds bool, next 4 bits hold int (n alive neighbors), next 3 bits not used.
    *(cell_ptr) = (state == true) ? *(cell_ptr) | 0x01 : *(cell_ptr) & ~0x01;

    *(cell_ptr + top + left)      += (state == true) ? 0x02 : -0x02;
    *(cell_ptr + top)             += (state == true) ? 0x02 : -0x02;
    *(cell_ptr + top + right)     += (state == true) ? 0x02 : -0x02;
    *(cell_ptr + left)            += (state == true) ? 0x02 : -0x02;
    *(cell_ptr + right)           += (state == true) ? 0x02 : -0x02;
    *(cell_ptr + bottom + left)   += (state == true) ? 0x02 : -0x02;
    *(cell_ptr + bottom)          += (state == true) ? 0x02 : -0x02;
    *(cell_ptr + bottom + right)  += (state == true) ? 0x02 : -0x02;

    population += (state == true) ? 1 : -1;
}

void Engine::step(int start, int end) {
    uint8_t *cells_ptr = cells_buffer+(width*start);
    for (int y = start; y < end; y++) {
        for (int x = 0; x < width; x++) {
            if (*cells_ptr != 0) {
                int n = *(cells_ptr) >> 1;
                if ((getState(cells_ptr) == 1) && (!(n == 2 || n == 3)))
                    setCell(false, x, y);
                else if ((getState(cells_ptr) == 0) && (n == 3))
                    setCell(true, x, y);
            }
            cells_ptr++;
        }
    }
}

void Engine::threadStartEnd(int *start, int *end, int n_threads, int thread_n, int pass) {
    int arr[height/n_threads];
    for (int i = 0; i <= n_threads; i++)
        arr[i] = height*(float(i)/float(n_threads));
    *start = arr[pass+(thread_n*2)];
    *end = arr[pass+(thread_n*2)+1];     
}

void Engine::multiThreadStep() {
    int start = 0;
    int end = 0;

    int n_threads = 30;
    if (n_threads == 1)
        step(0, GRID_HEIGHT);
    else {
        for (int pass = 0; pass <= 1; pass++) {
            std::vector<std::thread> threads;
            for (int i = 0; i < n_threads/2; i++){
                threadStartEnd(&start, &end, n_threads, i, pass);
                threads.push_back(std::thread(&Engine::step, this, start, end));
            }
            for(auto& thread: threads) thread.join();
        }

    }
    iteration++;
    memcpy(cells_buffer, cells, n_bytes);
}

void Engine::initCells() {
    memset(cells, 0, n_bytes);
    memset(cells_buffer, 0, n_bytes);
    population = 0;
    iteration = 0;
    srand(time(NULL));
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            if ((rand() % 1) != 0) // 1/n cells.
                setCell(true, x, y);
    memcpy(cells_buffer, cells, n_bytes);
}