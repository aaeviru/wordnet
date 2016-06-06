#include "wordnet.h"
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;


class Synnum{
   public:
		string syn;
      int hos;
      Synnum(){
         this->hos = 0;
      }
      Synnum(string syn,int hos){
         this->syn = syn;
         this->hos = hos;
      }
};

