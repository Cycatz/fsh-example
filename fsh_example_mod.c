#include <linux/init.h>
#include <linux/module.h>
#include "fsh.h"


FSH_SYSCALL_OVERRIDE(sys_execve)
{
    char filename[256] = {0};
    static int a = 0;

    if (strncpy_from_user(filename, (const char *) *FSH_FUNC_ARG1,
                          sizeof(filename) - 1) < 0) {
        return true;
    }

    pr_info("fsh: %s is executed!\n", filename);

    /* Two methods returning back */

    /* Method 1: Invoke the syscall manually and then return to the syscall
     * caller */
    if (a++ % 2 == 0) {
        int retval = FSH_CALL_ORIG_SYSCALL();
        pr_info("fsh: execve return value: %d\n", retval);
        /* Set the return value using FSH_SET_RETVAL(retval) */
        FSH_SET_RETVAL(retval);
        return false;
    }

    /* Method 2: Continue to execute the system call  */
    return true;
}

FSH_SYSCALL_OVERRIDE(sys_openat)
{
    char filename[256] = {0};
    const uid_t uid = 1000;

    if (current->cred->uid.val != uid) return true;

    if (strncpy_from_user(filename, (const char *) *FSH_FUNC_ARG2,
                          sizeof(filename) - 1) < 0) {
        return true;
    }

    pr_info("fsh: file %s opened by uid %u\n", filename,
            current->cred->uid.val);
    return true;
}

static int __init fsh_example_module_init(void)
{
    fsh_init();
    return 0;
}

static void __exit fsh_example_module_exit(void)
{
    fsh_exit();
}

module_init(fsh_example_module_init);
module_exit(fsh_example_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cycatz");
MODULE_DESCRIPTION(
    "A kernel module prints the program name being execved using ftrace");
