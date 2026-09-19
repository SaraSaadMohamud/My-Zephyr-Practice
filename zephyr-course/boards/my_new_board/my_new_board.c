#include <zephyr/init.h>
#include <zephyr/sys/printk.h>

static int my_new_board_init(void)
{
	printk("Board Initialized\n");

	return 0;
}

SYS_INIT(my_new_board_init, APPLICATION, 0);
