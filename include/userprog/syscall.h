#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include "threads/synch.h"
void syscall_init (void);


typedef int pid_t;
int process_add_file(struct file *f);
struct file *process_get_file(int fd);
void process_close_file(int fd);
/*경원이가 추가하라함 ㅎ */
struct lock filesys_lock;

#endif /* userprog/syscall.h */
