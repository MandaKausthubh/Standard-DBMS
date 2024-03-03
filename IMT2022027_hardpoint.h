#ifndef HARDPOINT_H
#define HARDPOINT_H

// Defining Macros:
#define HARDPOINT_SUCCESS 0
#define HARDPOINT_FAIL 1


int store_hardpoint( char *hardpoint_data_file );
int add_hardpoint(int child, int parent);
int search_hardpoint( int parent_id, int linked_keys[], int *result_set_size);

#endif
