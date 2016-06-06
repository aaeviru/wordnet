#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <list>
#include "travedir.h"
#include <set>
#include <vector>

class Synset{
   public:
      std::string synset;
      int relationships;
      Synset(char* sys,int rel){
         synset = std::string(sys);
         relationships = rel;
      }
      void display(){
         std::cout<<synset<<' '<<relationships<<std::endl;
      }
      void fileout(FILE* fp){
           fprintf(fp,"%s %d\n",synset.c_str(),relationships);
      }
      int rel(){
         return relationships;
      }
};
int callback(void *data, int argc, char **argv, char **azColName);
int callback2(void *data, int argc, char **argv, char **azColName);
int callback3(void *data, int argc, char **argv, char **azColName);
int callback4(void *data, int argc, char **argv, char **azColName);
int callback5(void *data, int argc, char **argv, char **azColName);
int callback6(void *data, int argc, char **argv, char **azColName);
int callback7(void *data, int argc, char **argv, char **azColName);
int callback8(void *data, int argc, char **argv, char **azColName);
int ssql(sqlite3 *db,char* sql,int (*callback)(void*,int,char**,char**),void* data = NULL);
int ssql2(sqlite3 *db,char* sql);
void db_init(sqlite3 *db);
