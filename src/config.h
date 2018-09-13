#ifndef __CONFIG_H__
#define __CONFIG_H__

extern char app_titleid[16];
extern char app_title[256];

typedef struct configure {
    const char *base;
    const char *slot_format;
    char *list_mode;
    uint8_t use_dpad;
    const char *domain;
    const char *url_path;
    const char *user;
    const char *password;
    const char *sync_dir;
} configure;

extern configure config;

void load_config();
void save_config();

#endif
