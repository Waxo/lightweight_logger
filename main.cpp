#include <lightweight_logger.h>

void example_use() {
    lightweight_logger *l = new lightweight_logger(lightweight_logger::lwl_lvl::LWL_TRACE);
//    lightweight_logger *l = new lightweight_logger(lightweight_logger::lwl_lvl::LWL_TRACE, "out");

    *l << lightweight_logger::lwl_lvl::LWL_ERROR << "azeazeaze\t" << __FILE__ << std::endl;

    for (int a = 0; a < 500; a++) {
        for (int b = 0; b < 1000000; b++) {
        }
    }

    *l << lightweight_logger::lwl_lvl::LWL_INFO << "qsdqsdqsd " << 12 << " " << std::endl;

    for (int a = 0; a < 500; a++) {
        for (int b = 0; b < 1000000; b++) {
        }
    }

    *l << lightweight_logger::lwl_lvl::LWL_WARNING << "wxcwxcwxc" << std::endl;

    delete l;
}

int main() {
    example_use();

    return 0;
}