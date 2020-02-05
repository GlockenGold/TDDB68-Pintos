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
  //thread_current()->fdtable[STDIN_FILENO] = ;
  //thread_current()->fdtable[STDOUT_FILENO] = ;
}

static void
syscall_handler (struct intr_frame *f UNUSED)
{
  switch(*(uint32_t *)(f->esp)){
    case SYS_HALT:
      printf("HALT\n");
      halt();
      break;
    case SYS_WAIT:
      printf("WAIT\n");
      //wait();
      break;
    case SYS_CREATE:
      printf("CREATE\n");
      (f->eax) = create(*(char **)(f->esp+4), *(unsigned *)(f->esp+8));
      break;
    case SYS_OPEN:
      printf("OPEN\n");
      (f->eax) =open(*(char **)(f->esp+4));
      break;
    case SYS_CLOSE:
      printf("CLOSE\n");
      close(*(int *)(f->esp+4));
      break;
    case SYS_WRITE:
      printf("WRITE\n");
      (f->eax) = write(*(int*)(f->esp+4), *(char **)(f->esp+8), *(unsigned*)(f->esp+12));
      break;
    case SYS_READ:
      printf("READ\n");
      (f->eax) = read(*(int*)(f->esp+4), *(char **)(f->esp+8), *(unsigned*)(f->esp+12));
      break;
    case SYS_EXIT:
    default:
      printf ("EXIT\n");
      exit(0);
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
  for(int i = 2; i < 130; i++){
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
  struct file *file = thread_current()->fdtable[fd];
  file_close(file);
  thread_current()->fdtable[fd] = NULL;
}

int write(int fd, const void *buffer, unsigned size){
  struct file *file = thread_current()->fdtable[fd];

  if(fd == STDOUT_FILENO){
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
  if(fd == STDIN_FILENO){
    return size;
  }
  if(file == NULL){
    return -1;
  }
  return file_read(file, buffer, size);
}

void exit(int status){
  thread_exit();
}
