#include<stdio.h>
#include <stdlib.h>

// Include the header file that contains the struct declarations for parent and child records
#include "IMT2022027_fighter.h"
#include "IMT2022027_hardpoint.h"
#include "IMT2022027_missile.h"
#include "pds.h"

// Declare global variables for 10 parent and 10 child records

void show_menu();
void setup_data();
void process_option( int option );
void process_option1();
void process_option2();

int main()
{
    int option;
    setup_data();
    do{
        show_menu();
        scanf("%d",&option);
        process_option(option);
    } while (option != 0);
    return 0;
}

void show_menu()
{
    printf("\nLINKED DATA DEMO\n\n");
    printf("0. Exit\n");
    printf("1. Add linked data\n");
    printf("2. Get linked data\n");
    printf("\nEnter option: ");  
}

void setup_data()
{
    int status = PDS_Create("Fighter", "Missile");
    if(status != PDS_SUCCESS) {printf("Unable to create files\n");}
    status = PDS_Open("Fighter", "Missile", sizeof(struct FighterJet), sizeof(struct Missile));
    if(status != PDS_SUCCESS) {printf("Unable to open files\n");}
    // statically create 10 parent and 10 child records individually
    for(int i = 0; i < 10; i++ ){
        struct FighterJet newFighter;
        struct Missile newMissile;
        
        // Creating FighterJet:
        snprintf(newFighter.FighterName, 30, "Name-of-Fighter-%d", i);
        snprintf(newFighter.FighterCallSign, 30, "Call-of-Fighter-%d", i);
        newFighter.fighter_id = 1000 + i;

        // Creating Missile:
        snprintf(newMissile.MissileName, 30, "Name-of-Fighter-%d", i);
        snprintf(newMissile.MissileCallSign, 30, "Call-of-Fighter-%d", i);
        newMissile.MissileID = 100 + i;

        int status_fighter = add_fighter(&newFighter);
        if(status_fighter != FIGHTER_SUCCESS) {printf("Failed to add Fighter\n");}
        status_fighter = add_missile(&newMissile);
        if(status_fighter != FIGHTER_SUCCESS) {printf("Failed to add Missile\n");}
    }
}

void process_option( int option )
{
    switch(option){
        case 0: 
            // do nothing
            break;
        case 1:
            process_option1();
            break;
        case 2:
            process_option2();
            break;
        default:
            printf("Invalid option\n");
            break;
    }
}

void process_option1()
{
    int parent_key, child_key;
    printf("Enter parent key and child key for linking: ");
    scanf("%d%d", &parent_key, &child_key);
    // TODO: Call appropriate PDS functions here to link the two records
    int status = add_hardpoint(parent_key, child_key);
    if(status != HARDPOINT_SUCCESS) {printf("Adding new hardpoint\n");}
}

void process_option2()
{
    int parent_key;
    printf("Enter parent key and child key for linking: ");
    scanf("%d", &parent_key);
    // TODO: Call appropriate PDS functions here to fetch arrach of child keys
    // Call your PDS functions to display the list of child keys retrieved for the given parent key
    // Display all field values of each child record in a nicely formatted manner
    int count;
    int arr[10000];
    
    search_hardpoint(parent_key, arr, &count);

    if(count > 10000) {printf("Too Many Records to Handle\n"); exit(1);}

    for(int i = 0; i < count; i++) {
        struct Missile newMissile;
        search_missile(arr[i], &newMissile);
        print_missile(&newMissile);
    }
    printf("Exiting Search: %d\n", count);
}
