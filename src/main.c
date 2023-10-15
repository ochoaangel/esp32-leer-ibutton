#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "onewire.h"

#define ONEWIRE_PIN GPIO_NUM_4

void onewire_task(void *pvParameters)
{
    onewire_search_t search;
    onewire_addr_t addr;

    // Inicializar el bus 1-Wire
    gpio_set_direction(ONEWIRE_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(ONEWIRE_PIN, 1);

    while (1)
    {

        printf("Dispositivos encontrados:\n");

        // Buscar y leer los dispositivos en el bus 1-Wire
        onewire_search_start(&search);
        while ((addr = onewire_search_next(&search, ONEWIRE_PIN)) != ONEWIRE_NONE)
        {
            printf("Dispositivo encontrado: \n");
            for (int i = 0; i < 8; i++)
                {
                    printf("%02llx", (unsigned long long)((addr >> (i * 8)) & 0xFF));
                }
            printf("\n");

            // Print additional information
            uint8_t family_code = (addr >> 56) & 0xFF;
            uint8_t serial_number[6];
            memcpy(serial_number, &addr, 6);

            printf("Tipo: %02x\n", family_code);
            printf("ID: %02x%02x%02x%02x%02x%02x\n", serial_number[5], serial_number[4], serial_number[3], serial_number[2], serial_number[1], serial_number[0]);
            printf("Serial: %02x%02x%02x%02x%02x%02x\n", serial_number[0], serial_number[1], serial_number[2], serial_number[3], serial_number[4], serial_number[5]);
        }

        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

void app_main()
{
    xTaskCreate(onewire_task, "onewire_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
}



//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

// #include <stdio.h>
// #include <freertos/FreeRTOS.h>
// #include <freertos/task.h>
// #include <driver/gpio.h>
// #include "onewire.h"

// #define ONEWIRE_PIN GPIO_NUM_4

// void onewire_task(void *pvParameters)
// {
//     onewire_search_t search;
//     onewire_addr_t addr;

//     // Inicializar el bus 1-Wire
//     gpio_set_direction(ONEWIRE_PIN, GPIO_MODE_OUTPUT);
//     gpio_set_level(ONEWIRE_PIN, 1);

//     while (1)
//     {
//         // Realizar un reset en el bus 1-Wire
//         if (!onewire_reset(ONEWIRE_PIN))
//         {
//             // printf("No se detectaron dispositivos 1-Wire\n");
//             // continue;
//         }

//         printf("Dispositivos encontrados:\n");

//         // Buscar y leer los dispositivos en el bus 1-Wire
//         onewire_search_start(&search);
//         while ((addr = onewire_search_next(&search, ONEWIRE_PIN)) != ONEWIRE_NONE)
//         {
//             printf("Dispositivo encontrado: %016llx\n", (unsigned long long)addr);
//         }

//         vTaskDelay(pdMS_TO_TICKS(10000));
//     }
// }

// void app_main()
// {
//     xTaskCreate(onewire_task, "onewire_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
// }
