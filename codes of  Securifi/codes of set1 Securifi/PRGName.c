#include <mysql/mysql.h>
#include<stdio.h>
int main()
{
MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
char *server="localhost";
char *user="root";
char *password="0000";
char *database="apoorva";
conn=mysql_init(NULL);   //mysql_init() will allocate memory and return a pointer to a MYSQL structure.

if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))  //Establishes a connection to a database server.
{
  printf("Failed to connect mysql server %s Error %s\n",server,mysql_error(conn));   //The mysql_error() function returns the error description of the last MySQL operation.
  return 0;
}
if(mysql_query(conn,"show tables"))  // sends a unique query (i.e.conn over here)(multiple queries are not supported) to the currently active database on the server
{
  printf("Failed to execute queries. Error:%s\n",mysql_error(conn));
  return 0;
}
 res=mysql_use_result(conn);  //read the results of a query, one row at a time
 printf("MYSQL tables in sample databases:\n");
 mysql_free_result(res);       //frees memory used by a result res.Returns TRUE on success, or FALSE on failure.

 if(mysql_query(conn,"select * from appu"))
{
 printf("Failed to execute query .Error:%s\n",mysql_error(conn));
 return 0;
}
 res=mysql_store_result(conn);   //retrieves all data at once nd store it
 if(res==NULL)
 {
  return 0;
 }
int columns=mysql_num_fields(res);   //returns the number of fields in a record. Returns FALSE on failure.
int i=0;
printf("entries in the table appu:\n");
while(row=mysql_fetch_row(res))   {    // returns a row from a record as a numeric array.
 for(i=0;i<columns;i++)
 {
   printf("%s",row[i]?row[i]:"NULL");
 }
  printf("\n");
}
 mysql_free_result(res); //frees memory used by a result res
 mysql_close(conn);  // Close MySQL connection
 return 1;
}
 


