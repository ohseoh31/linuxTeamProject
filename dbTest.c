
#include <stdio.h>  
#include <stdlib.h>  
#include "/usr/include/mysql/mysql.h"    
MYSQL *conn_ptr;  
MYSQL_RES *res_ptr;  
MYSQL_ROW sqlrow;  

  
void db_init() {  	
	conn_ptr = mysql_init(NULL);  
        if(!conn_ptr) {  
		printf("error");  
		exit(0);  
	}
	//DB서버접속
	conn_ptr = mysql_real_connect(conn_ptr,"localhost","root"
			,"soek12","linux_db",0,NULL,0);  
	        if(conn_ptr) {  
			printf("sucess\n");  
		}  
		        //mysql_close(conn_ptr); 
	
}
void Search_People(){

	
	if(mysql_query(conn_ptr,"select * from people")){
			printf("query fail\n");
				exit(1);
		}	
		res_ptr =mysql_store_result(conn_ptr); //qurry result
		while((sqlrow =mysql_fetch_row(res_ptr))!=NULL){
			printf("%s %s %s %s %s %s %s\n"
		,sqlrow[0],sqlrow[1],sqlrow[2],sqlrow[3]
		,sqlrow[4],sqlrow[5],sqlrow[6]);
	}
}
void insert_People(){
    res_ptr = mysql_query(&conn_ptr, "INSERT INTO people VALUES('10', '','','','','','')");

}
int main(int argc, char **argv) {  
	 db_init();
	 insert_People();
	 Search_People();
	 return 0; 
}  


