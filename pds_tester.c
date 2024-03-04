#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "pds.h"
#include "IMT2022027_fighter.h"
#include "IMT2022027_missile.h"
#include "IMT2022027_hardpoint.h"

#define TREPORT(a1,a2) printf("Status: %s - %s\n\n",a1,a2); fflush(stdout);

int compare_arrays_elementwise(const int *arr1, const int *arr2, size_t size) {
  // Check if the sizes are different, indicating non-equality
  // Loop through each element and compare
  for (size_t i = 0; i < size; i++) {
    if (arr1[i] != arr2[i]) {
      return 0;
    }
  }

  // If the loop finishes without finding a difference, the arrays are equal
  return 1;
}

void array_to_string_buffer(const int *arr, int num_elements, char *str, int buffer_size) {
  // Check if the provided buffer has enough space
  if (buffer_size < 2 + num_elements * 4 || buffer_size < 0) {
    return;  // Indicate insufficient buffer size
  }

  // Initialize the string pointer within the buffer
  char *str_ptr = str;

  // Write opening bracket
  *str_ptr++ = '[';

  // Iterate through the array elements
  for (int j = 0; j < num_elements; j++) {
    // Use snprintf to write the number and check its length
    int num_digits = snprintf(str_ptr, buffer_size - (str_ptr - str), "%d", arr[j]);

    // Update buffer size and pointer based on written characters
    buffer_size -= num_digits;
    str_ptr += num_digits;

    // Check if buffer space is still sufficient, including comma and closing bracket
    if (buffer_size < 2) {
      return;  // Indicate insufficient buffer space
    }

    // Append comma if not the last element
    if (j != num_elements - 1) {
      *str_ptr++ = ',';
      buffer_size--;
    }
  }

  // Write closing bracket and null terminator
  *str_ptr++ = ']';
  *str_ptr = '\0';
}


void process_line( char *test_case );

int main(int argc, char *argv[])
{
	FILE *cfptr;
	char test_case[50];

	if( argc != 2 ){
		fprintf(stderr, "Usage: %s testcasefile\n", argv[0]);
		exit(1);
	}

	cfptr = (FILE *) fopen(argv[1], "r");
	while(fgets(test_case, sizeof(test_case)-1, cfptr)){
		// printf("line:%s",test_case);
		if( !strcmp(test_case,"\n") || !strcmp(test_case,"") )
			continue;
		process_line( test_case );
	}
}

