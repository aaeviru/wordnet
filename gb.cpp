#include "gb.h"
const int BktSz = 4;
const int SegSz = 512;
#define INPUT "/home/ec2-user/git/wordnet/result/sequence_n1.txt"
#define SYNSET "/home/ec2-user/git/wordnet/result/synlink_n.txt"
#define RANK "/home/ec2-user/git/wordnet/log/wordsp.txt"
#define WORDNUM 249121

int compare (const void * a, const void * b){
  return ( ((Word*)a)->spec - ((Word*)b)->spec );
}

void wordqsort(Word* a,int start,int end){
   qsort(&(a[start]),end-start,sizeof(Word),compare);
}

int wordsp[WORDNUM + 1];
Word word[WORDNUM];
int main(int argc,char** argv){
   if(argc != 2){
      printf("input:sequence file\n");
      return -1;
   }
   memset(wordsp,0,WORDNUM + 1);
   FILE* frank = fopen(RANK,"r");
   int wordid,sp;
   while(fscanf(frank,"%d:%d",&wordid,&sp) != EOF){
      wordsp[wordid] = sp>10000?0:sp;
   }
   fclose(frank);
   
   int i = 0;
   FILE* fin = fopen(argv[1],"r");
   while(fscanf(fin,"%d",&wordid) != EOF){
      word[i].set(wordid,wordsp[wordid]);
      i++;
   }
   fclose(fin);
   const int N = i;
   const int Seg = (N + SegSz - 1) / SegSz;
   const int Bkz = (N + BktSz * SegSz - 1) / (BktSz * SegSz);
   for(i=0;i<Seg;i++){
      int start = i*SegSz;
      int end = (i+1)*SegSz>N?N:(i+1)*SegSz;
      wordqsort(word,start,end);
   }   
   
   for(i=0;i<Bkz;i++){
      for(int k=0;k<SegSz;k++){
         for(int j=0;j<BktSz;j++){
            if((i+Bkz*j)*SegSz+k >= N) continue;
            word[(i+Bkz*j)*SegSz+k].print();
            //if(word[(i+Bkz*j)*SegSz+k].spec < 1) getchar();
            //if(word[(i+Bkz*j)*SegSz+k].wordid == 209968) getchar();
         }
         printf("\n");
      }
   }
      
   return 0;
}
