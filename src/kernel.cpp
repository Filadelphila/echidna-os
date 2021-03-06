#include "video.hpp"
#include "gdt.hpp"
#include "idt.hpp"
#include "io.hpp"
#include "std.hpp"
#include "banner.hpp"

#define KERNEL_INFOS 0b00001110
#define KERNEL_DEFAULT 0b00001111
#define KERNEL_ERRORS 0b00001100

#define GIT_ADDRESS "https://github.com/Filadelphila/echidna-os"

void init_pic();
void kernel_main();

extern "C" void kernel_start() {
    Screen::set_blink(false);
    Screen::Terminal terminal;
    terminal.clear();

    init_idt();
    terminal.print_string("kernel: idt loaded\n", KERNEL_INFOS);

    init_pic();
    terminal.print_string("kernel: pic configured\n", KERNEL_INFOS);

    init_gdt();
    terminal.print_string("kernel: gdt loaded\n", KERNEL_INFOS);

    asm("movw $0x38, %ax\n\
            ltr %ax");
    terminal.print_string("kernel: tr loaded\n", KERNEL_INFOS);

    asm("movw $0x18, %ax\n\
            movw %ax, %ss\n\
            movl $0x20000, %esp");

    kernel_main();
}

void task_1(void) {
    while(1);
    return;
}

void kernel_main() {
    sti;
    Screen::Terminal terminal;

    terminal.print_string(BANNER, 0b00101111);
    terminal.print_string("Find EchidnaOS project on " GIT_ADDRESS "\n", KERNEL_DEFAULT);

    memcopy((char *) 0x30000, (char *) &task_1, 1000);
    terminal.print_string("kernel: switching to user task (ring3 mode)\n", KERNEL_INFOS);
    asm("cli\n\
            push $0x33\n\
            push $0x30000\n\
            pushfl\n\
            popl %%eax\n\
            orl $0x200, %%eax\n\
            and $0xffffbfff, %%eax\n\
            push %%eax\n\
            push $0x23\n\
            push $0x0\n\
            movl $0x20000, %0\n\
            movw $0x2b, %%ax\n\
            movw %%ax, %%ds\n\
            iret": "=m"(default_tss.esp0) :);

    terminal.print_string("kernel: error, halting system\n", KERNEL_ERRORS);
    asm("hlt");
}
