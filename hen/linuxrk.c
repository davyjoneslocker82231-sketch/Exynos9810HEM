#include "include/uapi/linux/elf.h"
#include "include/uapi/linux/elf-em.h"
#include "include/uapi/linux/elf-fdpic.h"
#include "include/uapi/linux/efs_fs_sb.h"
#include <linux/elf.h>

static int h3_check_header(struct elf64_hdr *hdr) {
    // 1. Check for Standard ELF Magic
    if (memcmp(hdr->e_ident, ELFMAG, SELFMAG) != 0) {
        return -ENOEXEC;
    }

    if (hdr->e_ident[EI_PAD] != 0x44) {
        printk(KERN_INFO "--- [h3nm] Standard ELF detected. Skipping h3-hook.\n");
        return -ENOEXEC;
    }

    printk(KERN_INFO "--- [h3nm] Authenticated Homebrew ELF detected\n");
    return 0;
}
