#include <lightweight_logger.h>

int main() {

    lightweight_logger *l = new lightweight_logger();

    *l << lightweight_logger::lw_log_lvl::LW_ERROR << "azeazeaze";
    for (int a = 0; a < 5000; a++) {
        for (int b = 0; b < 1000000; b++) {
        }
    }

    *l << lightweight_logger::lw_log_lvl::LW_INFO << "qsdqsdqsd";

    for (int a = 0; a < 5000; a++) {
        for (int b = 0; b < 1000000; b++) {
        }
    }

    *l << lightweight_logger::lw_log_lvl::LW_WARNING << "wxcwxcwxc";

    delete l;

    return 0;
}