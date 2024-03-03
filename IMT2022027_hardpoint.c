#include "IMT2022027_hardpoint.h"
#include "pds.h"

int store_hardpoint( char *hardpoint_data_file ) {
    
	FILE *cfptr;
	char contact_line[500], token;
	struct Link c, dummy;

	cfptr = (FILE *) fopen(hardpoint_data_file, "r");
	while(fgets(contact_line, sizeof(contact_line)-1, cfptr)){
		//printf("line:%s",contact_line);
		sscanf(contact_line, "%d%d", &(c.parent),&(c.child));
		// print_contact( &c );
		add_hardpoint(c.child, c.parent);
	}
    return PDS_SUCCESS;
}

int search_hardpoint( int parent_id, int linked_keys[], int *result_set_size) {
    int status = PDS_get_links(parent_id, linked_keys, result_set_size);

    if(status != PDS_SUCCESS) {
        fprintf(stderr, "Unable to find plane with missile hardpoints. PlaneID: %d, Error: %d", parent_id, status);
        return HARDPOINT_FAIL;
    } else return HARDPOINT_SUCCESS;
}

int add_hardpoint( int parent_id, int child_id) {
    int status = PDS_put_link_by_key(parent_id, child_id);
    if( status != PDS_SUCCESS ){
		fprintf(stderr, "Unable to add missile: %d to fighter: %d. Error %d", parent_id, child_id, status );
		return HARDPOINT_FAIL;
	}
	return HARDPOINT_SUCCESS;
}

