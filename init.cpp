#include "init.h"
const int BktSz = 4;
const int SegSz = 512;
#define INPUT "/home/ko/wordnet/result/sequence.txt"
#define SYNSET "/home/ko/wordnet/result/synlink_n.txt"
#define RANK "/home/ko/wordnet/result/rank2.txt"
#define WORDNUM 249121
#define SYNNUM 15300051
//map<int,int> synsp;
int wordsp[WORDNUM + 1];
int synsp[SYNNUM + 1];
int syntoi(string syn){
   char syntmp[10];
   snprintf(syntmp,10,"%s",syn.c_str());
   syntmp[8] = '\0';
   return atoi(syntmp);
}

int main(){
   sqlite3 *db;
   int rc;
   rc = sqlite3_open("wnjpn.1.1.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }

   int n = 0;
   //FILE* fin = fopen(INPUT,"r");
   FILE* fsyn = fopen(SYNSET,"r");
   FILE* frank = fopen(RANK,"r");
   char syn[20];
   int wordid;
   while(fscanf(fsyn,"%s %d",syn,&wordid) != EOF){
      int sp = 100000;
      char sql[1000];
      snprintf(sql,1000,"select hops from ancestor where synset1 = '%s' and synset2 = '00001740-n'",syn);
      ssql(db,sql,callback6,(void*)&sp);
      syn[8] = '\0';
      synsp[atoi(syn)] = sp;
      n++;
      cout<<n<<endl;
      fflush(stdout);
   }
   for(int i=0;i<WORDNUM;i++){
      char sql[1000];
      set<string> syntmp;
      snprintf(sql,1000,"select synset from sense where wordid = %d",i+1);
      ssql(db,sql,callback4,(void*)(&syntmp));
      set<string>::iterator itt;
      int sp = 100000;
      for(itt=syntmp.begin();itt!=syntmp.end();++itt){
         if(sp > synsp[syntoi(*itt)]) sp = synsp[syntoi(*itt)];
      }
      wordsp[i+1] = sp;
      cout<<i+1<<':'<<sp<<endl;

   }
   map<int,int> ::iterator iter;
   cout<<n<<endl;

   const int N = n;
   const int Seg = (N + SegSz - 1) / SegSz;
   return 0;
}
