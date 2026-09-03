#pragma once
#include "config.hpp"

inline uint count_bits(BB bitboard) {
    return __builtin_popcountll(bitboard);
}

inline uint lsb(BB bitboard) {
    return __builtin_ctzll(bitboard);
}

inline uint pop_lsb(BB& bitboard) {
    uint first = lsb(bitboard);
    bitboard &= bitboard - 1;
    return first;
}

inline BB bb(uint pos) {
    return 1ULL << pos;
}

inline bool is_bit_active(BB bitboard, uint index) {
    return bitboard & bb(index);
}

inline void set_bit_on(BB& bitboard, uint index) {
    bitboard |= bb(index);
}

inline void set_bit_off(BB& bitboard, uint index) {
    bitboard &= ~bb(index);
}

inline void toggle_bit(BB& bitboard, uint index) {
    bitboard ^= bb(index);
}

inline void print_BB(BB bitboard) {
    bool cur;
    for (int i = nSQUARES-1; i > -1; i--) {
        cur = is_bit_active(bitboard, i);
        std::cout << cur << " ";
        if (i % nCOLUMNS == 0) {
            std::cout << "\n";
        }
    }
    std::cout << "\n";
}
