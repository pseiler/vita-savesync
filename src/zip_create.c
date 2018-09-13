#include <stdio.h>
#include <zip.h>
#include <string.h>
#include <stdlib.h>

char *testfile = "ziptest.txt";

int main(int argc, char **argv) {
    const char *prg;
    prg = argv[0];
    zip_source_t *s;
    int err = 0;
    prg = argv[0];
    char *archive = argv[1];
//    char buf[100];

//    FILE * zipfile;
//    zipfile = fopen(archive, "w");
//    fclose(zipfile);
    printf("Programm: \"%s\". Zipfile: \"%s\"\n", prg, archive);
    zip_t *za = zip_open(archive, ZIP_CREATE, &err);
    printf("Pointer: %p\n", za);
    if (za == NULL) {
        printf("zip Objekt fehlgeschlagen\n");
        return 1;
    }
    else {
        printf("zip Objekt erzeugt\n");
    }
    zip_source_t *source = zip_source_file(za, testfile, 0, 0);
    char * dirname = "testdir";
//    char *file_dir = malloc(strlen(dirname) + 1 + strlen(testfile) + 1);
    char *file_dir = malloc(strlen(dirname) + 1 + strlen(testfile) + 1);
    strcpy(file_dir, dirname);
    strcat(file_dir, "/");
    strcat(file_dir, testfile);
    printf("%s\n", file_dir);
    zip_dir_add(za, dirname, ZIP_FL_OVERWRITE);
    zip_file_add(za, file_dir, source, ZIP_FL_ENC_UTF_8); 
//    fopen();
    if (zip_close(za) != 0) {
        printf("Fehler beim schließen des ZIP Archivs\n");
        printf("%s\n", zip_error_strerror(zip_get_error(za)));
    }
    else {
        printf("%s\n", zip_error_strerror(zip_get_error(za)));
    }
}
