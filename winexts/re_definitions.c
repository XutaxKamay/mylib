#include "main.h"

/**
 * Functions/symbols that the kernel doesn't export by default which is
 * needed by the kernel module
 */

struct tracepoint* __tracepoint_sched_process_fork_ptr = NULL;
unsigned long stack_guard_gap = 256UL << PAGE_SHIFT;

int find___tracepoint_sched_process_fork(void)
{
    __tracepoint_sched_process_fork_ptr = (ptr_t)kallsyms_lookup_name(
      "__tracepoint_sched_"
      "process_fork");

    if (__tracepoint_sched_process_fork_ptr == NULL)
    {
        return -1;
    }

    return 0;
}

#ifdef __arch_um__
void set_current(struct task_struct* task)
{
    static struct cpu_task* cpu_tasks = NULL;
    static int* __userspace_pid       = NULL;

    if (cpu_tasks == NULL)
    {
        cpu_tasks = (struct cpu_task*)kallsyms_lookup_name("cpu_tasks");
    }

    if (__userspace_pid == NULL)
    {
        __userspace_pid = (int*)kallsyms_lookup_name("userspace_pid");
    }

    cpu_tasks[task_thread_info(task)->cpu] = ((
      struct cpu_task) { __userspace_pid[0], task });
}
#endif

void vma_rb_erase(vm_area_t* vma, rb_root_t* root)
{
    typedef void (*vma_rb_erase_t)(vm_area_t*, rb_root_t*);
    static vma_rb_erase_t p_vma_rb_erase = NULL;

    if (p_vma_rb_erase == NULL)
    {
        p_vma_rb_erase = (vma_rb_erase_t)kallsyms_lookup_name("__vma_rb_"
                                                              "erase");
    }

    if (p_vma_rb_erase == NULL)
    {
        c_printk_error("couldn't find vma_rb_erase\n");
        return;
    }

    return p_vma_rb_erase(vma, root);
}

vm_area_t* vm_area_alloc(mm_t* mm)
{
    typedef vm_area_t* (*vm_area_alloc_t)(mm_t*);
    static vm_area_alloc_t p_vm_area_alloc = NULL;

    if (p_vm_area_alloc == NULL)
    {
        p_vm_area_alloc = (vm_area_alloc_t)kallsyms_lookup_name("vm_area_"
                                                                "alloc");
    }

    if (p_vm_area_alloc == NULL)
    {
        c_printk_error("couldn't find vm_area_alloc\n");
        return NULL;
    }

    return p_vm_area_alloc(mm);
}

void vm_area_free(vm_area_t* vma)
{
    typedef void (*vm_area_free_t)(vm_area_t*);
    static vm_area_free_t p_vm_area_free = NULL;

    if (p_vm_area_free == NULL)
    {
        p_vm_area_free = (vm_area_free_t)kallsyms_lookup_name("vm_area_"
                                                              "free");
    }

    if (p_vm_area_free == NULL)
    {
        c_printk_error("couldn't find vm_area_free\n");
        return;
    }

    p_vm_area_free(vma);
}

int insert_vm_struct(mm_t* mm, vm_area_t* vma)
{
    typedef int (*insert_vm_struct_t)(mm_t*, vm_area_t*);

    static insert_vm_struct_t p_insert_vm_struct = NULL;

    if (p_insert_vm_struct == NULL)
    {
        p_insert_vm_struct = (insert_vm_struct_t)kallsyms_lookup_name(
          "insert_vm_struct");
    }

    if (p_insert_vm_struct == NULL)
    {
        c_printk_error("couldn't find insert_vm_struct\n");
        return -1;
    }

    return p_insert_vm_struct(mm, vma);
}

