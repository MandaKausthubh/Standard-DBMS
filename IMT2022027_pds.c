#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Local files:
#include "pds.h"
#include "bst.h"

struct Repo_Info pds_repo_info;

int PDS_Create(char *parent_repo_name, char* child_repo_name) {
    char parent_name[30] = "", child_name[30] = "", parent_ndx[30] = "", link_name[20] = "";
    strcpy(parent_name, parent_repo_name); strcat(parent_name, ".dat");
    strcpy(parent_ndx, parent_repo_name); strcat(parent_ndx, ".ndx");
    strcpy(link_name, parent_repo_name);strcat(link_name, child_repo_name); strcat(link_name, ".dat");
    strcpy(child_name, child_repo_name); strcat(child_name, ".dat");
    
    FILE* parent, *ndx, *child, *link;

    parent = fopen(parent_name, "wb+");
    child = fopen(child_name, "wb+");
    ndx = fopen(parent_ndx, "wb+");
    link = fopen(link_name, "wb+");

    if(parent && ndx && child && link) {
        int x = 0;
        fread(&x, sizeof(int), 1, ndx);
        fclose(parent);
        fclose(ndx);
        fclose(child);
        fclose(link);

        return PDS_SUCCESS;
    }

    return PDS_FAILURE;
}

int PDS_Open(char *parent_repo_name, char* child_repo_name, int parent_rec_size, int child_rec_size) {

    char parent_name[30] = "", child_name[30] = "", parent_ndx[30] = "", link_name[20] = "";
    strcpy(parent_name, parent_repo_name); strcat(parent_name, ".dat");
    strcpy(parent_ndx, parent_repo_name); strcat(parent_ndx, ".ndx");
    strcpy(link_name, parent_repo_name);strcat(link_name, child_repo_name); strcat(link_name, ".dat");
    strcpy(child_name, child_repo_name); strcat(child_name, ".dat");
    
    FILE* parent, *ndx, *child, *link;

    parent = fopen(parent_name, "rb+");
    child = fopen(child_name, "rb+");
    ndx = fopen(parent_ndx, "rb+");
    link = fopen(link_name, "rb+");

    if(parent && ndx && child && link) {
        pds_repo_info.ndx_file = ndx;
        pds_repo_info.link_data_file = link;
        pds_repo_info.child_data_file = child;
        pds_repo_info.parent_data_file = parent;

        strcpy(pds_repo_info.pds_name, parent_repo_name);

        pds_repo_info.pds_repo_status = PDS_REPO_OPEN;

        pds_repo_info.pds_child_size = child_rec_size;
        pds_repo_info.pds_parent_size = parent_rec_size;

        pds_repo_info.pds_bst = NULL;

        return PDS_SUCCESS;
    }

    return PDS_FILE_ERROR;

}

int PDS_LoadNDX() {
    if(pds_repo_info.pds_repo_status == PDS_REPO_OPEN) {
        
        int number_of_rec;
        FILE* fptr = pds_repo_info.ndx_file;
        fseek(fptr, 0, SEEK_SET);
        fread(&number_of_rec, sizeof(int), 1, fptr);

        for(int i = 0; i < number_of_rec; i++) {
            struct PDS_NdxInfo Info;
            fread(&Info, sizeof(struct PDS_NdxInfo), 1, fptr);
            bst_add_node(&(pds_repo_info.pds_bst), Info.key, &Info);
        }
        pds_repo_info.rec_count_parents = number_of_rec;
        return PDS_SUCCESS;

    } else return PDS_REPO_NOT_OPEN;
}


int PDS_put_parent_rec_by_key(int key, void* rec) {
    if(pds_repo_info.pds_repo_status == PDS_REPO_OPEN) {
        struct BST_Node* return_val = bst_search(pds_repo_info.pds_bst, key);
        if(return_val) {
            if(((struct PDS_NdxInfo*)(return_val->data))-> is_deleted == 0) {
                return PDS_FAILURE;
            } else {
                ((struct PDS_NdxInfo*)(return_val->data))-> is_deleted = 0;
                int offset = ((struct PDS_NdxInfo*)(return_val->data))->offset;
                FILE* fptr = pds_repo_info.parent_data_file;
                fseek(fptr, offset+sizeof(int), SEEK_SET);
                
                int valid = 1;
                fwrite(&valid, sizeof(int), 1, fptr);

                return PDS_SUCCESS;
            }
        } else {
            FILE* fptr = pds_repo_info.parent_data_file;
            fseek(fptr, 0, SEEK_END);
            int valid = 1;
            int offset = ftell(fptr);
            fwrite(&key, sizeof(int), 1, fptr);
            fwrite(&valid, sizeof(int), 1, fptr);
            fwrite(rec, pds_repo_info.pds_parent_size, 1, fptr);

            struct PDS_NdxInfo repo;
            repo.key = key;
            repo.offset = offset;
            repo.is_deleted = 0;
            
            bst_add_node(&(pds_repo_info.pds_bst), key, rec);
            return PDS_SUCCESS;
        }
    } else return PDS_REPO_NOT_OPEN;
}


int PDS_put_child_rec_by_key(int key, void* rec) {
    if(pds_repo_info.pds_repo_status == PDS_REPO_OPEN) {
        FILE* fptr = pds_repo_info.child_data_file;
        fseek(fptr, 0, SEEK_END);
        fwrite(&key, sizeof(int), 1, fptr);
        fwrite(rec, pds_repo_info.pds_child_size, 1, fptr);
        return PDS_SUCCESS;
    }else return PDS_REPO_NOT_OPEN;
}

