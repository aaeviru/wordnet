#include "wordlist.h"
#define INPUT "/home/ec2-user/git/statresult/wordslist_dsw.txt"
#define WORDNUM 249121

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    stringstream ss(s);
    string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

int main(){
   ofstream fout;
   fout.open("result/wordlist.txt",ofstream::out);
   map<string,int> word;
   char term[200];
   FILE* fp = NULL;
   fp = fopen("/home/ec2-user/git/statresult/wordslist_dsw.txt","r");
   int num = 0;
   while(fscanf(fp,"%s\n",term) != EOF){
      word[string(term)] = num;
      num++;
   }
   fclose(fp);

   sqlite3 *db;
   int rc;
   rc = sqlite3_open("wnjpn.1.1.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }
   for(int i=1;i<=WORDNUM;i++){
      char sql[1000];
      snprintf(sql,1000,"select lemma from word where wordid = %d",i);
      string term;
      ssql(db,sql,callback7,(void*)(&term));
      vector<string> termss = split(term,' ');
      vector<string>::iterator it;
      if(termss.size() > 1){
         it=termss.begin();
         term = *it;
         for(it++;it!=termss.end();++it){
            term = term +"_" + *it;
         }
         cout<<term<<endl;
      }
      vector<string> terms = split(term,'_');
      for(it=terms.begin();it!=terms.end();++it){
         if(word.find(*it) != word.end() ){
            fout<<i<<" "<<word[*it]<<" "<<term<<endl;
         }else{
            fout<<i<<" "<<term<<endl;
         }
      }
   }
   fout.close();
   return 0;
}
