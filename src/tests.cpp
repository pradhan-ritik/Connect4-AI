#include "tests.hpp"

void test_BBs() {
    print_BB(ROW_1);
    print_BB(ROW_2);
    print_BB(ROW_3);
    print_BB(ROW_4);
    print_BB(ROW_5);
    print_BB(ROW_6);
    print_BB(A_COL);
    print_BB(B_COL);
    print_BB(C_COL);
    print_BB(D_COL);
    print_BB(E_COL);
    print_BB(F_COL);
    print_BB(G_COL);
    BB test = 0x73ULL;
    print_BB(test);
    print_BB(bb(lsb(test)));
    std::cout << count_bits(test) << " <- bitcount\n";
    std::cout << pop_lsb(test) << " <- pop_lsb\n";
    print_BB(test);
    std::cout << "on: 2 off: 1 toggle: 0\n";
    set_bit_on(test, 2);
    set_bit_off(test, 1);
    toggle_bit(test, 0);
    print_BB(test);
}
