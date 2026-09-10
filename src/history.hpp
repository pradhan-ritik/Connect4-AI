#include "config.hpp"
#include "bit_operations.hpp"

struct History {
private:
    BB hist[2];
    uint8_t move_count;
    inline bool current_bitboard() { return move_count > 20; }
public:
    inline BB get_hist(int n) { return hist[n]; }

    History() : hist{0ULL, 0ULL}, move_count(0u) { }
    void append(BB col);
    inline uint8_t get_move_count() {
        return move_count;
    }
}; 
