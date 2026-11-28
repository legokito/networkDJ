#include <atomic>

class Grid {
private:
    std::atomic<int64_t> total_frame_counter_ = 0;
    int BPM = 128;

public:
    int64_t get_total_frame_counter();
    const int64_t get_total_frame_counter() const; 
    void set_total_frame_counter(int64_t frames);
    void update_total_frame_counter(int64_t frames);

    int get_BPM();
};