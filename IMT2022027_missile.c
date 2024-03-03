#include<stdio.h>

// Local Files:
#include "pds.h"
#include "IMT2022027_missile.h"

// Load all the missiles from a CSV file
int store_missiles( char *missile_data_file )
{
	FILE *cfptr;
	char missile_line[500], token;
	struct Missile c, dummy;

	cfptr = (FILE *) fopen(missile_data_file, "r");
	while(fgets(missile_line, sizeof(missile_line)-1, cfptr)){
		//printf("line:%s",missile_line);
		sscanf(missile_line, "%d%s%s", &(c.MissileID),c.MissileName,c.MissileCallSign);
		print_missile( &c );
		add_missile( &c );
	}
    return PDS_SUCCESS;
}

void print_missile( struct Missile *c )
{
	printf("%d,%s,%s\n", c->MissileID,c->MissileName,c->MissileCallSign);
}

// Use get_rec_by_key function to retrieve missile
int search_missile( int MissileID, struct Missile *c )
{
	return PDS_get_child_by_key( MissileID, c );
}

// Add the given missile into the repository by calling put_rec_by_key
int add_missile( struct Missile *c )
{
	int status;

	status = PDS_put_child_rec_by_key( c->MissileID, c );

	if( status != PDS_SUCCESS ){
		fprintf(stderr, "Unable to add missile with key %d. Error %d", c->MissileID, status );
		return MISSILE_FAIL;
	}
	return status;
}

/*
// Use get_rec_by_non_ndx_key function to retrieve missile
// Hint: get_rec_by_non_ndx_key( MissileCallSign, c, &match_missile_MissileCallSign, io_count );
// Return MISSILE_SUCCESS if missile is found, MISSILE_FAILURE if not found
int search_missile_by_MissileCallSign( char *MissileCallSign, struct Missile *c, int *io_count )
{
    int status = get_rec_by_non_ndx_key(MissileCallSign, c, match_missile_MissileCallSign, io_count);
    if(status == PDS_SUCCESS) {
        return MISSILE_SUCCESS;
    }
    else return status;
} */

/* Return 0 if MissileCallSign of the missile matches with MissileCallSign parameter */
/* Return 1 if MissileCallSign of the missile does NOT match */
/* Return > 1 in case of any other error */
/*
int match_missile_MissileCallSign( void *rec, void *key )
{
    // printf("b");
	// Store the rec in a struct missile pointer
    // Store the key in a char pointer
    // Compare the MissileCallSign values in key and record
    // Return 0,1,>1 based on above condition
    struct Missile* Cont = rec;
    char* MissileCallSign = (char*) key;
    // printf("\nThe REC Value:%d %s %s\t The key: %s\n",Cont->MissileID, Cont->MissileName, Cont->MissileCallSign, MissileCallSign);
    if(strcmp(Cont->MissileCallSign, MissileCallSign) == 0){
        // printf("Matcher is returning: %d %s %s\n", Cont->MissileID, Cont->MissileName, Cont->MissileCallSign);
        return MISSILE_SUCCESS;
    }
    else return MISSILE_FAIL;
} 

*/

/*
int delete_missile(int key) {
    int status = delete_rec_by_ndx_key(key);
    if(status == PDS_SUCCESS) return MISSILE_SUCCESS;
    else return MISSILE_FAILURE;
} */


