#include "headers/tui.hh"
#include <algorithm>

#define LEFT_DECK 0
#define RIGHT_DECK 1


Tui::Tui() {
    initscr();
    cbreak(); /* directly pass user input, except control characters (e.g. ctrl-C, ctrl-Z) */
    noecho(); /* don't echo chars back to user */
    timeout(33); /* 30 FPS */
    keypad(stdscr, TRUE); /* enable arrow keys */

    getmaxyx(stdscr, row_, col_);	
    selector_win_ = newwin(row_, col_/3, 0, col_/3);
    left_win_ = newwin(row_, col_/3, 0, 0);
    right_win_ = newwin(row_, col_/3, 0, 2 * (col_/3));
}

Tui::~Tui() {
    delwin(selector_win_);
    delwin(left_win_);
    delwin(right_win_);
    endwin();
}

void Tui::redraw_ui(const Playback& playback) {
    werase(stdscr);
    werase(selector_win_);
    werase(left_win_);
    werase(right_win_);

    if (row_ < 16 || col_ < 64) {
        constexpr std::string_view mesg = "please enlarge window";
        mvprintw(row_/2, col_/2 - (mesg.size()/2), "%s", mesg);
        refresh();
        return;
    }
    wnoutrefresh(stdscr);

    box(left_win_, 0, 0);
    draw_deck(left_win_, LEFT_DECK, playback);
    wnoutrefresh(left_win_);

    box(right_win_, 0, 0);
    draw_deck(right_win_, RIGHT_DECK, playback);
    wnoutrefresh(right_win_);
    
    /* selector logic */
    box(selector_win_, 0, 0);
    int s_row, s_col;
    getmaxyx(selector_win_, s_row, s_col);

    const std::vector<std::string>& songs = playback.get_song_names();
    int limit = std::min(static_cast<int>(songs.size()), s_row - 6);
    int start = songs.size() <= s_row - 6 ? 0 : selected_song_index_ - (s_row - 6) + 1;
    start = std::max(start, 0);

    mvwaddnstr(selector_win_, 2, 2, "arrow up/down to navigate", s_col - 4);
    mvwaddnstr(selector_win_, 3, 2, "arrow left/right to load track", s_col - 4);

    for (int i = start; i < start + limit; i++) {
        if (i == selected_song_index_) {
            wattron(selector_win_, A_REVERSE);
            mvwaddnstr(selector_win_, i - start + 5, 2, songs[i].c_str(), s_col - 4);
            wattroff(selector_win_, A_REVERSE);
        }
        else {
            mvwaddnstr(selector_win_, i - start + 5, 2, songs[i].c_str(), s_col - 4);
        }
    }
    wnoutrefresh(selector_win_);

    doupdate();
}

void Tui::draw_deck(WINDOW *win, int deck_index, const Playback& playback) {
    int row, col;
    getmaxyx(win, row, col);

    const Deck& deck = playback.get_deck(deck_index);
    int song_index = deck.get_song_index();
    std::string_view song_name = song_index == -1 ? std::string_view{""} : playback.get_song_names()[song_index];
    mvwprintw(win, row - 3, 2, "now loaded:");
    mvwaddnstr(win, row - 2, 2, song_name.data(), col - 4);

    if (deck_index == LEFT_DECK) {
        mvwprintw(win, 2, 2, "volume up:   d");
        mvwprintw(win, 3, 2, "volume down: c");
        mvwprintw(win, row - 8, 2, "reset track: a");
        mvwprintw(win, row - 7, 2, "play/pause:  z");
    }
    else if (deck_index == RIGHT_DECK) {
        mvwprintw(win, 2, 2, "volume up:   h");
        mvwprintw(win, 3, 2, "volume down: b");
        mvwprintw(win, row - 8, 2, "reset track: k");
        mvwprintw(win, row - 7, 2, "play/pause:  m");
    }
    mvwprintw(win, 5, 2, "cur volume:  %.1f", deck.get_volume());

    if (deck.is_playing()) {
        mvwprintw(win, row - 5, 2, "is playing: %s", "ya");
    }
    else {
        mvwprintw(win, row - 5, 2, "is playing: %s", "no");
    }
}

void Tui::get_key(Playback& playback) {

    int ch = getch(); 
    if (ch == ERR) {
        return;
    }

    /* resize redraws */
    else if (ch == KEY_RESIZE) {
        getmaxyx(stdscr, row_, col_);	

        wresize(selector_win_, row_, col_/3);
        mvwin(selector_win_, 0, col_/3);
        werase(selector_win_);
        wrefresh(selector_win_);

        wresize(left_win_, row_, col_/3);
        mvwin(left_win_, 0, 0);
        werase(left_win_);
        wrefresh(left_win_);

        wresize(right_win_, row_, col_/3);
        mvwin(right_win_, 0, 2 * (col_/3));
        werase(right_win_);
        wrefresh(right_win_);
    }




    /* define that 0 is left deck and 1 is right deck !!!!! */

    /* song select */
    else if (ch == KEY_UP) {
        if (selected_song_index_ > 0) {
            selected_song_index_--;
        }
    }
    else if (ch == KEY_DOWN) {
        int size = playback.get_song_names().size();
        if (size > 0 && selected_song_index_ < size - 1) {
            selected_song_index_++;
        }
    }
    else if (ch == KEY_LEFT) {
        int size = playback.get_song_names().size();
        if (size > 0) {
            playback.get_deck(0).set_song_index(selected_song_index_); 
        }
    }
    else if (ch == KEY_RIGHT) {
        int size = playback.get_song_names().size();
        if (size > 0) {
            playback.get_deck(1).set_song_index(selected_song_index_); 
        }
    }
    
    /* left deck */
    else if (ch == 'z') {
        playback.get_deck(0).play_pause();
    }
    else if (ch == 'a') {
        playback.get_deck(0).reset();
    }
    else if (ch == 'd') {
        playback.get_deck(0).update_volume(0.1);
    }
    else if (ch == 'c') {
        playback.get_deck(0).update_volume(-0.1);
    }

    /* right deck */
    else if (ch == 'm') {
        playback.get_deck(1).play_pause();
    }
    else if (ch == 'k') {
        playback.get_deck(1).reset();
    }
    else if (ch == 'h') {
        playback.get_deck(1).update_volume(0.1);
    }
    else if (ch == 'b') {
        playback.get_deck(1).update_volume(-0.1);
    }

}

