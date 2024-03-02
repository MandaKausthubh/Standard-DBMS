#include "pds.h"
#include "bst.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct Repo_Info pds_repo_info;

int PDS_Create(char *parent_repo_name, char* child_repo_name) {
    char parent_name[30], child_name[30];
    strcpy();
}

int PDS_Open(char *parent_repo_name, char* child_repo_name, int parent_rec_size, int child_rec_size);
int PDS_LoadNDX();
int PDS_put_parent_rec_by_key(int key, void* rec);
int PDS_put_child_rec_by_key(int key, void* rec);
int PDS_put_link_by_key(int p_key, int c_key);
int PDS_get_parent_by_key(int p_key, void* rec);
int PDS_get_child_by_key(int c_key, void* rec);
int PDS_get_parent_by_ndx_key(void* key, void* rec, int (*matcher)(void* rec, void* key), int io_count);
int PDS_get_links(int p_key, int arr[], int count);
int PDS_Close();
