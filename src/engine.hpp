#ifndef ENGINE_H
#define ENGINE_H

#include <stdint.h>
#include <string.h>
#include <vector>
#include <thread>
#include <random>

const int GRID_WIDTH = 1500;
const int GRID_HEIGHT = 1000;

class Engine 
{
    public:
        uint8_t *cells, *cells_buffer;
        int height, width, n_bytes;
        int iteration, population;
        
        Engine() {
            width = GRID_WIDTH;
            height = GRID_HEIGHT;
            n_bytes = width * height;
            iteration = 0;
            population = 0;
            cells = new uint8_t[n_bytes];
            cells_buffer = new uint8_t[n_bytes];
            initCells();
        }

        int getState(uint8_t *cells_);
        void setCell(bool state, int x, int y);
        void step(int start, int end);
        void threadStartEnd(int *start, int *end, int n_threads, int thread_n, int pass);
        void multiThreadStep();
        void initCells();

        ~Engine() {delete[] cells; delete[] cells_buffer;}
};

#endif