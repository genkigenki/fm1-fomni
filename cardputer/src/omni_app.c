/* SPDX-License-Identifier: GPL-3.0-only
 * Copyright (C) 2026 M. Geisler (genkigenki)
 * FoMni-1 on the M5Stack Cardputer: the app (UI, project, the screen reader's protocol, the canvas) as one unit, the
 * same sources host/omni_host.c builds, unchanged; the engine (dsp/omni.c) is its own unit (platformio.ini).
 * Drawing goes to the mvave_cardputer board layer's screen; plat.h is implemented in plat_cardputer.c. */
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "cp_display.h"
#include "cp_console.h"

/* gfx.c puts its canvas in the FM-1's .pool section; the ESP32 has none: attributes are dropped from here on */
#define __attribute__(x)

static void lcd_sync(void) {}
static void lcd_fill(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint16_t c) { cp_lcd_fill(x, y, w, h, c); }
static void lcd_blit(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const uint16_t *px) { cp_lcd_blit(x, y, w, h, px); }
#include "gfx.c"

#include "app/plat.h"
#include "app/app.h"
#ifndef OM_VERSION
#define OM_VERSION "DEV"
#endif
#define ANN_NAME "FOMNI-1 " OM_VERSION
#include "ann_words.h"
static int ann_wire_send(const uint8_t *p, uint32_t n)   /* no USB MIDI on the Cardputer yet */
{
    (void)p;
    (void)n;
    return 0;
}
#include "announce.c"
#include "app/project.c"
#include "app/ui.c"

#undef __attribute__

void omni_boot(void)
{
    om_init();
    project_load();                             /* defaults are in place when nothing is saved */
    project_apply();
    lcd_fill(0, 0, 240, 240, 0);
    ui_init();
}

int omni_tick(uint32_t now_ms)                  /* the FM-1's main loop: a UI frame every 16 ms, input between */
{
    static uint32_t last_frame;
    if (now_ms - last_frame >= 16u) {
        last_frame = now_ms;
        ui_frame();
        return 1;
    }
    ui_input_only();
    return 0;
}

void omni_render(int32_t *o, uint32_t n) { om_render(o, n, plat_master()); }

static void cmd_omni(const char *args)
{
    (void)args;
    cp_console_printf("keys %07lx buttons %04lx master %lu cpu %lu%%\n", (unsigned long)plat_keys(),
                      (unsigned long)plat_buttons(), (unsigned long)plat_master(), (unsigned long)plat_cpu_pct());
}
void omni_console(void) { cp_console_register("omni", cmd_omni); }
