#ifndef PDS_H
#define PDS_H

// Defining Error codes:
#define PDS_SUCCESS 0
#define PDS_FAILURE 1
#define PDS_ADD_FAIL 12
#define PDS_REPO_OPEN 3
#define PDS_REPO_CLOSED 4
#define PDS_REPO_NOT_OPEN 5
#define PDS_REC_NOT_FOUND 3
#define PDS_REC_ALREADY_EXISTS 3
#define PDS_FILE_ERROR 3

#include <stdio.h>
#include "pds_bst.h"

struct PDS_NdxInfo{
    int key, offset, is_deleted;
};

struct Repo_Info {
    char pds_name[30];
    int pds_repo_status;
    FILE* ndx_file;
    FILE* parent_data_file;
    FILE* child_data_file;
    FILE* link_data_file;
    int rec_count_parents;
    int rec_count_children;
    int rec_count_links;

    struct BST_Node* pds_bst;
};

extern struct Repo_Info pds_repo_info;

struct Link{int parent, child;};

int PDS_Create(char *parent_repo_name, char* child_repo_name);
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

#endif
