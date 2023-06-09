#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/loader.h"
#include "userprog/gdt.h"
#include "threads/flags.h"
#include "intrinsic.h"
/* added for project2 */
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "user/syscall.h"
void syscall_entry (void);
void syscall_handler (struct intr_frame *);
int _read (int fd, void *buffer, unsigned length);
/* System call.
 *
 * Previously system call services was handled by the interrupt handler
 * (e.g. int 0x80 in linux). However, in x86-64, the manufacturer supplies
 * efficient path for requesting the system call, the `syscall` instruction.
 *
 * The syscall instruction works by reading the values from the the Model
 * Specific Register (MSR). For the details, see the manual. */

#define MSR_STAR 0xc0000081         /* Segment selector msr */
#define MSR_LSTAR 0xc0000082        /* Long mode SYSCALL target */
#define MSR_SYSCALL_MASK 0xc0000084 /* Mask for the eflags */

void
syscall_init (void) {
	write_msr(MSR_STAR, ((uint64_t)SEL_UCSEG - 0x10) << 48  |
			((uint64_t)SEL_KCSEG) << 32);
	write_msr(MSR_LSTAR, (uint64_t) syscall_entry);

	/* The interrupt service rountine should not serve any interrupts
	 * until the syscall_entry swaps the userland stack to the kernel
	 * mode stack. Therefore, we masked the FLAG_FL. */
	write_msr(MSR_SYSCALL_MASK,
			FLAG_IF | FLAG_TF | FLAG_DF | FLAG_IOPL | FLAG_AC | FLAG_NT);


    /*added*/
    lock_init(&filesys_lock);
}

/* The main system call interface   rdi, rsi, rdx*/
void
syscall_handler (struct intr_frame *f UNUSED) {
	// TODO: Your implementation goes here.
	
	switch (f->R.rax) {
        case SYS_HALT:
            halt();
            break;
        case SYS_EXIT:
            exit(f->R.rdi);
            f->R.rax = f->R.rdi;
            break;
        case SYS_FORK:
            f->R.rax = fork(f->R.rdi);
            break;
        case SYS_EXEC:
            // f->R.rax = exec();
            break;
        case SYS_WAIT:
            f->R.rax = process_wait(f->R.rdi);
            break;
        case SYS_CREATE:
            f->R.rax = create(f->R.rdi,f->R.rsi);
            break;
        case SYS_REMOVE:
            f->R.rax = remove(f->R.rdi);
            break;
        case SYS_OPEN:
            f->R.rax = open(f->R.rdi);
            break;
        case SYS_FILESIZE:
            f->R.rax = filesize(f->R.rdi);
            break;
        case SYS_READ:
            f->R.rax = _read(f->R.rdi, f->R.rsi, f->R.rdx);
            break;
        case SYS_WRITE:
            f->R.rax = write(f->R.rdi, f->R.rsi, f->R.rdx);
            break;
        case SYS_SEEK:
            seek(f->R.rdi, f->R.rsi);
            break;
        case SYS_TELL:
            f->R.rax = tell(f->R.rdi);
            break;
        case SYS_CLOSE:
            close(f->R.rdi);
            break;
        default:
            // printf("System Call \n");
            thread_exit();
            break;
    }

}

/*Terminates Pintos by calling power_off() (declared in src/include/threads/init.h). 
This should be seldom used, because you lose some information about possible deadlock situations, etc.*/
void halt (void){
	power_off();
}

/* 커널에 상태를 리턴하면서 종료, 0=성공, -1=실패
    만약 부모프로세스가 유저프로그램의 종료를 기다리는 중이라면, 종료될 시 리턴될 그 상태를 기다린다.
*/
void exit(int status) {
    struct thread *cur = thread_current();
    cur->exit_status = status;
    printf("%s: exit(%d)\n", thread_name(), status); 	
    thread_exit();
}


pid_t fork (const char *thread_name){

}

/*
현재의 프로세스가 cmd_line에서 이름이 주어지는 실행가능한 프로세스로 변경됩니다. 
이때 주어진 인자들을 전달합니다. 성공적으로 진행된다면 어떤 것도 반환하지 않습니다.
만약 프로그램이 이 프로세스를 로드하지 못하거나 다른 이유로 돌리지 못하게 되면 exit state -1을 반환하며 프로세스가 종료됩니다.
이 함수는 exec 함수를 호출한 쓰레드의 이름은 바꾸지 않습니다. file descriptor는 exec 함수 호출 시에 열린 상태로 있다는 것을 알아두세요.
Run program which execute cmd_line.
Create thread and run. exec() in pintos is equivalent to fork()+exec() in Unix.
Pass the arguments to program to be executed.
Return pid of the new child process.
If it fails to load the program or to create a process, return -1.
Parent process calling exec should wait until child process is created and loads the executable completely.
*/
int exec (const char *file){
    return process_exec(file);
}