void process_line( char *test_case )
{
	char repo_name[30], linked_repo_name[30];
	char command[100], param1[15], param2[15], param3[15], info[1024];
	int fighter_id, missile_id, status, rec_size, expected_status, linked_rec_size;
	struct FighterJet testFighterJet;
    struct Missile testMissile;
    struct Link testHardPoint;

    char Mode[10];

	// strcpy(testFighterJet.FighterName, "dummy name");
	// strcpy(testFighterJet.phone, "dummy number");

	rec_size = sizeof(struct FighterJet);
    linked_rec_size = sizeof(struct Missile);
    // sscanf()
	sscanf(test_case, "%s%s%s%s", command, param1, param2, param3);
	printf("Test case: %s", test_case); fflush(stdout);
	if( !strcmp(command,"CREATE") ){
		strcpy(repo_name, param1);
		strcpy(linked_repo_name, param2);
		if( !strcmp(param3,"0") )
			expected_status = FIGHTER_SUCCESS;
		else
			expected_status = FIGHTER_FAIL;

		status = PDS_Create( repo_name, linked_repo_name);
		if(status == PDS_SUCCESS)
			status = FIGHTER_SUCCESS;
		else
			status = FIGHTER_FAIL;
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"pds_create returned status %d",status);
			TREPORT("FAIL", info);
		}
	}
	else if( !strcmp(command,"OPEN") ){
		strcpy(repo_name, param1);
		strcpy(linked_repo_name, param2);
		if( !strcmp(param3,"0") )
			expected_status = FIGHTER_SUCCESS;
		else
			expected_status = FIGHTER_FAIL;
        
        // printf("b\n");
		status = PDS_Open( repo_name, linked_repo_name ,rec_size, linked_rec_size);
        // printf("b\n");
		if(status == PDS_SUCCESS)
			status = FIGHTER_SUCCESS;
		else
			status = FIGHTER_FAIL;
        // printf("%d %d", status, expected_status);
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"pds_open returned status %d",status);
			TREPORT("FAIL", info);
		}
	}
	else if( !strcmp(command,"STORE_FIGHTER") ){
        //  printf("STORE has been read\n");
		if( !strcmp(param2,"0") )
			expected_status = FIGHTER_SUCCESS;
		else
			expected_status = FIGHTER_FAIL;

		sscanf(param1, "%d", &fighter_id);
		testFighterJet.fighter_id = fighter_id;
		sprintf(testFighterJet.FighterCallSign, "CallSign-of-%d", fighter_id);
		sprintf(testFighterJet.FighterName, "Name-of-%d", fighter_id);
		// sprintf(testFighterJet.fighter_type, "Name-of-%d", fighter_id);
		status = add_fighter( &testFighterJet );
		if(status == PDS_SUCCESS)
			status = FIGHTER_SUCCESS;
		else
			status = FIGHTER_FAIL;
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"add_fighter returned status %d",status);
			TREPORT("FAIL", info);
		}
	} else if ( !strcmp(command, "STORE_MISSILE")) {
		if( !strcmp(param2,"0") )
			expected_status = FIGHTER_SUCCESS;
		else
			expected_status = FIGHTER_FAIL;

		sscanf(param1, "%d", &fighter_id);
		testMissile.MissileID = fighter_id;
		sprintf(testMissile.MissileCallSign, "FighterCallSign-of-%d", fighter_id);
		sprintf(testMissile.MissileName, "Name-of-%d", fighter_id);
		// sprintf(testMissile.range, "Name-of-%d", fighter_id);
		status = add_missile( &testMissile );
		if(status == PDS_SUCCESS)
			status = FIGHTER_SUCCESS;
		else
			status = FIGHTER_FAIL;
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"add_fighter returned status %d",status);
			TREPORT("FAIL", info);
		}
	} else if ( !strcmp(command, "STORE_HARDPOINT")) {
		if( !strcmp(param3,"0") )
			expected_status = FIGHTER_SUCCESS;
		else
			expected_status = FIGHTER_FAIL;

		sscanf(param1, "%d", &fighter_id);
		sscanf(param2, "%d", &missile_id);
		testHardPoint.parent = fighter_id;
		testHardPoint.child = missile_id;

        status = add_hardpoint(fighter_id, missile_id);        
		if(status == PDS_SUCCESS)
			status = FIGHTER_SUCCESS;
		else
			status = FIGHTER_FAIL;
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"add_hardpoint returned status %d",status);
			TREPORT("FAIL", info);
		}
    }else if( !strcmp(command,"SEARCH_HARDPOINTS") ){
        printf("Hello\n");
		char expected_name[30], expected_phone[30];
		int expected_io[1000], actual_io[1000], expected_size; int count_obtained;
		
		if( strcmp(param3,"-1") == 0 )
			expected_status = FIGHTER_FAIL;
		else
			expected_status = FIGHTER_SUCCESS;

		sscanf(param1, "%d", &fighter_id);
		testFighterJet.fighter_id = -1;
		// actual_io = 0;


        // Count the number of elements within the brackets
        int count = 1;  // Initially 1 for the first element
        for (int i = 1; param2[i] != ']'; i++) {
            if (param2[i] == ',') {
                count++;
            }
        }

       
        // Parse the string and convert elements to integers
        int i = 1, j = 0;
        while (param2[i] != ']') {
            int num = 0;
            while (param2[i] != ',' && param2[i] != ']') {
                num = num * 10 + (param2[i] - '0');
                i++;
            }
            expected_io[j++] = num;
            if (param2[i] == ',') {
                i++;
            }
        }

        status = search_hardpoint( fighter_id , actual_io, &count_obtained );
        
        if(status == PDS_SUCCESS)
            status = FIGHTER_SUCCESS;
        else
            status = FIGHTER_FAIL;

        // printf("%d %d\n", status, expected_status);
        if( status != expected_status ){
            sprintf(info,"search key: %d; Got status %d",fighter_id, status);
            TREPORT("FAIL", info);
        }
        else{
            // Check if the retrieved values match
            // Check if num block accesses match too
            // Extract the expected fighter_id from the phone number
            // sscanf(phone_num+sizeof("Phone-of"), "%d", &fighter_id);
            // sprintf(expected_name,"Name-of-%d",fighter_id);
            // sprintf(expected_phone,"Phone-of-%d",fighter_id);
            char expected_array[200], actual_array[200];
            array_to_string_buffer(expected_io, count, expected_array, 200 );
            array_to_string_buffer(actual_io, count, actual_array, 200 );
            if( expected_status == 0 ){
                if (count_obtained == count && compare_arrays_elementwise(expected_io, actual_io, count)){
                    TREPORT("PASS", "");
                } else if(count != count_obtained) {
                    sprintf(info, "Count not matching. Count expected: %d, count obtained: %d", count, count_obtained);
                    TREPORT("FAIL", info);
                } else {
                    sprintf(info,"HardPoint data not matching... Expected:{%s} Got:{%s}\n",
						expected_array,
                        actual_array
                            );
					TREPORT("FAIL", info);
				}
			} else
				TREPORT("PASS", "");
		}
	}

	else if( !strcmp(command,"SEARCH_MISSILE_BY_INDEX") ){
		if( !strcmp(param2,"0") )
			expected_status = FIGHTER_SUCCESS;
		else
			expected_status = FIGHTER_FAIL;

		sscanf(param1, "%d", &fighter_id);
		testMissile.MissileID = -1;
		status = search_missile( fighter_id, &testMissile );
		if(status == PDS_SUCCESS)
			status = FIGHTER_SUCCESS;
		else
			status = FIGHTER_FAIL;
		if( status != expected_status ){
			sprintf(info,"search key: %d; Got status %d",fighter_id, status);
			TREPORT("FAIL", info);
		}
		else{
			// Check if the retrieved values match
			char expected_phone[30];
    		sprintf(expected_phone, "FighterCallSign-of-%d", fighter_id);
			char expected_name[30];
    		sprintf(expected_name, "Name-of-%d", fighter_id);
			if( expected_status == 0 ){
				if (testMissile.MissileID == fighter_id && 
					strcmp(testMissile.MissileName,expected_name) == 0 &&
					strcmp(testFighterJet.FighterCallSign, expected_phone) == 0){
						TREPORT("PASS", "");
				}
				else{
					sprintf(info,"Missile data not matching... Expected:{%d,%s,%s} Got:{%d,%s,%s}\n",
						fighter_id, expected_name, expected_phone, 
						testFighterJet.fighter_id, testFighterJet.FighterName, testFighterJet.FighterCallSign
					);
					TREPORT("FAIL", info);
				}
			}
			else
				TREPORT("PASS", "");
		}
	}
	else if( !strcmp(command,"SEARCH_FIGHTER_BY_INDEX") ){
        printf("Hello\n");
		if( !strcmp(param2,"0") )
			expected_status = FIGHTER_SUCCESS;
		else
			expected_status = FIGHTER_FAIL;

		sscanf(param1, "%d", &fighter_id);
		testFighterJet.fighter_id = -1;
        printf("Hello\n");
		status = search_fighter_by_id( fighter_id, &testFighterJet );
		if(status == PDS_SUCCESS)
			status = FIGHTER_SUCCESS;
		else
			status = FIGHTER_FAIL;
        printf("Hello\n");
		if( status != expected_status ){
			sprintf(info,"search key: %d; Got status %d",fighter_id, status);
			TREPORT("FAIL", info);
		}
		else{
			// Check if the retrieved values match
			char expected_phone[30];
    		sprintf(expected_phone, "FighterCallSign-of-%d", fighter_id);
			char expected_name[30];
    		sprintf(expected_name, "Name-of-%d", fighter_id);
			if( expected_status == 0 ){
				if (testFighterJet.fighter_id == fighter_id && 
					strcmp(testFighterJet.FighterName,expected_name) == 0 &&
					strcmp(testFighterJet.FighterCallSign, expected_phone) == 0){
						TREPORT("PASS", "");
				}
				else{
					sprintf(info,"FighterJet data not matching... Expected:{%d,%s,%s} Got:{%d,%s,%s}\n",
						fighter_id, expected_name, expected_phone, 
						testFighterJet.fighter_id, testFighterJet.FighterName, testFighterJet.FighterCallSign
					);
					TREPORT("FAIL", info);
				}
			}
			else
				TREPORT("PASS", "");
		}
	}
	else if( !strcmp(command,"SEARCH_FIGHTER_BY_CALLSIGN") ){
		char phone_num[30], expected_name[30], expected_phone[30];
		int expected_io, actual_io;
		
		if( strcmp(param2, "-1") == 0 )
			expected_status = FIGHTER_FAIL;
		else
			expected_status = FIGHTER_SUCCESS;

		sscanf(param1, "%s", phone_num);
		sscanf(param2, "%d", &expected_io);
		testFighterJet.fighter_id = -1;
		actual_io = 0;
		status = search_fighter_by_FighterCallSign( phone_num, &testFighterJet, &actual_io );
		if(status == PDS_SUCCESS)
			status = FIGHTER_SUCCESS;
		else
			status = FIGHTER_FAIL;

        // printf("%d %d\n", status, expected_status);
		if( status != expected_status ){
			sprintf(info,"search key: %d; Got status %d",fighter_id, status);
			TREPORT("FAIL", info);
		}
		else{
			// Check if the retrieved values match
			// Check if num block accesses match too
			// Extract the expected fighter_id from the phone number
			sscanf(phone_num+sizeof("Phone-of"), "%d", &fighter_id);
			sprintf(expected_name,"Name-of-%d",fighter_id);
			sprintf(expected_phone,"Phone-of-%d",fighter_id);
			if( expected_status == 0 ){
				if (testFighterJet.fighter_id == fighter_id && 
					strcmp(testFighterJet.FighterName, expected_name) == 0 &&
					strcmp(testFighterJet.FighterCallSign, expected_phone) == 0 ){
						if( expected_io == actual_io ){
							TREPORT("PASS", "");
						}
						else{
							sprintf(info,"Num I/O not matching for fighter %d... Expected:%d Got:%d\n",
								fighter_id, expected_io, actual_io
							);
							TREPORT("FAIL", info);
						}
				}
				else{
					sprintf(info,"FighterJet data not matching... Expected:{%d,%s,%s} Got:{%d,%s,%s}\n",
						fighter_id, expected_name, expected_phone, 
						testFighterJet.fighter_id, testFighterJet.FighterName, testFighterJet.FighterCallSign
					);
					TREPORT("FAIL", info);
				}
			}
			else
				TREPORT("PASS", "");
		}
	}
	else if( !strcmp(command,"DELETE_FIGHTER_NDX") ){
		if( strcmp(param2,"0") == 0 )
			expected_status = FIGHTER_SUCCESS;
		else
			expected_status = FIGHTER_FAIL;

		sscanf(param1, "%d", &fighter_id);
		testFighterJet.fighter_id = -1;
		status = delete_fighter( fighter_id );
		if( status != expected_status ){
			sprintf(info,"delete key: %d; Got status %d",fighter_id, status);
			TREPORT("FAIL", info);
		}
		else{
			TREPORT("PASS", "");
		}
	}
	else if( !strcmp(command,"CLOSE") ){
		if( !strcmp(param1,"0") )
			expected_status = FIGHTER_SUCCESS;
		else
			expected_status = FIGHTER_FAIL;

		status = PDS_Close();
		if(status == PDS_SUCCESS)
			status = FIGHTER_SUCCESS;
		else
			status = FIGHTER_FAIL;
		if( status == expected_status ){
			TREPORT("PASS", "");
		}
		else{
			sprintf(info,"pds_close returned status %d",status);
			TREPORT("FAIL", info);
		}
	} else {
        printf("COmmand not detected: %s\n", command);
    }
}


