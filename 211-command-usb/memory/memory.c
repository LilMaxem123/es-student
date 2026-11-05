#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/regs/addressmap.h"

extern char __flash_binary_start;
extern char __flash_binary_end;
extern char __boot2_start__;
extern char __boot2_end__;
extern char __etext;
extern char __data_start__;
extern char __data_end__;
extern char __bss_start__;
extern char __bss_end__;
extern char __HeapLimit;
extern char __StackBottom;
extern char __StackTop;

// memory.c

static void row(const char *name, uintptr_t start, uintptr_t end)
{
    printf("%-10s 0x%08x 0x%08x %8u\n",
           name, (unsigned)start, (unsigned)end, (unsigned)(end - start));
}

void mem_info(void)
{
    // шапка таблицы: область, начало, конец, размер
    printf("%-10s %-10s %-10s %8s\n", "area", "start", "end", "size");

    // flash — XIP_BASE и PICO_FLASH_SIZE_BYTES
    row("flash", XIP_BASE, XIP_BASE + PICO_FLASH_SIZE_BYTES);
    // sram — базовый адрес из SDK, размер из datasheet
    row("sram", 0x20000000, 0x20000000 + 264 * 1024);
    // rom — базовый адрес из SDK, размер из datasheet
    row("rom", ROM_BASE, ROM_BASE + 16 * 1024);
    // image — от __flash_binary_start до __flash_binary_end
    row("image", (uintptr_t)&__flash_binary_start, (uintptr_t)&__flash_binary_end);
    // free  — от __flash_binary_end до конца флеш-памяти
    row("free", (uintptr_t)&__flash_binary_end, XIP_BASE + PICO_FLASH_SIZE_BYTES);
    // boot2 — от __boot2_start__ до __boot2_end__
    row("boot2", (uintptr_t)&__boot2_start__, (uintptr_t)&__boot2_end__);
    // text  — от __boot2_end__ до __etext: код и константы
    row("text", (uintptr_t)&__boot2_end__, (uintptr_t)&__etext);

    // data flash — хранение .data, от __etext, длиной с .data
    row("data flash",
    (uintptr_t)&__etext,
    (uintptr_t)&__etext +
    ((uintptr_t)&__data_end__ - (uintptr_t)&__data_start__));
    // data ram   — работа .data, от __data_start__ до __data_end__
    row("data ram", (uintptr_t)&__data_start__, (uintptr_t)&__data_end__);
    // bss        — от __bss_start__ до __bss_end__
    row("bss", (uintptr_t)&__bss_start__, (uintptr_t)&__bss_end__);
    // heap       — от __bss_end__ до __HeapLimit
    row("heap", (uintptr_t)&__bss_end__, (uintptr_t)&__HeapLimit);
    // stack      — от __StackBottom до __StackTop
    row("stack", (uintptr_t)&__StackBottom, (uintptr_t)&__StackTop);

    printf("\ntotal\n");

printf("  flash image %8u = boot2 %u + text %u + data %u\n",
       (unsigned)((uintptr_t)&__flash_binary_end - (uintptr_t)&__flash_binary_start),
       (unsigned)((uintptr_t)&__boot2_end__ - (uintptr_t)&__boot2_start__),
       (unsigned)((uintptr_t)&__etext - (uintptr_t)&__boot2_end__),
       (unsigned)((uintptr_t)&__data_end__ - (uintptr_t)&__data_start__));

printf("  flash free  %8u of %u\n",
       (unsigned)(XIP_BASE + PICO_FLASH_SIZE_BYTES - (uintptr_t)&__flash_binary_end),  
       (unsigned)PICO_FLASH_SIZE_BYTES);

printf("  ram used    %8u = data %u + bss %u\n",
       (unsigned)((uintptr_t)&__data_end__ - (uintptr_t)&__data_start__
                + (uintptr_t)&__bss_end__ - (uintptr_t)&__bss_start__),
       (unsigned)((uintptr_t)&__data_end__ - (uintptr_t)&__data_start__),
       (unsigned)((uintptr_t)&__bss_end__ - (uintptr_t)&__bss_start__));

printf("  ram free    %8u for heap and %u for stack\n",
       (unsigned)((uintptr_t)&__HeapLimit - (uintptr_t)&__bss_end__),
       (unsigned)((uintptr_t)&__StackTop - (uintptr_t)&__StackBottom));

}