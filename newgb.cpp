#include "newgb.h"


void sqls(sqlite3 *db,const char* filepath){
	FILE* fin = fopen(filepath,"r");
	char tmpt[100];
	map<string,string> terms;
   while(fscanf(fin,"%s",tmpt) != EOF){
		char sql[1000];
		snprintf(sql,1000,"select synset from sense where wordid in (select wordid from word where lemma = '%s' and pos = 'n')",tmpt);
		set<string> syntmp;
   	ssql(db,sql,callback4,(void*)(&syntmp));
		int num = syntmp.size();
		if(num > 0){
			if(num > 1){
				set<string>::iterator itt;
				int min = 100000;
				string minsyn = "";
				for(itt=syntmp.begin();itt!=syntmp.end();++itt){
					int hops;
					snprintf(sql,1000,"select hops from ancestor where synset1 = '%s' and synset2 = '00001740-n'",(*itt).c_str());
					ssql(db,sql,callback2,(void*)(&hops));
					if(hops < min){
						min = hops;
						minsyn = *itt;
					}
				}
				terms[string(tmpt)] = minsyn;
			}else{
				terms[string(tmpt)] = *(syntmp.begin());
			}
			printf("%s %s\n",tmpt,terms[string(tmpt)].c_str());
		}
	}
	map<string,string>::iterator itt;
  	for(itt=terms.begin();itt!=terms.end();++itt){
		char sql[1000];
		int num = 0,hops=1;
		vector<string> syntmp;
		while(num < 4){
			syntmp.clear();
			snprintf(sql,1000,"select synset1 from ancestor where synset2 in (select synset2 from ancestor where synset1 = '%s' and hops = %d) and hops = %d",itt->second.c_str(),hops,hops);
			ssql(db,sql,callback8,(void*)(&syntmp));
			num = syntmp.size();
			hops++;
			cout<<sql<<' '<<hops<<' '<<num<<endl;
		}
		int j;
		for(j=0;j<num;j++){
			if(syntmp[j] == (itt->second).c_str())
				break;
		}
		for(int k=j % (num/4);k<num;k=k+(num/4)){
			string wordtmp;
			snprintf(sql,1000,"select lemma from word where wordid in (select wordid from sense where synset = '%s')",syntmp[k].c_str());
			ssql(db,sql,callback7,(void*)(&wordtmp));
			printf("%s\n",wordtmp.c_str());
		}
	}
	fclose(fin);

}

int List(sqlite3 *db,const char *path) {
    //printf("%s\n",path);
    struct dirent* ent = NULL;
    DIR *pDir;
    pDir = opendir(path);
    if (pDir == NULL) {
        //被当作目录，但是执行opendir后发现又不是目录，比如软链接就会发生这样的情况。  
        return 0;
    }
    while (NULL != (ent = readdir(pDir))) {
    	if (ent->d_type == 8) {
      	//file  
         string _path(path);
         string _dirName(ent->d_name);
         string fullDirPath = _path + "/" + _dirName;
         if(_dirName.at(_dirName.length()-1) == 't'){
         //      printf("%s\n",ent->d_name);
         	sqls(db,fullDirPath.c_str()); 
				return -1;
			}
         	//getchar(); 
		} else {
      	if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
         	continue;
         }
         //directory  
         string _path(path);
         string _dirName(ent->d_name);
         string fullDirPath = _path + "/" + _dirName;
         if(List(db,fullDirPath.c_str()) < 0)
         	return -1;
		}
	}
   closedir(pDir);
   return 0;
}

int main(int argc,char** argv){
	sqlite3 *db;
   int rc;
   rc = sqlite3_open("wnjpn.1.1.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }
	List(db,"/home/ec2-user/data/topics");
/*
	char sql[1000];
	int num,i = 0;
	int wordit = 175707;
	while(true){
		i++;
	   snprintf(sql,1000,"select count(*) from ancestor where synset2 in (select synset2 from ancestor where synset1 in (select synset from sense where wordid = %d) and hops = %d) and hops = %d",wordid,i,i);
   	ssql(db,sql,callback2,(void*)(&num));
		if(num > 3) break;
	}
*/

	return 0;
}
