#ifndef FIGHTER_H
#define FIGHTER_H

#define FIGHTER_SUCCESS 0
#define FIGHTER_FAIL 1

// Local Files:
#include "pds.h"

struct FighterJet {
    int fighter_id;
    char FighterName[30];
    char FighterCallSign[30];
};

int add_fighter(struct FighterJet* newFighter); 
int delete_fighter(int p_key);
int search_fighter_by_id (int p_key, struct FighterJet* rec);
int search_fighter_by_FighterCallSign(char* callsign, struct FighterJet* fighter, int* counter);
void print_fighter(struct FighterJet* testFighter);
int match_fighter_FighterCallSign( void *rec, void *key );

#endif
