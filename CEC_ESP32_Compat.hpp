#pragma once

#include <driver/gpio.h>
#include <esp_timer.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

constexpr int HIGH = 1;
constexpr int LOW = 0;

constexpr int INPUT = 0;
constexpr int OUTPUT = 1;
constexpr int INPUT_PULLUP = 2;

inline void pinMode(int pin, int mode)
{
    gpio_mode_t esp_mode;
    bool enable_pullup;

    switch (mode)
    {
    case OUTPUT:
        esp_mode = GPIO_MODE_OUTPUT;
        enable_pullup = false;
        break;
    case INPUT_PULLUP:
        esp_mode = GPIO_MODE_INPUT;
        enable_pullup = true;
        break;
    case INPUT:
    default:
        esp_mode = GPIO_MODE_INPUT;
        enable_pullup = false;
        break;
    }

    const gpio_num_t esp_pin { static_cast<gpio_num_t>(pin) };

    gpio_set_direction(esp_pin, esp_mode);

    if (enable_pullup)
    {
        gpio_pullup_en(esp_pin);
    }
    else
    {
        gpio_pullup_dis(esp_pin);
    }
}

inline void digitalWrite(int pin, int state)
{
    const gpio_num_t esp_pin { static_cast<gpio_num_t>(pin) };
    gpio_set_level(esp_pin, state ? 1 : 0);
}

inline int digitalRead(int pin)
{
    const gpio_num_t esp_pin { static_cast<gpio_num_t>(pin) };
    return gpio_get_level(esp_pin);
}

inline void delay(int ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

inline unsigned long millis()
{
    return (unsigned long) (esp_timer_get_time() / 1000);
}