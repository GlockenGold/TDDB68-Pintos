#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include <stdbool.h>

void syscall_init (void);

typedef int pid_t;

pid_t exec (const char *cmd_line);

void halt(void);

bool create(const char *file, unsigned initial_size);

int open(const char *file);

void close(int fd);

int read(int fd, void *buffer, unsigned size);

int write(int fd, const void *buffer, unsigned size);

int wait(pid_t pid);

void exit(int status);

bool is_valid_fd(int fd);

bool is_valid_ptr(const void *ptr);

bool is_valid_buff(const void *buff, unsigned size);

bool is_valid_string(const char *string);

#endif /* userprog/syscall.h */
