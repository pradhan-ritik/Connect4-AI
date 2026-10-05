#pragma once
#include "config.hpp"

inline BB bb(uint pos) {
    return 1ULL << pos;
}

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

inline uint msb(BB bitboard) {
    return 63 - __builtin_clzll(bitboard);
}

inline uint pop_msb(BB &bitboard) {
    uint last = msb(bitboard);
    bitboard ^= bb(last);
    return last;
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

inline uint column(Move pos) {
    assert((pos & 0b111) < nCOLUMNS);
    return pos & 0b111;
}

inline uint row(Move pos) {
    return pos / STRIDE;
}

inline void print_BB(BB bitboard) {
    bool cur;

    for (int row = nROWS - 1; row >= 0; --row) {
        for (int col = nCOLUMNS - 1; col >= 0; --col) {
            uint pos = row * STRIDE + col;

            cur = is_bit_active(bitboard, pos);
            std::cout << cur << " ";
        }

        std::cout << "\n";
    }

    std::cout << "\n";
}

inline void print_BB64(BB bitboard) {
    bool cur;
    for (int i = 63; i > -1; i--) {
        cur = is_bit_active(bitboard, i);
        std::cout << cur << " ";
        if (i % 8 == 0) {
            std::cout << "\n";
        }
    }
    std::cout << "\n";
}

inline bool move_in_range(Move m) { 
    return /*0 <=*/m <= 6;
}
