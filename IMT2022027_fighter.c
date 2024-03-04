#include<stdio.h>
#include<string.h>

#include "pds.h"
#include "IMT2022027_fighter.h"

// Load all the fighters from a CSV file
int store_fighters( char *fighter_data_file )
{
	FILE *cfptr;
	char fighter_line[500], token;
	struct FighterJet c, dummy;
	cfptr = (FILE *) fopen(fighter_data_file, "r");
	while(fgets(fighter_line, sizeof(fighter_line)-1, cfptr)){
		sscanf(fighter_line, "%d%s%s", &(c.fighter_id),c.FighterName,c.FighterCallSign);
		print_fighter( &c );
		add_fighter( &c );
	}
    return PDS_SUCCESS;
}


void print_fighter( struct FighterJet *c )
{
	printf("%d,%s,%s\n", c->fighter_id,c->FighterName,c->FighterCallSign);
}



// Use get_rec_by_key function to retrieve fighter
int search_fighter_by_id( int fighter_id, struct FighterJet *c )
{
	return PDS_get_parent_by_key( fighter_id, c );
}



// Add the given fighter into the repository by calling put_rec_by_key
int add_fighter( struct FighterJet *c )
{
	int status;

	status = PDS_put_parent_rec_by_key( c->fighter_id, c );

	if( status != PDS_SUCCESS ){
		fprintf(stderr, "Unable to add fighter with key %d. Error %d", c->fighter_id, status );
		return FIGHTER_FAIL;
	}
	return FIGHTER_SUCCESS;
}



// Use get_rec_by_non_ndx_key function to retrieve fighter
// Hint: get_rec_by_non_ndx_key( FighterCallSign, c, &match_fighter_FighterCallSign, io_count );
// Return FIGHTER_SUCCESS if fighter is found, FIGHTER_FAILURE if not found
int search_fighter_by_FighterCallSign( char *FighterCallSign, struct FighterJet *c, int *io_count )
{
    int status = PDS_get_parent_by_non_ndx_key(FighterCallSign, c, match_fighter_FighterCallSign, io_count);
    if(status == PDS_SUCCESS) {
        return FIGHTER_SUCCESS;
    }
    else return FIGHTER_FAIL;
}



/* Return 0 if FighterCallSign of the fighter matches with FighterCallSign parameter */
/* Return 1 if FighterCallSign of the fighter does NOT match */
/* Return > 1 in case of any other error */
int match_fighter_FighterCallSign( void *rec, void *key )
{
	// Store the rec in a struct fighter pointer
    // Store the key in a char pointer
    // Compare the FighterCallSign values in key and record
    // Return 0,1,>1 based on above condition
    struct FighterJet* Cont = rec;
    char* FighterCallSign = (char*) key;
    if(strcmp(Cont->FighterCallSign, FighterCallSign) == 0){
        return FIGHTER_SUCCESS;
    }
    else return FIGHTER_FAIL;
}



int delete_fighter(int key) {
    int status = PDS_delete_parent_with_key(key);
    if(status == PDS_SUCCESS) return FIGHTER_SUCCESS;
    else return FIGHTER_FAIL;
}


