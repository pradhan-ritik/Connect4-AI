#include "config.hpp"
#include "bit_operations.hpp"

struct History {
private:
    BB hist[2];
    uint8_t move_count;

    inline bool current_bitboard(bool last_move=false) { return (move_count-last_move) > 20; }
    inline uint8_t offset(bool current_bitboard) { return 3*(move_count - 21*current_bitboard); }
public:
    // temp
    inline BB get_hist(bool n) { return hist[n]; }

    History() : hist{0ULL, 0ULL}, move_count(0u) { }
    void append(BB col);
    Move get_last();
    Move pop_last();

    inline uint8_t get_move_count() {
        return move_count;
    }
}; 