int PDS_put_link_by_key(int p_key, int c_key) {
    if(pds_repo_info.pds_repo_status == PDS_REPO_OPEN) {
        FILE* fptr = pds_repo_info.link_data_file;
        fseek(fptr, 0, SEEK_END);
        struct Link newLink;
        newLink.parent = p_key;
        newLink.child = c_key;

        fwrite(&newLink, sizeof(struct Link), 1, fptr);
        return PDS_SUCCESS;
    } else  return PDS_REPO_NOT_OPEN;
}

int PDS_get_parent_by_key(int p_key, void* rec) {
    if(pds_repo_info.pds_repo_status == PDS_REPO_OPEN) {
        struct PDS_NdxInfo* info = (struct PDS_NdxInfo*)(bst_search(pds_repo_info.pds_bst, p_key)->data);
        if(info) {
            if(info->is_deleted == 1) return PDS_REC_NOT_FOUND;
            else {
                int offset = info->offset;
                FILE* fptr = pds_repo_info.parent_data_file;
                fseek(fptr, offset+2*sizeof(int), SEEK_SET);
                fread(rec, pds_repo_info.pds_parent_size, 1, fptr);
                return PDS_SUCCESS;
            }
        } return PDS_REC_NOT_FOUND;
    } else return PDS_REPO_NOT_OPEN;
}

int PDS_get_child_by_key(int c_key, void* rec) {
    if(pds_repo_info.pds_repo_status == PDS_REPO_OPEN) {
        FILE* fptr = pds_repo_info.child_data_file;
        fseek(fptr, 0, SEEK_SET);
        while(!feof(fptr)) {
            int key;
            fread(&key, sizeof(int), 1, fptr);
            fread(rec, pds_repo_info.pds_child_size, 1, fptr);
            if(key == c_key) {
                return PDS_SUCCESS;
            }
        }
        return PDS_REC_NOT_FOUND;
    } return PDS_REC_NOT_FOUND;
}

int PDS_get_parent_by_non_ndx_key(void* p_key, void* rec, int (*matcher)(void* rec, void* key), int* io_count) {
    if(pds_repo_info.pds_repo_status == PDS_REPO_OPEN) {
        FILE* fptr = pds_repo_info.parent_data_file;
        fseek(fptr, 0, SEEK_SET);
        *(io_count) = 0;
        while(!feof(fptr)) {
            int key, valid;
            fread(&key, sizeof(int), 1, fptr);
            fread(&valid, sizeof(int), 1, fptr);
            fread(rec, pds_repo_info.pds_child_size, 1, fptr);
            if(valid == 1) {
                if(matcher(rec, p_key)) {
                    return PDS_SUCCESS;
                }
                (*io_count)++;
            }
        }
        return PDS_REC_NOT_FOUND;
    } else return PDS_REPO_NOT_OPEN;
}

int PDS_get_links(int p_key, int arr[], int* count) {
    if(pds_repo_info.pds_repo_status == PDS_REPO_OPEN) {
        FILE* fptr = pds_repo_info.link_data_file;
        (*count) = 0;
        fseek(fptr, 0, SEEK_SET);
        while(!feof(fptr)) {
            struct Link temp;
            fread(&temp, sizeof(struct Link), 1, fptr);
            // printf("Reading: %d %d\n", temp.parent, temp.child);
            if(temp.parent == p_key) {arr[(*count)++] = temp.child;}
        }
        return PDS_SUCCESS;
    } else return PDS_REPO_NOT_OPEN;
}

int PDS_delete_parent_with_key(int p_key) {
    if(pds_repo_info.pds_repo_status == PDS_REPO_OPEN) {
        struct BST_Node* temp = bst_search(pds_repo_info.pds_bst, p_key);
        
        if(temp) {
            struct PDS_NdxInfo* result = (struct PDS_NdxInfo*) temp->data;
            if(result->is_deleted == 1) {
                return PDS_REC_NOT_FOUND;
            }else {
                int offset = result->offset; int valid = 0;
                FILE* fptr = pds_repo_info.parent_data_file;
                fseek(fptr, offset + sizeof(int), SEEK_SET);
                fwrite(&valid, sizeof(int), 1, fptr);
                result->is_deleted = 1;

                return PDS_SUCCESS;
            }
        } else return PDS_REC_NOT_FOUND;

    } else return PDS_REPO_NOT_OPEN;
}

void PrintIntoFile(struct BST_Node* root, FILE* fptr) {
    if(root) {
        PrintIntoFile(root->left_child, fptr);
        PrintIntoFile(root->right_child, fptr);
        fwrite(root->data, sizeof(struct PDS_NdxInfo), 1, fptr);
    }
}

int PDS_Close() {
    if(pds_repo_info.pds_repo_status == PDS_REPO_OPEN) {
        char FileName[30]; strcpy(FileName, pds_repo_info.pds_name);
        strcat(FileName, ".ndx");

        FILE* fptr = fopen(FileName, "wb+");

        fwrite(&(pds_repo_info.rec_count_parents), sizeof(int), 1, fptr);

        PrintIntoFile(pds_repo_info.pds_bst, fptr);

        bst_destroy(pds_repo_info.pds_bst);

        fclose(pds_repo_info.ndx_file);
        fclose(pds_repo_info.parent_data_file);
        fclose(pds_repo_info.ndx_file);
        fclose(pds_repo_info.child_data_file);
        fclose(fptr);

        pds_repo_info.pds_repo_status = PDS_REPO_CLOSED;
        return PDS_SUCCESS;
    } else return PDS_REPO_NOT_OPEN;
}