unsigned long ksys_mmap_pgoff(unsigned long addr,
                              unsigned long len,
                              unsigned long prot,
                              unsigned long flags,
                              unsigned long fd,
                              unsigned long pgoff)
{
    typedef unsigned long (*ksys_mmap_pgoff_t)(unsigned long addr,
                                               unsigned long len,
                                               unsigned long prot,
                                               unsigned long flags,
                                               unsigned long fd,
                                               unsigned long pgoff);

    static ksys_mmap_pgoff_t p_ksys_mmap_pgoff = NULL;

    if (p_ksys_mmap_pgoff == NULL)
    {
        p_ksys_mmap_pgoff = (ksys_mmap_pgoff_t)kallsyms_lookup_name(
          "ksys_mmap_pgoff");
    }

    if (p_ksys_mmap_pgoff == NULL)
    {
        c_printk_error("couldn't find ksys_mmap_pgoff\n");
        return -1;
    }

    return p_ksys_mmap_pgoff(addr, len, prot, flags, fd, pgoff);
}

long _do_fork(struct kernel_clone_args* kargs)
{
    typedef long (*_do_fork_t)(struct kernel_clone_args * kargs);
    static _do_fork_t p__do_fork = NULL;

    if (p__do_fork == NULL)
    {
        p__do_fork = (_do_fork_t)kallsyms_lookup_name("_do_fork");
    }

    if (p__do_fork == NULL)
    {
        c_printk_error("couldn't find _do_fork\n");
        return -1;
    }

    return p__do_fork(kargs);
}

struct task_struct* __switch_to(struct task_struct* prev,
                                struct task_struct* next)
{
    typedef struct task_struct* (
      *__switch_to_t)(struct task_struct * prev,
                      struct task_struct * next);
    static __switch_to_t p___switch_to = NULL;

    if (p___switch_to == NULL)
    {
        p___switch_to = (__switch_to_t)kallsyms_lookup_name("__switch_"
                                                            "to");
    }

    if (p___switch_to == NULL)
    {
        c_printk_error("couldn't find __switch_to\n");
        return NULL;
    }

    return p___switch_to(prev, next);
}

struct task_struct* __switch_to_asm(struct task_struct* prev,
                                    struct task_struct* next)
{
    typedef struct task_struct* (
      *__switch_to_asm_t)(struct task_struct * prev,
                          struct task_struct * next);
    static __switch_to_asm_t p___switch_to_asm = NULL;

    if (p___switch_to_asm == NULL)
    {
        p___switch_to_asm = (__switch_to_asm_t)kallsyms_lookup_name(
          "__switch_to_asm");
    }

    if (p___switch_to_asm == NULL)
    {
        c_printk_error("couldn't find __switch_to_asm\n");
        return NULL;
    }

    return p___switch_to_asm(prev, next);
}

int __do_munmap(struct mm_struct* mm,
                unsigned long start,
                size_t len,
                struct list_head* uf,
                bool downgrade)
{
    typedef int (*__do_munmap_t)(struct mm_struct * mm,
                                 unsigned long start,
                                 size_t len,
                                 struct list_head* uf,
                                 bool downgrade);

    static __do_munmap_t p___do_munmap = NULL;

    if (p___do_munmap == NULL)
    {
        p___do_munmap = (__do_munmap_t)kallsyms_lookup_name("__do_"
                                                            "munmap");
    }

    if (p___do_munmap == NULL)
    {
        c_printk_error("couldn't find __do_munmap\n");
        return -1;
    }

    return p___do_munmap(mm, start, len, uf, downgrade);
}

int do_munmap(struct mm_struct* mm,
              unsigned long start,
              size_t len,
              struct list_head* uf)
{
    typedef int (*do_munmap_t)(struct mm_struct * mm,
                               unsigned long start,
                               size_t len,
                               struct list_head* uf);

    static do_munmap_t p_do_munmap = NULL;

    if (p_do_munmap == NULL)
    {
        p_do_munmap = (do_munmap_t)kallsyms_lookup_name("do_"
                                                        "munmap");
    }

    if (p_do_munmap == NULL)
    {
        c_printk_error("couldn't find do_munmap\n");
        return -1;
    }

    return p_do_munmap(mm, start, len, uf);
}

