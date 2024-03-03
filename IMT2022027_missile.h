#ifndef MISSILE_H
#define MISSILE_H

#define MISSILE_SUCCESS 0
#define MISSILE_FAIL 1

struct Missile {
    int MissileID;
    char MissileName[30];
    char MissileCallSign[30];
};


void print_missile( struct Missile *c );
int add_missile( struct Missile *c );
int store_missiles( char *missile_data_file );
int search_missile( int MissileID, struct Missile *c );

#endif
