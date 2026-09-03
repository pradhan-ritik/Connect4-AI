#pragma once
#include "config.hpp"

inline int count_bits(BB bitboard) {
    return __builtin_popcountll(bitboard);
}

inline int lsb(BB bitboard) {
    return __builtin_ctzll(bitboard);
}

inline int pop_lsb(BB& bitboard) {
    int first = lsb(bitboard);
    bitboard &= bitboard - 1;
    return first;
}

inline BB bb(int pos) {
    return 1ULL << pos;
}

inline bool get_bit(BB bitboard, int index) {
    return bitboard & bb(index);
}

inline void set_bit_on(BB& bitboard, int index) {
    bitboard |= bb(index);
}

inline void set_bit_off(BB& bitboard, int index) {
    bitboard &= ~bb(index);
}

inline void toggle_bit(BB& bitboard, int index) {
    bitboard ^= bb(index);
}

inline void print_BB(BB bitboard) {
    bool cur;
    for (int i = nSQUARES-1; i > -1; i--) {
        cur = get_bit(bitboard, i);
        std::cout << cur << " ";
        if (i % nCOLUMNS == 0) {
            std::cout << "\n";
        }
    }
    std::cout << "\n";
}
