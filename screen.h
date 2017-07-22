#ifndef SCREEN_H
#define SCREEN_H
#include <sys/ioctl.h>
#include <unistd.h>
#include <algorithm>
using namespace std;
typedef int cols;
typedef int rows;

pair<cols, rows> win_sz(){
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    int col = ws.ws_col;
    int row = ws.ws_row;
    return make_pair(col, row);
}

#endif // SCREEN_H
