#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  pagetable_t u_page_table = myproc()->pagetable;
  uint64 first_addr, mask_addr;
  int check_size;
  uint mask = 0;
  argaddr(0, &first_addr);
  argint(1, &check_size);
  argaddr(2, &mask_addr);

  // If the size is too big to store in mask addr.
  if (check_size > 32) {
      return -1;
  }

  pte_t *first_pte = walk(u_page_table, first_addr, 0);
  for (int i = 0; i < check_size; i++) {
      if ((first_pte[i] & PTE_V) && (first_pte[i] & PTE_A)) {
          mask |= (1 << i);
          first_pte[i] ^= PTE_A; // reset
      }
  }
  copyout(u_page_table, mask_addr, (char*)&mask, sizeof(uint));
  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
