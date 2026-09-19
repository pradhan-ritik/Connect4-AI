#include "config.hpp"
#include "bit_operations.hpp"
#include "tests.hpp"
#include "board.hpp"
#include "interface.hpp"


int main() {
    // test_board();
    Board b;
    Interface interface(&b);
    interface.run();
    // b.print_board();
    return 0;
}
