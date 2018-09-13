#ifndef __WEBDAV_H__
#define __WEBDAV_H__

void webdavDownloadFile(char *url, char *file);
void webdavUploadFile(char *url, char *file);
void webdavCreateDir(char *url);
int webdavMove(char *url_old, char *url_new);
int webdavTest(char *url);
char *userpwd();
#endif
