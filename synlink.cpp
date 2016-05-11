#include "synlink.h"

bool compare (Synset first, Synset second){
  return ( first.rel() > second.rel() );
}

int main(int argc, char* argv[]){
   if(argc != 3){
      printf("input:input-filename output-filename\n");
      return -1;
   }
   list<Synset> synsetlist;
   sqlite3 *db;
   int rc,i;
   FILE  *fp,*fout;
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
   int tmpnum = 0;
   fp = fopen(argv[1],"r");
   fout = fopen(argv[2],"w");
   while(fscanf(fp,"%s",tmpsyn) != EOF){
      total++;
      char sql[1000];
      snprintf(sql,1000,"select count(*) from synlink where synset2 = '%s' or synset1 = '%s'",tmpsyn,tmpsyn);
      ssql(db,sql,callback2,(void*)&tmpnum);
      synsetlist.push_back(Synset(tmpsyn,tmpnum));
   }
   synsetlist.sort(compare);
   for (iter = synsetlist.begin(); iter != synsetlist.end(); iter++){
      iter->fileout(fout);
   }
   printf("over\n");
   fclose(fp);
   fclose(fout);
   sqlite3_close(db);
   printf("%s\n",total);
   return 0;
}
    
