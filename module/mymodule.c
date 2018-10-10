#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Robert W. Oliver II");
MODULE_DESCRIPTION("A simple example Linux module.");
MODULE_VERSION("0.01");

static int __init mymodule_init(void) {
 pr_info("Hello, World!\n");
 return 0;
}
static void __exit mymodule_exit(void) {
 pr_info ("Goodbye, World!\n");
}
module_init(mymodule_init);
module_exit(mymodule_exit);
