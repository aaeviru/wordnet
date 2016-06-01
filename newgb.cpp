#include "newgb.h"

int main(int argc,char** argv){
	sqlite3 *db;
   int rc;
   rc = sqlite3_open("wnjpn.1.1.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }
	char sql[1000];
	int num;
   snprintf(sql,1000,"select count(*) from ancestor where synset2 in (select synset2 from ancestor where synset1 in (select synset from sense where wordid = %d) and hops = 1) and hops = 1",186607);
   ssql(db,sql,callback2,(void*)(&num));
	printf("%d\n",num);

	return 0;
}
