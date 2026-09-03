#include "tests.hpp"

void test_BBs() {
    std::cout << 15 << " row: " << row(15) << " column: " << column(15) << "\n";
    print_BB(ROW[row(15)] ^ COLUMN[column(15)]);
    print_BB(ROW_1);
    print_BB(ROW_2);
    print_BB(ROW_3);
    print_BB(ROW_4);
    print_BB(ROW[R5]);
    print_BB(ROW_6);
    print_BB(A_COL);
    print_BB(B_COL);
    print_BB(COLUMN[C]);
    print_BB(D_COL);
    print_BB(E_COL);
    print_BB(F_COL);
    print_BB(G_COL);
    BB test = 0x73ULL;
    print_BB(test);
    print_BB(bb(lsb(test)));
    print_BB(bb(msb(test)));
    std::cout << count_bits(test) << " <- bitcount\n";
    std::cout << pop_lsb(test) << " <- pop_lsb\n";
    std::cout << pop_msb(test) << " <- pop_msb\n";
    print_BB(test);
    std::cout << "on: 2 off: 1 toggle: 0\n";
    set_bit_on(test, 2);
    set_bit_off(test, 1);
    toggle_bit(test, 0);
    print_BB(test);
}

void test_board() {
    Board b;
    b.print_board();
    std::cout << "\n\n\n";
    b.make_move(1);
    b.make_move(1);
    b.make_move(1);
    std::cout << "\n\n\n";
    b.print_board();
}
