#include "main.h"

list< Seq > sqset;
set<int> word;
set<string> syn;
int type;
void ProcessSynset(sqlite3* db,const char* synset,vector<int>* sq){
   list< Seq >::iterator sqset_it;
   char sql[1000];
   int wordid[1000];
   wordid[0] = 0;
   if( type == 0)
      snprintf(sql,1000,"select wordid from sense where synset = '%s'",synset);
   else
      snprintf(sql,1000,"select wordid from sense where synset = '%s' and lang = 'jpn'",synset);
   //printf("%s\n",sql);
   ssql(db,sql,callback3,(void*)wordid);
   vector<int>::iterator iter;
   for(int i = 1;i < wordid[0] + 1;i++){
      if(word.find(wordid[i]) != word.end()){
         for(sqset_it=sqset.begin(); sqset_it!=sqset.end(); ++sqset_it){
            if(sqset_it->seqset.find(wordid[i]) != sqset_it->seqset.end()){
               for(iter = sqset_it->vecbegin();iter!=sqset_it->vecend();++iter)
                  sq->push_back(*iter);
               sqset.erase(sqset_it);
               break;
            }
         }
      }else{
         sq->push_back(wordid[i]);
         word.insert(wordid[i]);
      }
   }
   syn.insert(string(synset));
}


void ProcessRelation(sqlite3* db,char* synset,const char* relation,vector<int>* sq){
   char sql[1000];
   snprintf(sql,1000,"select synset1 from synlink where synset2 = '%s' and link = '%s' union select synset2 from synlink where synset1 = '%s' and link = '%s'",synset,relation,synset,relation);
   set<string> syntmp;
   ssql(db,sql,callback4,(void*)(&syntmp));
   set<string>::iterator itt;
   for(itt=syntmp.begin();itt!=syntmp.end();++itt){
      ProcessSynset(db,itt->c_str(),sq);   
   }
//   cout<<sql<<endl;
}

int main(int argc, char* argv[]){
   if(argc != 4){
      printf("input:input-filename output-filename type[0(all)/1(jp)]\n");
      return -1;
   }
   type = atoi(argv[3]);
   list< Seq >::iterator sqset_it;
   set<int>::iterator word_it;
   sqlite3 *db;
   int rc,i;
   FILE* fp = fopen(argv[1],"r");
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
      printf("%d\n",xxx);
      fflush(stdout);
      if(syn.find(string(synset)) != syn.end() ) continue;
      vector<int> sqq;
      vector<int>* sq = &sqq;
      ProcessSynset(db,synset,sq);
      ProcessRelation(db,synset,"hypo",sq);
      ProcessRelation(db,synset,"hype",sq);
      ProcessRelation(db,synset,"hypo",sq);
      ProcessRelation(db,synset,"mmem",sq);
      ProcessRelation(db,synset,"msub",sq);
      ProcessRelation(db,synset,"mprt",sq);
      ProcessRelation(db,synset,"hmem",sq);
      ProcessRelation(db,synset,"hsub",sq);
      ProcessRelation(db,synset,"hprt",sq);
      Seq tmpseq(sqq);
      sqset.push_front(tmpseq);
/*
      vector<int>::iterator iter;
      for (sqset_it=sqset.begin(); sqset_it!=sqset.end(); ++sqset_it){
         for(iter=sqset_it->vecbegin();iter!=sqset_it->vecend();++iter)
            printf("%d\n",*iter);
         printf("\n");
      }
      getchar();
*/


   }
   vector<int>::iterator iter;
   fclose(fp);
   fp = fopen(argv[2],"w");
   for (sqset_it=sqset.begin(); sqset_it!=sqset.end(); ++sqset_it){
      for(iter=sqset_it->seqvec.begin();iter!=sqset_it->seqvec.end();++iter)
         fprintf(fp,"%d\n",*iter);
      fprintf(fp,"\n");
   }
   for(word_it=word.begin();word_it!=word.end();++word_it){
      printf("%d\n",*word_it);
   }
   fclose(fp);
   sqlite3_close(db);
   return 0;
}

