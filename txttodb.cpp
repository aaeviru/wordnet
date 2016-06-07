#include "txttodb.h"

int main(int argc,char** argv){
	int kk = 30;
   sqlite3 *db;
   int rc;
   rc = sqlite3_open(argv[1], &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }
 
   string sql = string("CREATE TABLE lsa(");
   sql += "wordid int primary key not null";
   for(int i=0;i<kk;i++){
		char sqltmp[100];
		snprintf(sqltmp,100,",topic%d real not null",i);
      sql+= sqltmp;
   }
	sql += ")";
	char *csql = new char[sql.length() + 1];
	strcpy(csql,sql.c_str());
	ssql(db,csql,callback);
	delete[] csql;
	sql.clear();
	FILE* fsg = fopen("/home/ec2-user/data/classinfo/sigma-kk.txt","r");
	FILE* fvt = fopen("/home/ec2-user/data/classinfo/vt-kk.txt","r");
	double sigma[kk];
	int i = 0;
	while(fscanf(fsg,"%lf\n",&(sigma[i])) != EOF ){
		i++;
	}
	double tmp;
	string sql_tmp = string("insert into lsa(wordid");
	for(int i=0;i<kk;i++){
		char sqltmp[100];
      snprintf(sqltmp,100,",topic%d",i);
		sql_tmp+= sqltmp;
	}
	sql_tmp += ") values (";
	char sqltmp[100];
	sql = sql_tmp + "0";
	i = 0;
	int j = 0;
	while(fscanf(fvt,"%lf",&tmp) != EOF ){
		i++;
		if( i == kk ){
			tmp /= sigma[i-1];
			char sqltmp[100];
	      snprintf(sqltmp,100,",%e)",tmp);
			sql += sqltmp;
			i = 0;
			j++;
			csql = new char[sql.length() + 1];
			strcpy(csql,sql.c_str());
			ssql(db,csql,NULL);
			delete[] csql;
			sql.clear();
	      snprintf(sqltmp,100,"%d",j);
			sql = sql_tmp + sqltmp;
		}else{
			tmp /= sigma[i-1];
			char sqltmp[100];
	      snprintf(sqltmp,100,",%e",tmp);
			sql += sqltmp;
		}
	}
	fclose(fsg);
	fclose(fvt);
	sqlite3_close(db);
}