void set_task_cpu(struct task_struct* p, unsigned int cpu)
{
    typedef void (*set_task_cpu_t)(struct task_struct * p,
                                   unsigned int cpu);

    static set_task_cpu_t p_set_task_cpu = NULL;

    if (p_set_task_cpu == NULL)
    {
        p_set_task_cpu = (set_task_cpu_t)kallsyms_lookup_name("set_task_"
                                                              "cpu");
    }

    if (p_set_task_cpu == NULL)
    {
        c_printk_error("couldn't find set_task_cpu\n");
        return;
    }

    set_task_cpu(p, cpu);
}

void* vmalloc_exec(unsigned long size)
{
    typedef void* (*vmalloc_exec_t)(unsigned long size);

    static vmalloc_exec_t p_vmalloc_exec = NULL;

    if (p_vmalloc_exec == NULL)
    {
        p_vmalloc_exec = (vmalloc_exec_t)kallsyms_lookup_name("vmalloc_"
                                                              "exec");
    }

    if (p_vmalloc_exec == NULL)
    {
        c_printk_error("couldn't find vmalloc_exec\n");
        return NULL;
    }

    return p_vmalloc_exec(size);
}

struct task_struct* copy_process(struct pid* pid,
                                 int trace,
                                 int node,
                                 struct kernel_clone_args* args)
{
    typedef struct task_struct* (
      *copy_process_t)(struct pid*, int, int, struct kernel_clone_args*);

    static copy_process_t copy_process_ptr = NULL;

    if (copy_process_ptr == NULL)
    {
        copy_process_ptr = (copy_process_t)kallsyms_lookup_name(
          "copy_process");
    }

    if (copy_process_ptr == NULL)
    {
        c_printk_error("couldn't find copy_process\n");
        return NULL;
    }

    return copy_process_ptr(pid, trace, node, args);
}

void wake_up_new_task(struct task_struct* tsk)
{
    typedef void (*wake_up_new_task_t)(struct task_struct*);

    static wake_up_new_task_t p_wake_up_new_task = NULL;

    if (p_wake_up_new_task == NULL)
    {
        p_wake_up_new_task = (wake_up_new_task_t)kallsyms_lookup_name(
          "wake_up_new_task");
    }

    if (p_wake_up_new_task == NULL)
    {
        c_printk_error("couldn't find wake_up_new_task\n");
        return;
    }

    p_wake_up_new_task(tsk);
}

void ptrace_notify(int exit_code)
{
    typedef void (*ptrace_notify_t)(int);

    static ptrace_notify_t p_ptrace_notify = NULL;

    if (p_ptrace_notify == NULL)
    {
        p_ptrace_notify = (ptrace_notify_t)kallsyms_lookup_name("ptrace_"
                                                                "notify");
    }

    if (p_ptrace_notify == NULL)
    {
        c_printk_error("couldn't find ptrace_notify\n");
        return;
    }

    p_ptrace_notify(exit_code);
}

void cgroup_update_frozen(struct cgroup* cgrp)
{
    typedef void (*cgroup_update_frozen_t)(void*);
    static cgroup_update_frozen_t cgroup_update_frozen_ptr = NULL;

    if (cgroup_update_frozen_ptr == NULL)
    {
        cgroup_update_frozen_ptr = (cgroup_update_frozen_t)
          kallsyms_lookup_name("cgroup_update_frozen");
    }

    if (cgroup_update_frozen_ptr == NULL)
    {
        c_printk_error("couldn't find cgroup_update_frozen\n");
        return;
    }

    cgroup_update_frozen_ptr(cgrp);
}

void task_clear_jobctl_trapping(struct task_struct* task)
{
    typedef void (*task_clear_jobctl_trapping_t)(void*);
    static task_clear_jobctl_trapping_t task_clear_jobctl_trapping_ptr
      = NULL;

    if (task_clear_jobctl_trapping_ptr == NULL)
    {
        task_clear_jobctl_trapping_ptr = (task_clear_jobctl_trapping_t)
          kallsyms_lookup_name("task_clear_jobctl_trapping");
    }

    if (task_clear_jobctl_trapping_ptr == NULL)
    {
        c_printk_error("couldn't find "
                       "task_clear_jobctl_trapping\n");
        return;
    }

    task_clear_jobctl_trapping_ptr(task);
}

