#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"
#include "filesys/filesys.h"
#include "filesys/file.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void)
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED)
{
  switch(*(uint32_t *)f->esp){
    case SYS_HALT:
      printf("HALT\n");
      halt();
      break;
    case SYS_EXIT:
      printf("EXIT\n");
      //exit();
      break;
    case SYS_WAIT:
      printf("WAIT\n");
      //wait();
      break;
    case SYS_CREATE:
      printf("CREATE\n");
      create(*(char **)f->esp+4, *(unsigned *)f->esp+8);
      break;
    case SYS_OPEN:
      printf("OPEN\n");
      open(*(char **)f->esp+4);
      break;
    case SYS_CLOSE:
      printf("CLOSE\n");
      close(*(int *)f->esp+4);
      break;
    case SYS_WRITE:
      printf("WRITE\n");
      write(*(int*)f->esp+4, *(char **)f->esp+8, *(int*)f->esp+12);
      break;
    default:
      printf ("system call!\n");
      thread_exit ();
      break;
    }
}

void halt(void){
  printf("power off\n");
  power_off();
}

bool create(const char *file, unsigned initial_size){
  return filesys_create(file, initial_size);
}

int open(const char *file){
  int fd = -1;
  for(int i = 0; i < 128; i++){
    if(thread_current()->fdtable[i] == NULL){
      fd = i;
      struct file *fil_ = filesys_open(file);
      thread_current()->fdtable[fd] = fil_;
      break;
    }
  }
  return fd;
}

void close(int fd){
  struct file *fil_ = thread_current()->fdtable[fd];
  file_close(fil_);
  thread_current()->fdtable[fd] = NULL;
}

int write(int fd, const void *buffer, unsigned size){
  struct file *file = thread_current()->fdtable[fd];

  if(fd == 1){
    putbuf(buffer, size);
    return size;
  }
  if(file == NULL){
    return -1;
  }
  return file_write(file, buffer, size);
}

int read(int fd, void *buffer, unsigned size){
  struct file *file = thread_current()->fdtable[fd];
  if(fd == 0){
    return size;
  }
  if(file == NULL){
    return -1;
  }
  return file_read(file, buffer, size);
}
