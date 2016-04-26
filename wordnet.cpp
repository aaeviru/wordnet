#include "wordnet.h"

using namespace std;


bool compare (Synset first, Synset second){
  return ( first.rel() > second.rel() );
}

int tmpnum;
list<Synset> synsetlist;

FILE* fout;
static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   
   fprintf(stderr, "%s\n", (const char*)data);
   for(i=0; i<argc; i++){
      printf("%s = %s ", azColName[i], argv[i] ? argv[i] : "NULL");
   }
//   num += argv[0] ? 1:0;
   printf("\n");
   return 0;
}

static int callback2(void *data, int argc, char **argv, char **azColName){
   tmpnum = atoi(argv[0]);
   return 0;
}

static int callback5(void *data, int argc, char **argv, char **azColName){
   fprintf(fout,"%s\n",argv[0]);
   return 0;
}


int callback3(void *data, int argc, char **argv, char **azColName){
   ((int*)data)[0]++;
   if(((int*)data)[0] >= 1000){
      printf("error over 1000\n");
      return -1;
   }
   ((int*)data)[((int*)data)[0]] = atoi(argv[0]);
   return 0;
}

int callback4(void *data, int argc, char **argv, char **azColName){
   ((set<string>*)data)->insert(argv[0]);
   return 0;
}

int callback6(void *data, int argc, char **argv, char **azColName){
   if(*(int*)data > atoi(argv[0])) *(int*)data = atoi(argv[0]);
   return 0;
}

int callback7(void *data, int argc, char **argv, char **azColName){
   *(string*)data = string(argv[0]);
   return 0;
}
   

int ssql(sqlite3 *db,char* sql,int (*callback)(void*,int,char**,char**),void* data){
   char *zErrMsg = 0;
   int rc;
//   sql = "select * from sense where synset = '06402202-n'";
/*   Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s %s\n", sql,zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      //fprintf(stdout, "Operation done successfully\n");
   }

}
int ssql2(sqlite3 *db,char* sql){
   char *zErrMsg = 0;
   int rc;
   const char* data = "Callback function called";
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
//      fprintf(stdout, "Operation done successfully\n");
   }

}

void db_init(sqlite3 *db){
   int rc;
   FILE* fp;
   rc = sqlite3_open("wnjpn.1.1.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }
}
/*

int main(int argc, char* argv[]){
   sqlite3 *db;
   int rc,i;
   FILE* fp;
//   fout = fopen("/home/ko/wordnet/result/synset.txt","w");
   rc = sqlite3_open("wnjpn.1.1.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }
   int total = 0;
   list<Synset>::iterator iter;
   char tmpsyn[20];
   fp = fopen("/home/ko/wordnet/result/synset.txt","r");
   fout = fopen("/home/ko/wordnet/result/synlink.txt","w");
   while(fscanf(fp,"%s",tmpsyn) != EOF){
      total++;
      char sql[1000];
      snprintf(sql,1000,"select count(*) from synlink where synset2 = '%s' or synset1 = '%s'",tmpsyn,tmpsyn);
      ssql(db,sql,callback2);
      synsetlist.push_back(Synset(tmpsyn,tmpnum));
   }
   synsetlist.sort(compare);
   for (iter = synsetlist.begin(); iter != synsetlist.end(); iter++){
      iter->fileout(fout);
   }

   fclose(fp);
   fclose(fout);
   sqlite3_close(db);
   printf("%s\n",total);
   return 0;
}
*/