void task_clear_jobctl_pending(struct task_struct* task,
                               unsigned long mask)
{
    typedef void (*task_clear_jobctl_pending_t)(void*, unsigned long);
    static task_clear_jobctl_pending_t task_clear_jobctl_pending_ptr
      = NULL;

    if (task_clear_jobctl_pending_ptr == NULL)
    {
        task_clear_jobctl_pending_ptr = (task_clear_jobctl_pending_t)
          kallsyms_lookup_name("task_clear_jobctl_pending");
    }

    if (task_clear_jobctl_pending_ptr == NULL)
    {
        c_printk_error("couldn't find task_clear_jobctl_pending\n");
        return;
    }

    task_clear_jobctl_pending_ptr(task, mask);
}

int __group_send_sig_info(int i,
                          struct kernel_siginfo* sig,
                          struct task_struct* tsk)
{
    typedef int (*__group_send_sig_info_t)(int, void*, void*);
    static __group_send_sig_info_t __group_send_sig_info_ptr = NULL;

    if (__group_send_sig_info_ptr == NULL)
    {
        __group_send_sig_info_ptr = (__group_send_sig_info_t)
          kallsyms_lookup_name("__group_send_sig_info");
    }

    if (__group_send_sig_info_ptr == NULL)
    {
        c_printk_error("couldn't find __group_send_sig_info\n");
        return -1;
    }

    return __group_send_sig_info_ptr(i, sig, tsk);
}

void __wake_up_parent(struct task_struct* p, struct task_struct* parent)
{
    typedef void (*__wake_up_parent_t)(void*, void*);
    static __wake_up_parent_t __wake_up_parent_ptr = NULL;

    if (__wake_up_parent_ptr == NULL)
    {
        __wake_up_parent_ptr = (__wake_up_parent_t)kallsyms_lookup_name(
          "__wake_up_parent");
    }

    if (__wake_up_parent_ptr == NULL)
    {
        c_printk_error("couldn't find __wake_up_parent\n");
        return;
    }

    return __wake_up_parent_ptr(p, parent);
}

void task_work_run(void)
{
    typedef void (*task_work_run_t)(void);
    static task_work_run_t task_work_run_ptr = NULL;

    if (task_work_run_ptr == NULL)
    {
        task_work_run_ptr = (task_work_run_t)kallsyms_lookup_name(
          "task_work_run");
    }

    if (task_work_run_ptr == NULL)
    {
        c_printk_error("couldn't find task_work_run\n");
        return;
    }

    return task_work_run_ptr();
}

u64 nsec_to_clock_t(u64 x)
{
    typedef u64 (*nsec_to_clock_t_t)(u64);
    static nsec_to_clock_t_t nsec_to_clock_t_ptr = NULL;

    if (nsec_to_clock_t_ptr == NULL)
    {
        nsec_to_clock_t_ptr = (nsec_to_clock_t_t)kallsyms_lookup_name(
          "nsec_to_clock_t");
    }

    if (nsec_to_clock_t_ptr == NULL)
    {
        c_printk_error("couldn't find nsec_to_clock_t\n");
        return 0;
    }

    return nsec_to_clock_t_ptr(x);
}

mm_t* mm_access(task_t* task, unsigned int mode)
{
    mm_t* mm;

    typedef mm_t* (*mm_access_t)(task_t*, unsigned int);

    static mm_access_t p_mm_access = NULL;

    if (p_mm_access == NULL)
    {
        p_mm_access = (mm_access_t)kallsyms_lookup_name("mm_access");
    }

    if (p_mm_access == NULL)
    {
        c_printk_error("couldn't find mm_access\n");
        return NULL;
    }

    mm = p_mm_access(task, mode);

    return mm;
}

