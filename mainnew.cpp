#include "main.h"

#define SYSLINK "/home/ko/wordnet/result/synlink.txt"
#define RESULT "/home/ko/wordnet/result/sequence.txt"

set< set<int> > sqset;
set<int> word;
set<string> syn;
void ProcessSynset(sqlite3* db,char* synset){
   set< set<int> >::iterator sqset_it;
   char sql[1000];
   int wordid[1000];
   wordid[0] = 0;
   snprintf(sql,1000,"select wordid from sense where synset = '%s' and lang = 'jpn'",synset);
   //printf("%s\n",sql);
   ssql(db,sql,callback3,(void*)wordid);
   set<int> sq;
   set<int>::iterator iter;
   for(int i = 1;i < wordid[0] + 1;i++){
      if(word.find(wordid[i]) != word.end()){
         for(sqset_it=sqset.begin(); sqset_it!=sqset.end(); ++sqset_it){
            if(sqset_it->find(wordid[i]) != sqset_it->end()){
               for(iter=sqset_it->begin();iter!=sqset_it->end();++iter)
                  sq.insert(*iter);
               sqset.erase(sqset_it);
               break;
            }
         }
      }else{
         sq.insert(wordid[i]);
         word.insert(wordid[i]);
      }
   }
   sqset.insert(sq);
   syn.insert(string(synset));
}
void ProcessRelation(sqlite3* db,char* synset,char* relation,set<string>* syntmp){
   char sql[1000];
   snprintf(sql,1000,"select synset1 from synlink where synset2 = '%s' and link = '%s' union select synset2 from synlink where synset1 = '%s' and link = '%s'",synset,relation,synset,relation);
   ssql(db,sql,callback4,(void*)syntmp);
   cout<<sql<<endl;
}

int main(int argc, char* argv[]){
   set< set<int> >::iterator sqset_it;
   set<int>::iterator word_it;
   sqlite3 *db;
   int rc,i;
   FILE* fp = fopen(SYSLINK,"r");
   rc = sqlite3_open("wnjpn.1.1.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }
   char synset[40];
   int temp;
   int xxx = 0;
   while(fscanf(fp,"%s %d",synset,&temp) != EOF){
      xxx++;
      set<string> syntmp;
      ProcessSynset(db,synset);
      ProcessRelation(db,synset,"hypo",&syntmp);
      set<string>::iterator itt;
      for(itt=syntmp.begin();itt!=syntmp.end();++itt){
         cout<<*itt<<endl;
      }

      getchar();
   }
   set<int>::iterator iter;
   fclose(fp);
   fp = fopen(RESULT,"w");
   for (sqset_it=sqset.begin(); sqset_it!=sqset.end(); ++sqset_it){
      for(iter=sqset_it->begin();iter!=sqset_it->end();++iter)
         fprintf(fp,"%d\n",*iter);
      fprintf(fp,"\n");
   }
   printf("%d\n",xxx);
   for(word_it=word.begin();word_it!=word.end();++word_it){
      printf("%d\n",*word_it);
   }
   fclose(fp);
   sqlite3_close(db);
   return 0;
}

