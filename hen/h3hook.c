#include <include/linux/kernel.h>
#include <include/linux/init.h>
#include <arch/x86/include/asm/pgtable.h>
#include <arch/x86/include/asm/tlbflush.h>

void h3_unlock_mem(unsigned long addr) {
    pgd_t *pgd;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *pte;

    pgd = pgd_offset_k(addr);
    if (pgd_none(*pgd) || pgd_bad(*pgd)) return;

    pud = pud_offset(pgd, addr);
    pmd = pmd_offset(pud, addr);
    if (pmd_none(*pmd) || pmd_bad(*pmd)) return; // Added safety check
    
    pte = pte_offset_kernel(pmd, addr);
    set_pte(pte, pte_mkwrite(*pte));

    flush_tlb_all();
    printk(KERN_INFO "--- [h3nm] Memory Unlocked at: 0x%lx\n", addr);
}

// Wrapper for the early boot unlock
static int __init h3_boot_unlock(void) {
    printk(KERN_INFO "--- [h3nm] Initializing Memory Unlock Phase...\n");
    
    unsigned long selinux_addr = 0xffffffc001a2b3c4; 
    
    h3_unlock_mem(selinux_addr);

    // Now that it's unlocked, let's force it to 0 (Permissive)
    int *enforcing = (int *)selinux_addr;
    *enforcing = 0; 

    printk(KERN_INFO "--- [h3nm] SELinux forced to Permissive via h3hook.\n");
    return 0;
}


void h3_vdb_handler(struct pt_regs *regs) {
    unsigned int instr;
    instr = *(unsigned int *)regs->pc;

    if (instr == 0xD4200000) { 
        printk(KERN_INFO "--- [h3nm VDB] Breakpoint Hit at 0x%llx\n", regs->pc);
        regs->pc += 4; 
        printk(KERN_INFO "--- [h3nm VDB] Instruction Skipped. Resuming...\n");
    }
}

static int __init h3_vdb_init(void) {
    printk(KERN_INFO "--- [h3nm] init\n");
    printk(KERN_INFO "--- [h3nm] made by dave lone\n");
    return 0;
}

pure_initcall(h3_boot_unlock);
late_initcall(h3_vdb_init);

