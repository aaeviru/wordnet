#include "wordnet.h"
#include <stdlib.h>

class Word{
   public:
      int wordid;
      int spec;
      Word(){
         this->wordid = 0;
         this->spec = 0;
      }
      Word(int wordid,int spec){
         this->wordid = wordid;
         this->spec = spec;
      }
      void set(int wordid,int spec){
         this->wordid = wordid;
         this->spec = spec;
      }

      void print(){
         printf("%d %d\n",wordid,spec);
      }
};

