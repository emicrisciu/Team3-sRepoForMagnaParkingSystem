#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

int main(int argc, char **argv)
{
  FILE *fptr;
  fptr=fopen("/home/team3/Desktop/proiect/fisier.txt","r");
  char nr_inmat[10];
  char comanda_sql[100];
  nr_inmat[0]='\0';
  comanda_sql[0]='\0';
  if(fptr==NULL)
  {
    printf("eroare baza de date fisier!");
    exit(1);
  }
  fgets(nr_inmat,sizeof(nr_inmat),fptr);
  fgets(nr_inmat,sizeof(nr_inmat),fptr);
  strcpy(nr_inmat,"B59PKW");
  //printf("Hello world");
  //printf("MySQL client version: %s\n", mysql_get_client_info());
  
    MYSQL *con = mysql_init(NULL);

  if (con == NULL)
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      exit(1);
  }

  if (mysql_real_connect(con, "localhost", "team3", "team3",
          "PARCARE", 0, NULL, 0) == NULL)
  {
      finish_with_error(con);
  }
  
  strcpy(comanda_sql,"SELECT count(*) FROM MASINI WHERE NR_INMATRICULARE='");
  strcat(comanda_sql,nr_inmat);
  strcat(comanda_sql,"'");
  
  if (mysql_query(con, comanda_sql)) {
      finish_with_error(con);
  }
  
  MYSQL_RES *result = mysql_store_result(con);
  
  if(result==NULL)
  {
		finish_with_error(con);
  }
  int num_fields = mysql_num_fields(result);
  MYSQL_ROW row;
  row=mysql_fetch_row(result);
  printf("ceva:%s",row[0] ? row[0] : "NULL");
  
  if(row[0]!=0)
  {
    strcat(comanda_sql,"INSERT INTO MASINI(NR_INMATRICULARE) VALUES('");
    strcat(comanda_sql,nr_inmat);
    strcat(comanda_sql,"')");
    if (mysql_query(con, comanda_sql)) {
      finish_with_error(con);
    }
  }



  mysql_close(con);

  exit(0);
}
