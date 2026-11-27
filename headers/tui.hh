#include <ncurses.h>
#include "headers/playback.hh"

class Tui{
private:
    int selected_song_index_ = 0;
    int row_;
    int col_;
    WINDOW *selector_win_;
    WINDOW *left_win_;
    WINDOW *right_win_;

public:
    Tui();
    ~Tui();

    /* rule of 5. */
    Tui(const Tui& x) = delete;
    Tui& operator=(const Tui& x) = delete;
    Tui(Tui&& x) noexcept = delete;
    Tui& operator=(Tui&& x) noexcept = delete;

    void redraw_ui(const Playback& playback);    
    void get_key(Playback& playback);
    void draw_deck(WINDOW *win, int deck_index, const Playback& playback);
};