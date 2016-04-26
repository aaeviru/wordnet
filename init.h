#include "wordnet.h"
#include <map>
#include <vector>

class Synsp{
   public:
      int synset;
      int spec;
      Synsp(int synset = 0,int spec = 0){
         this->synset = synset;
         this->spec = spec;
      }
      void print() const{
         printf("%d %d\n",synset,spec);
      }
};


using namespace std;

