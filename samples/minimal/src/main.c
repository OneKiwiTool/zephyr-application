#include <zephyr/kernel.h>

int main(void)
{
    printk("Hello World! %s\n", CONFIG_BOARD);

    while (1) {
    }
    return 0;
}