void __memcg_kmem_uncharge(struct page* page, int order)
{
    typedef void (*__memcg_kmem_uncharge_t)(struct page * page,
                                            int order);
    static __memcg_kmem_uncharge_t __memcg_kmem_uncharge_ptr = NULL;

    if (__memcg_kmem_uncharge_ptr == NULL)
    {
        __memcg_kmem_uncharge_ptr = (__memcg_kmem_uncharge_t)
          kallsyms_lookup_name("__memcg_kmem_uncharge");
    }

    if (__memcg_kmem_uncharge_ptr == NULL)
    {
        c_printk_error("couldn't find __memcg_kmem_uncharge\n");
        return;
    }

    __memcg_kmem_uncharge_ptr(page, order);
}

int arch_dup_task_struct(struct task_struct* dst, struct task_struct* src)
{
    typedef int (*arch_dup_task_struct_t)(struct task_struct * dst,
                                          struct task_struct * src);

    static arch_dup_task_struct_t arch_dup_task_struct_ptr = NULL;

    if (arch_dup_task_struct_ptr == NULL)
    {
        arch_dup_task_struct_ptr = (arch_dup_task_struct_t)
          kallsyms_lookup_name("arch_dup_task_struct");
    }

    if (arch_dup_task_struct_ptr == NULL)
    {
        c_printk_error("couldn't find arch_dup_task_struct\n");
        return -1;
    }

    return arch_dup_task_struct_ptr(dst, src);
}

void set_task_stack_end_magic(struct task_struct* tsk)
{
    typedef void (*func_t)(void*);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("set_task_stack_end_"
                                                "magic");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find set_task_stack_end_magic\n");
        return;
    }

    func_ptr(tsk);
}

void __mod_memcg_state(struct mem_cgroup* memcg, int idx, int val)
{
    typedef void (*func_t)(void*, int, int);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("__mod_memcg_state");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find __mod_memcg_state\n");
        return;
    }

    func_ptr(memcg, idx, val);
}

struct vm_struct* find_vm_area(const void* addr)
{
    typedef void* (*func_t)(const void*);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("find_vm_area");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find find_vm_area\n");
        return NULL;
    }

    return func_ptr(addr);
}

void* __vmalloc_node_range(unsigned long size,
                           unsigned long align,
                           unsigned long start,
                           unsigned long end,
                           gfp_t gfp_mask,
                           pgprot_t prot,
                           unsigned long vm_flags,
                           int node,
                           const void* caller)
{
    typedef void* (*func_t)(unsigned long,
                            unsigned long,
                            unsigned long,
                            unsigned long,
                            gfp_t,
                            pgprot_t,
                            unsigned long,
                            int,
                            const void*);

    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("__vmalloc_node_range");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find __vmalloc_node_range\n");
        return NULL;
    }

    return func_ptr(size,
                    align,
                    start,
                    end,
                    gfp_mask,
                    prot,
                    vm_flags,
                    node,
                    caller);
}

void mod_memcg_obj_state(void* p, int idx, int val)
{
    typedef void (*func_t)(void*, int, int);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("mod_memcg_obj_state");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find mod_memcg_obj_state\n");
        return;
    }

    return func_ptr(p, idx, val);
}

int sched_fork(unsigned long clone_flags, struct task_struct* p)
{
    typedef int (*func_t)(unsigned long, void*);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("sched_fork");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find sched_fork\n");
        return -1;
    }

    return func_ptr(clone_flags, p);
}

int tsk_fork_get_node(struct task_struct* tsk)
{
    typedef int (*func_t)(void*);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("tsk_fork_get_node");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find tsk_fork_get_node\n");
        return -1;
    }

    return func_ptr(tsk);
}

void vfree_atomic(const void* addr)
{
    typedef void (*func_t)(const void*);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("vfree_atomic");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find vfree_atomic\n");
        return;
    }

    func_ptr(addr);
}

