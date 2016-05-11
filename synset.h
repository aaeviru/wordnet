#include "wordnet.h"
#include <set>
#include <vector>
#include <list>

using namespace std;

class Seq{
   public:
      set<int> seqset;
      vector<int> seqvec;
      Seq(vector<int> sq){
         seqset = set<int>(sq.begin(),sq.end());
         seqvec = vector<int>(sq);
      }
      vector<int>::iterator vecbegin(){
         return seqvec.begin();
      }
      vector<int>::iterator vecend(){
         return seqvec.end();
      }

};
