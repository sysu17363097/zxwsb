# zxwsb
asmlinkage long sys_mycall(void)
{
	struct task_struct *task;
	task = get_current();
	struct sched_entity sen;
	sen = task -> se;
	printk(KERN_INF0 "se.exec_start:%u\n",sen.exec_start);
	printk(KERN_INF0 "se.vruntime:%u\n",sen.vruntime);
	printk(KERN_INF0 "se.sum_exec_runtime:%u\n",sen.sum_exec_runtime);
	printk(KERN_INF0 "se.nr_migrations:%u\n",sen.nr_migrations);
	printk(KERN_INF0 "se.load.weight:%d\n",sen.load.weight);
	printk(KERN_INF0 "se.avg.load_sum:%d\n",sen.load.weight);
	printk(KERN_INF0 "se.avg.util_sum:%d\n",sen.avg.util_sum);
	printk(KERN_INF0 "se.avg.load_avg:%d\n",sen.avg.load_avg);
	printk(KERN_INF0 "se.avg.util_avg:%d\n",sen.avg.util_avg);
	printk(KERN_INF0 "se.avg.last_update_time:%d\n",sen.avg.last_update_time);
}
