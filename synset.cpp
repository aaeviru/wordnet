#include "synset.h"

using namespace std;


int main(int argc, char* argv[]){
   if(argc != 3){
      printf("input:output-filename type[0(all)/1(jp)]\n");
      return -1;
   }
   sqlite3 *db;
   int rc,i;
   FILE* fout;
   fout = fopen(argv[1],"w");
   rc = sqlite3_open("wnjpn.1.1.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }
   char sql[1000];
   if(atoi(argv[2]) == 0)
      snprintf(sql,1000,"select distinct synset from sense where synset like '%%n'");
   else
      snprintf(sql,1000,"select distinct synset from sense where lang = 'jpn' and synset like '%%n'");
      
   ssql(db,sql,callback5,(void*)fout);
   fclose(fout);
   sqlite3_close(db);
   printf("over\n");
   return 0;
}
