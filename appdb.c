#include <stdlib.h>
#include <string.h>
#include "sqlite3/sqlite3.h"
#include "appdb.h"

#define APP_DB "ur0:shell/db/app.db"

static int get_applist_callback(void *data, int argc, char **argv, char **cols) {
    applist *list = (applist*)data;
    appinfo *info = malloc(sizeof(appinfo));
    memset(info, 0, sizeof(appinfo));
    if (list->count == 0) {
        list->items = info;
    } else {
        appinfo *tmp = list->items;
        while(tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = info;
        info->prev = tmp;
    }
    list->count += 1;
    strcpy(info->title_id, argv[0]);
    strcpy(info->title, argv[1]);
    strcpy(info->eboot, argv[2]);
    strcpy(info->dev, argv[3]);
    for (int i = 0; i < 256; i++) {
        if (info->title_id[i] == '\n') {
            info->title_id[i] = ' ';
        }
    }
    return 0;
}

int get_applist(applist *list) {
    char *query = "select a.titleid, b.title, c.ebootbin,"
                  "       rtrim(substr(c.ebootbin, 0, 5), ':') as dev"
                  "  from (select titleid"
                  "          from tbl_appinfo"
                  "         where key = 566916785"
                  "           and (val = 'gd' or val = 'gp')"
                  "           and titleid like 'P%'"
                  "         order by titleid) a,"
                  "       tbl_appinfo_icon b,"
                  "       (select titleid, val as ebootbin"
                  "          from tbl_appinfo"
                  "         where key = 3022202214) c"
                  " where a.titleid = b.titleid"
                  "   and a.titleid = c.titleid";

    sqlite3 *db;
    int ret = sqlite3_open(APP_DB, &db);
    if (ret) {
        return -1;
    }

    memset(list, 0, sizeof(applist));

    char *errMsg;
    ret = sqlite3_exec(db, query, get_applist_callback, (void *)list, &errMsg);
    if (ret != SQLITE_OK) {
        sqlite3_close(db);
        return -2;
    }
    sqlite3_close(db);
    return 0;
}