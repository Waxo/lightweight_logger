#include <lightweight_logger.h>

int main() {

    lightweight_logger *l = new lightweight_logger();
    l->add_message("azeaze");
    l->add_message("wxcwxc");
    l->add_message("qsdqsd");

    sleep(6);
    l->add_message("vbnvbnvbn");


sleep(30);


    delete l;

    return 0;
}