/*
Wait for a child process pid to exit and retrieve the child’s exit status.
If pid is alive, wait till it terminates. 
Returns the status that pid passed to exit.
If pid did not call exit, but was terminated by the kernel, return -1.

A parent process can call wait for the child process that has terminated. 
 return exit status of the terminated child process.

After the child terminates, the parent should deallocate its process descriptor
wait fails and return -1 if
- pid does not refer to a direct child of the calling process.
- The process that calls wait has already called wait on pid.
*/
int wait (pid_t pid){
        
}
/* create file, success = true, fail = false*/
bool create (const char *file, unsigned initial_size){
    check_address(file);
    return filesys_create(file,initial_size);
}

bool remove (const char *file){
    check_address(file);
    return filesys_remove(file);
}

/* 성공시 fd를 생성 */
int open (const char *file){
    check_address(file);
    struct file *open_file = filesys_open(file);

    if(open_file == NULL){
        return -1;
    }
    int fd = process_add_file(open_file);

    if( fd == -1){
        file_close(open_file);
    }
    return fd;
}


/* 파일의 크기를 알려주는 시스템 콜   성공시 파일의 크기 반환 실패시 -1 반환*/
int filesize (int fd){
    struct file* f = thread_current()->fdt[fd];
    if(f == NULL){
        return -1;
    }
    return file_length(f);
    
}

/* 열린 파일의 데이터를 읽는 시스템 콜 , 성공시 읽은 바이트 수 반환, 실패시 -1 반환 */
int _read (int fd, void *buffer, unsigned length){
    check_address(buffer);
    if(fd==0){
        char key;
        char** pBuf= &buffer;
        int len=1;
        for(int i = 0;i<length;i++){
            key = input_getc();
            if(key == -1){
                *pBuf = '\n';
                break;
            }
            *pBuf= key;
            pBuf++;
            len++;
        }
        return len;
    }
    else if(fd == 1){
        return -1;
    }

    lock_acquire(&filesys_lock);
    struct file*f = thread_current()->fdt[fd];
    if( f== NULL){
        return -1;
    }
    off_t return_length;
    return_length = file_read(f,buffer,length);
    lock_release(&filesys_lock);
    // printf("return_length = %d \n\n\n\n\n\n\n", return_length);
    return return_length;
}


/* buffer : 기록할 데이터를 저장한 버퍼의 주소 값, length: 데이터 크기 */
/* 열린 파일의 데이터를 기록 시스템 콜 기록한 바이트 수 반환, 실패시 -1 반환*/
int write (int fd, const void *buffer, unsigned length){
    check_address(buffer);
    int byte_written;
    if(fd == 1){
        putbuf(buffer, length);
        byte_written = length;
        return byte_written;
    }   
    struct file* f = thread_current()->fdt[fd];
    if(f == NULL){
        return -1;
    }
    else{
        lock_acquire(&filesys_lock);
        byte_written = file_write(f,buffer,length);
        lock_release(&filesys_lock);
    }
    return byte_written;
}

/* Changes the next byte to be read or written in open file fd to position 
    Use void file_seek(struct file *file, off_t new_pos).*/
void seek (int fd, unsigned position){
    struct file *f = process_get_file(fd);
    file_seek(f,position);
}

/* Return the position of the next byte to be read or written in open file fd.
Use off_t file_tell(struct file *file).*/
unsigned tell (int fd){
    return file_tell(process_get_file(fd));
}

/* Close the file descriptor fd */
void close (int fd){    
    file_close(process_get_file(fd));
    thread_current()->fdt[fd] = NULL;
}

/* 주소 값이 유저 영역에서 사용하는 주소 값인지 확인 하는 함수
유저 영역을 벗어난 영역일 경우 프로세스 종료(exit(-1)) */
void check_address(void *addr){
	if (addr == NULL || is_kernel_vaddr(addr)) {
		exit(-1);
	}
}


/* 시스템 콜 인자를 커널에 복사 */
void get_argument(void *rsp, int * arg, int count){
    
}