int __memcg_kmem_charge(struct page* page, gfp_t gfp, int order)
{
    typedef int (*func_t)(void*, gfp_t, int);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("__memcg_kmem_charge");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find __memcg_kmem_charge\n");
        return -1;
    }

    return func_ptr(page, gfp, order);
}

void put_task_stack(struct task_struct* tsk)
{
    typedef void (*func_t)(void*);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("put_task_stack");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find put_task_stack\n");
        return;
    }

    func_ptr(tsk);
}

struct pid* alloc_pid(struct pid_namespace* ns,
                      pid_t* set_tid,
                      size_t set_tid_size)
{
    typedef void* (*func_t)(void*, void*, size_t);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("alloc_pid");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find alloc_pid\n");
        return NULL;
    }

    return func_ptr(ns, set_tid, set_tid_size);
}

int copy_namespaces(unsigned long flags, struct task_struct* tsk)
{
    typedef int (*func_t)(unsigned long, void*);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("copy_namespaces");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find copy_namespaces\n");
        return -1;
    }

    return func_ptr(flags, tsk);
}

void cgroup_enter_frozen(void)
{
    typedef void (*func_t)(void);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("cgroup_enter_frozen");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find cgroup_enter_frozen\n");
        return;
    }

    func_ptr();
}

void cgroup_leave_frozen(bool always_leave)
{
    typedef void (*func_t)(bool);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("cgroup_leave_frozen");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find cgroup_leave_frozen\n");
        return;
    }

    func_ptr(always_leave);
}

void __mpol_put(struct mempolicy* p)
{
    typedef void (*func_t)(void*);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("__mpol_put");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find __mpol_put\n");
        return;
    }

    func_ptr(p);
}

int force_sig_info(struct kernel_siginfo* info)
{
    typedef int (*func_t)(void*);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("force_sig_info");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find force_sig_info\n");
        return -1;
    }

    return func_ptr(info);
}

const char* arch_vma_name(struct vm_area_struct* vma)
{
    typedef void* (*func_t)(void*);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("arch_vma_name");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find arch_vma_name\n");
        return NULL;
    }

    return func_ptr(vma);
}

void dump_mm(const struct mm_struct* mm)
{
    typedef void (*func_t)(const void*);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("dump_mm");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find dump_mm\n");
        return;
    }

    return func_ptr(mm);
}

long populate_vma_page_range(struct vm_area_struct* vma,
                             unsigned long start,
                             unsigned long end,
                             int* nonblocking)
{
    typedef long (*func_t)(void*, unsigned long, unsigned long, int*);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("populate_vma_page_"
                                                "range");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find populate_vma_page_range\n");
        return 0;
    }

    return func_ptr(vma, start, end, nonblocking);
}

bool may_expand_vm(struct mm_struct* mm,
                   vm_flags_t flags,
                   unsigned long npages)
{
    typedef bool (*func_t)(void*, vm_flags_t, unsigned long);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("may_expand_vm");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find may_expand_vm\n");
        return false;
    }

    return func_ptr(mm, flags, npages);
}

int vma_wants_writenotify(struct vm_area_struct* vma,
                          pgprot_t vm_page_prot)
{
    typedef int (*func_t)(void*, pgprot_t);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("vma_wants_writenotify");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find vma_wants_writenotify\n");
        return -1;
    }

    return func_ptr(vma, vm_page_prot);
}

int walk_page_range(struct mm_struct* mm,
                    unsigned long start,
                    unsigned long end,
                    const struct mm_walk_ops* ops,
                    void* private)
{
    typedef int (
      *func_t)(void*, unsigned long, unsigned long, const void*, void*);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("walk_page_range");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find walk_page_range\n");
        return -1;
    }

    return func_ptr(mm, start, end, ops, private);
}

int security_file_mprotect(struct vm_area_struct* vma,
                           unsigned long reqprot,
                           unsigned long prot)
{
    typedef int (*func_t)(void*, unsigned long, unsigned long);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("security_file_mprotect");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find security_file_mprotect\n");
        return -1;
    }

    return func_ptr(vma, reqprot, prot);
}

