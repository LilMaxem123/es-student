#include "device.h"
#include "pico/version.h"
#include <stdio.h>
#include "pico/unique_id.h"
#include "hardware/regs/addressmap.h"
#include "hardware/regs/sysinfo.h"

void device_info(void)
{
    // читаем серийный номер платы функцией SDK
    pico_unique_board_id_t board_id;
    // читаем регистр CHIP_ID по адресу и разбираем три поля
    volatile uint32_t *chip_id =
    (volatile uint32_t *)(SYSINFO_BASE + SYSINFO_CHIP_ID_OFFSET);
    uint32_t id = *chip_id;
    
    uint32_t manufacturer =
        (id & SYSINFO_CHIP_ID_MANUFACTURER_BITS)
        >> SYSINFO_CHIP_ID_MANUFACTURER_LSB;

    uint32_t part =
        (id & SYSINFO_CHIP_ID_PART_BITS)
        >> SYSINFO_CHIP_ID_PART_LSB;

    uint32_t revision =
        (id & SYSINFO_CHIP_ID_REVISION_BITS)
        >> SYSINFO_CHIP_ID_REVISION_LSB;
    char serial[2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1];
    pico_get_unique_board_id_string(serial, sizeof(serial));


    printf("project: %s\n", DEVICE_PROJECT);
    printf("repo: %s\n", DEVICE_REPO);
    printf("board: %s\n", DEVICE_BOARD);
    printf("serial: %s\n", serial);
    printf("chip: manufacturer 0x%03x, part 0x%04x, revision %u\n", manufacturer, part, revision);
    printf("pico-sdk: %s\n", PICO_SDK_VERSION_STRING);
    
}