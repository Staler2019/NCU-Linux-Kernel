#include <linux/init_task.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <../kernel/sched/sched.h>


asmlinkage unsigned long sys_get_CPU_number(unsigned long x) {
    int cnt = 0;
    struct task_struct *t;
    printk("current->pid = %d\n", current->pid);
    for_each_thread(current, t) {
        int is_running = t->se.cfs_rq->curr == &t->se;  // should be same as t->on_cpu
        printk("compare %p %p\n", t->se.cfs_rq->curr, &t->se);
        printk("thread: %s, pid: %d, tgid: %d, state: %ld, on_cpu: %d, is_running: %d\n", t->comm, t->pid, t->tgid, t->state, t->on_cpu, is_running);
        if (is_running) {
            cnt++;
        }
    }
	return cnt;
}