struct vm_area_struct* vma_merge(struct mm_struct* mm,
                                 struct vm_area_struct* prev,
                                 unsigned long addr,
                                 unsigned long end,
                                 unsigned long vm_flags,
                                 struct anon_vma* anon_vma,
                                 struct file* file,
                                 pgoff_t pgoff,
                                 struct mempolicy* mempolicy,
                                 struct vm_userfaultfd_ctx ctx)
{
    typedef void* (*func_t)(void*,
                            void*,
                            unsigned long,
                            unsigned long,
                            unsigned long,
                            void*,
                            void*,
                            pgoff_t,
                            void*,
                            struct vm_userfaultfd_ctx);

    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("vma_merge");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find vma_merge\n");
        return NULL;
    }

    return func_ptr(mm,
                    prev,
                    addr,
                    end,
                    vm_flags,
                    anon_vma,
                    file,
                    pgoff,
                    mempolicy,
                    ctx);
}

int security_vm_enough_memory_mm(struct mm_struct* mm, long pages)
{
    typedef int (*func_t)(void*, long);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("security_vm_enough_"
                                                "memory_mm");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find security_vm_enough_memory_mm\n");
        return -1;
    }

    return func_ptr(mm, pages);
}

unsigned long change_protection(struct vm_area_struct* vma,
                                unsigned long start,
                                unsigned long end,
                                pgprot_t newprot,
                                int dirty_accountable,
                                int prot_numa)
{
    typedef unsigned long (
      *func_t)(void*, unsigned long, unsigned long, pgprot_t, int, int);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("change_protection");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find change_protection\n");
        return -1;
    }

    return func_ptr(vma,
                    start,
                    end,
                    newprot,
                    dirty_accountable,
                    prot_numa);
}

void vma_set_page_prot(struct vm_area_struct* vma)
{
    typedef void (*func_t)(void*);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("vma_set_page_prot");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find vma_set_page_prot\n");
        return;
    }

    func_ptr(vma);
}

bool pfn_modify_allowed(unsigned long pfn, pgprot_t prot)
{
    typedef bool (*func_t)(unsigned long, pgprot_t);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("pfn_modify_allowed");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find pfn_modify_allowed\n");
        return false;
    }

    return func_ptr(pfn, prot);
}

int split_vma(struct mm_struct* mm,
              struct vm_area_struct* vma,
              unsigned long addr,
              int new_below)
{
    typedef int (*func_t)(void*, void*, unsigned long, int);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("split_vma");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find split_vma\n");
        return -1;
    }

    return func_ptr(mm, vma, addr, new_below);
}

int __arch_override_mprotect_pkey(struct vm_area_struct* vma,
                                  int prot,
                                  int pkey)
{
    typedef int (*func_t)(void*, int, int);
    static func_t func_ptr = NULL;

    if (func_ptr == NULL)
    {
        func_ptr = (func_t)kallsyms_lookup_name("__arch_override_"
                                                "mprotect_pkey");
    }

    if (func_ptr == NULL)
    {
        c_printk_error("couldn't find __arch_override_mprotect_pkey\n");
        return -1;
    }

    return func_ptr(vma, prot, pkey);
}

/**
 * Code
 * Credits to linux kernel developers
 */

static inline unsigned long vma_compute_gap(struct vm_area_struct* vma)
{
    unsigned long gap, prev_end;

    /*
     * Note: in the rare case of a VM_GROWSDOWN above a VM_GROWSUP, we
     * allow two stack_guard_gaps between them here, and when choosing
     * an unmapped area; whereas when expanding we only require one.
     * That's a little inconsistent, but keeps the code here simpler.
     */
    gap = vm_start_gap(vma);
    if (vma->vm_prev)
    {
        prev_end = vm_end_gap(vma->vm_prev);
        if (gap > prev_end)
            gap -= prev_end;
        else
            gap = 0;
    }
    return gap;
}

