#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "devices/input.h"
#include "userprog/process.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void)
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED)
{
  if(!is_valid_ptr(f->esp)) exit(-1);
  switch(*(uint32_t *)(f->esp)){
    case SYS_HALT:
      //printf("HALT\n");
      halt();
      break;
    case SYS_WAIT:
      //printf("WAIT\n");
      (f->eax) = wait(*(pid_t *)(f->esp+4));
      break;
    case SYS_CREATE:
      //printf("CREATE\n");
      (f->eax) = create(*(char **)(f->esp+4), *(unsigned *)(f->esp+8));
      break;
    case SYS_OPEN:
      //printf("OPEN\n");
      (f->eax) =open(*(char **)(f->esp+4));
      break;
    case SYS_CLOSE:
      //printf("CLOSE\n");
      close(*(int *)(f->esp+4));
      break;
    case SYS_WRITE:
      //printf("WRITE\n");
      (f->eax) = write(*(int*)(f->esp+4), *(char **)(f->esp+8), *(unsigned*)(f->esp+12));
      break;
    case SYS_READ:
      //printf("READ\n");
      (f->eax) = read(*(int*)(f->esp+4), *(char **)(f->esp+8), *(unsigned*)(f->esp+12));
      break;
    case SYS_EXEC:
      (f->eax) = (pid_t) exec(*(const char**)(f->esp+4));
      break;
    case SYS_EXIT:
    default:
      //printf ("EXIT\n");
      exit(*(int *)(f->esp+4));
      break;
    }
}

pid_t exec (const char *cmd_line){
  if(!is_valid_string(cmd_line)) exit(-1);
  pid_t pid = process_execute(cmd_line);
  return pid;
}

void halt(void){
  // printf("power off\n");
  power_off();
}

bool create(const char *file, unsigned initial_size){
  if(!is_valid_string(file)) exit(-1);
  return filesys_create(file, initial_size);
}

int open(const char *file){
  if(!is_valid_string(file)) exit(-1);
  int fd = -1;
  for(int i = 2; i < 130; i++){
    if(thread_current()->fdtable[i] == NULL){
      fd = i;
      struct file *fil_ = filesys_open(file);
      thread_current()->fdtable[fd] = fil_;
      if(fil_ == NULL){
        return -1;
      }
      break;
    }
  }
  return fd;

}

void close(int fd){
  if(!is_valid_fd(fd)) exit(-1);
  struct file *file = thread_current()->fdtable[fd];
  file_close(file);
  thread_current()->fdtable[fd] = NULL;
}

int write(int fd, const void *buffer, unsigned size){
  if(!is_valid_buff(buffer, size)) exit(-1);
  struct file *file = thread_current()->fdtable[fd];

  if(fd == STDOUT_FILENO){
    putbuf(buffer, size);
    return size;
  }
  if(file == NULL || fd == STDIN_FILENO){
    return -1;
  }
  return file_write(file, buffer, size);
}

int read(int fd, void *buffer, unsigned size){
  if(!is_valid_buff(buffer, size)) exit(-1);

  struct file *file = thread_current()->fdtable[fd];
  if(fd == STDIN_FILENO){
    uint8_t *buff = (uint8_t*)buffer;
    for(unsigned i = 0; i < size; i++){
      buff[i] = input_getc();
    }
    return size;
  }
  if(file == NULL || fd == STDOUT_FILENO){
    return -1;
  }
  return file_read(file, buffer, size);
}

int wait(pid_t pid){
  return process_wait(pid);
}

void exit(int status){
  for(int i = 2; i < 130; i++){
    close(i);
  }
  thread_current()->parent->exit_status = status;
  printf("%s: exit(%d)\n", thread_current()->name, thread_current()->parent->exit_status);
  thread_exit();
}

/*
  Makes sure that the file descriptor table isn't full. STDIN and STDOUT are
  not checked.
*/
bool is_valid_fd(int fd){
  return ((fd < 130) && (fd > 1));
}

/*  Checks if ptr is a valid non-NULL pointer in the user address space and in
  the current threads page directory.
*/
bool is_valid_ptr(const void *ptr){
  return (ptr != NULL &&
    is_user_vaddr(ptr) &&
    pagedir_get_page(thread_current()->pagedir, ptr) != NULL);
}

/*
  Makes sure all possible points in buff for size are valid
*/
bool is_valid_buff(const void *buff, unsigned size){
  if (buff == NULL) return false;
  unsigned b = (unsigned) buff;
  unsigned i;
  for(i = b; i < b + size; i++){
    if(!is_valid_ptr((void *) i)) return false;
  }
  return true;
}

/*
  Checks that string is a valid non-NULL pointer and doesn't contain any bad pointers
  If string is not NULL, it will loop over all characters in the string to make
  sure they are all valid, and finally return true if it encounters \0 without
  any problems.
  Uses is_valid_ptr to check pointer validity.
*/
bool is_valid_string(const char *string){
  if(!is_valid_ptr(string)) return false;
  unsigned ch = (unsigned) string;
  while(true){
    if(!is_valid_ptr((void *) ++ch)) return false;
    if(*(char*) ch == '\0') return true;
  }
}
