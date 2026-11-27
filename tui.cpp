#include "headers/tui.hh"

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
    endwin();
}

void Tui::redraw_ui(const Playback& playback) {
    erase();

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
    
    box(selector_win_, 0, 0);
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
        mvwprintw(win, 5, 2, "cur volume:  %.1f", deck.get_volume());
        mvwprintw(win, row - 6, 2, "reset track: a");
        mvwprintw(win, row - 5, 2, "play/pause:  z");
    }
    else if (deck_index == RIGHT_DECK) {
        mvwprintw(win, 2, 2, "volume up:   h");
        mvwprintw(win, 3, 2, "volume down: b");
        mvwprintw(win, 5, 2, "cur volume:  %.1f", deck.get_volume());
        mvwprintw(win, row - 6, 2, "reset track: k");
        mvwprintw(win, row - 5, 2, "play/pause:  m");
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
        if (selected_song_index_ < playback.get_song_names().size() - 1) {
            selected_song_index_++;
        }
    }
    else if (ch == KEY_LEFT) {
        playback.get_deck(0).set_song_index(selected_song_index_); 
    }
    else if (ch == KEY_RIGHT) {
        playback.get_deck(1).set_song_index(selected_song_index_); 
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