static unsigned long vma_compute_subtree_gap(struct vm_area_struct* vma)
{
    unsigned long max = vma_compute_gap(vma), subtree_gap;
    if (vma->vm_rb.rb_left)
    {
        subtree_gap = rb_entry(vma->vm_rb.rb_left,
                               struct vm_area_struct,
                               vm_rb)
                        ->rb_subtree_gap;
        if (subtree_gap > max)
            max = subtree_gap;
    }
    if (vma->vm_rb.rb_right)
    {
        subtree_gap = rb_entry(vma->vm_rb.rb_right,
                               struct vm_area_struct,
                               vm_rb)
                        ->rb_subtree_gap;
        if (subtree_gap > max)
            max = subtree_gap;
    }
    return max;
}

static int browse_rb(struct mm_struct* mm)
{
    struct rb_root* root = &mm->mm_rb;
    int i = 0, j, bug = 0;
    struct rb_node *nd, *pn = NULL;
    unsigned long prev = 0, pend = 0;

    for (nd = rb_first(root); nd; nd = rb_next(nd))
    {
        struct vm_area_struct* vma;
        vma = rb_entry(nd, struct vm_area_struct, vm_rb);
        if (vma->vm_start < prev)
        {
            pr_emerg("vm_start %lx < prev %lx\n", vma->vm_start, prev);
            bug = 1;
        }
        if (vma->vm_start < pend)
        {
            pr_emerg("vm_start %lx < pend %lx\n", vma->vm_start, pend);
            bug = 1;
        }
        if (vma->vm_start > vma->vm_end)
        {
            pr_emerg("vm_start %lx > vm_end %lx\n",
                     vma->vm_start,
                     vma->vm_end);
            bug = 1;
        }
        spin_lock(&mm->page_table_lock);
        if (vma->rb_subtree_gap != vma_compute_subtree_gap(vma))
        {
            pr_emerg("free gap %lx, correct %lx\n",
                     vma->rb_subtree_gap,
                     vma_compute_subtree_gap(vma));
            bug = 1;
        }
        spin_unlock(&mm->page_table_lock);
        i++;
        pn   = nd;
        prev = vma->vm_start;
        pend = vma->vm_end;
    }
    j = 0;
    for (nd = pn; nd; nd = rb_prev(nd))
        j++;
    if (i != j)
    {
        pr_emerg("backwards %d, forwards %d\n", j, i);
        bug = 1;
    }
    return bug ? -1 : i;
}

void validate_mm(struct mm_struct* mm)
{
    int bug                       = 0;
    int i                         = 0;
    unsigned long highest_address = 0;
    struct vm_area_struct* vma    = mm->mmap;

    while (vma)
    {
        struct anon_vma* anon_vma = vma->anon_vma;
        struct anon_vma_chain* avc;

        if (anon_vma)
        {
            anon_vma_lock_read(anon_vma);
            list_for_each_entry(avc, &vma->anon_vma_chain, same_vma)
              anon_vma_unlock_read(anon_vma);
        }

        highest_address = vm_end_gap(vma);
        vma             = vma->vm_next;
        i++;
    }
    if (i != mm->map_count)
    {
        pr_emerg("map_count %d vm_next %d\n", mm->map_count, i);
        bug = 1;
    }
    if (highest_address != mm->highest_vm_end)
    {
        pr_emerg("mm->highest_vm_end %lx, found %lx\n",
                 mm->highest_vm_end,
                 highest_address);
        bug = 1;
    }
    i = browse_rb(mm);
    if (i != mm->map_count)
    {
        if (i != -1)
            pr_emerg("map_count %d rb %d\n", mm->map_count, i);
        bug = 1;
    }
    VM_BUG_ON_MM(bug, mm);
}

void vm_stat_account(struct mm_struct* mm, vm_flags_t flags, long npages)
{
    mm->total_vm += npages;

    if (is_exec_mapping(flags))
        mm->exec_vm += npages;
    else if (is_stack_mapping(flags))
        mm->stack_vm += npages;
    else if (is_data_mapping(flags))
        mm->data_vm += npages;
}
