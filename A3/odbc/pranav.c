#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>

#define MAX_ARR_LEN 200

SQLHENV  henv=NULL;
SQLHDBC  hdbc=NULL;

int ODBCConnectDB(SQLCHAR* ds, SQLCHAR* user, SQLCHAR* pw)
{
  SQLRETURN  rc;

  hdbc=NULL;
  henv=NULL;

  // Allocate environment handle
  rc=SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);  
  if (rc==SQL_SUCCESS || rc==SQL_SUCCESS_WITH_INFO) 
  {
    //  Set the ODBC version environment attribute
    rc=SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0); 

    if (rc==SQL_SUCCESS || rc==SQL_SUCCESS_WITH_INFO) 
    {
      // Allocate connection handle 
      rc=SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc); 

      if (rc==SQL_SUCCESS || rc==SQL_SUCCESS_WITH_INFO) 
      {
        // Set login timeout to 5 seconds. 
        SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

        // Connect to data source
        rc=SQLConnect(hdbc, ds, SQL_NTS, user, SQL_NTS, pw, SQL_NTS);
	
        if (rc==SQL_SUCCESS || rc==SQL_SUCCESS_WITH_INFO)
        {
          return 1;
        }
        SQLDisconnect(hdbc);
      }
      SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
    }
    else(printf("Failure in connecting\n"));
  }
  SQLFreeHandle(SQL_HANDLE_ENV, henv);

  hdbc=NULL;
  henv=NULL;

  return 0;
}
int ODBCDisconnectDB()
{
  if(hdbc) 
  {
    SQLDisconnect(hdbc);	
    SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
  }
  if(henv) SQLFreeHandle(SQL_HANDLE_ENV, henv);
  hdbc=NULL;
  henv=NULL;
  return 1;
}
int db_exec_stmt(char* stmt_str)
{
  SQLHSTMT hstmt;
  SQLRETURN  rc;

  if(henv==NULL || hdbc==NULL) return 0;

  /* Allocate statement handle */
  rc = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt); 
  if(!(rc==SQL_SUCCESS || rc==SQL_SUCCESS_WITH_INFO) ) return 0;
	
  rc = SQLExecDirect(hstmt, stmt_str, SQL_NTS);
  SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
  if(rc==SQL_SUCCESS || rc==SQL_SUCCESS_WITH_INFO) return 1;
  if(rc==SQL_NO_DATA) return 1;

  printf("sqlerr: %d\n", rc);
  return 0;
}
int db_fetch()
{
  SQLRETURN r;
  SQLHSTMT hstmt;
  SQLCHAR sql[4096];
  SQLLEN n;
  SQLINTEGER id;
  SQLCHAR sql_name[MAX_ARR_LEN];
  SQLCHAR sql_address[MAX_ARR_LEN];
  SQLCHAR sql_physician[MAX_ARR_LEN];
  SQL_TIMESTAMP_STRUCT sql_date_time;

  if(hdbc==NULL || henv==NULL) return 0;

  r=SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
  if(!(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)) return 0;

  int i;
  printf("Enter the query number: ");
  scanf("%d", &i);
  if(i==1)
  {
    strcpy(sql,"SELECT Name \"Physician Name\" "
                "FROM Physician "
                "WHERE EmployeeID IN (SELECT Physician "
                                     "FROM Trained_In "
                                     "WHERE Treatment = (SELECT Code "
                                                        "FROM Procedures "
                                                        "WHERE Name = \"Bypass Surgery\"));");
    // strcpy(sql, query);
    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    r=SQLFetch(hstmt);
    if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      printf("\nPhysician Name\n");
    else if(SQL_NO_DATA==r) printf("No Data Found related to the query\n");
    while(1)
    {
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
        printf("%s\n",sql_name);
      }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      r=SQLFetch(hstmt);
    }
  }
  
  else if(i==2)
  {
    strcpy(sql,"SELECT Name \"Physician Name\" "
"FROM Physician "
"WHERE EmployeeID IN (SELECT Physician "
                     "FROM Trained_In "
                     "WHERE Treatment = (SELECT Code "
                                        "FROM Procedures "
                                        "WHERE Name = \"Bypass Surgery\") AND Physician IN (SELECT Physician "
                                                                                          "FROM Affiliated_With "
                                                                                          "WHERE Department = (SELECT DepartmentID "
                                                                                                               "FROM Department "
                                                                                                               "WHERE Name = \"Cardiology\")));");
    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    r=SQLFetch(hstmt);
    if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
    printf("\nPhysician Name\n");
    else if(SQL_NO_DATA==r) printf("No Data Found related to the query\n");
    while(1)
    {
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
        printf("%s\n",sql_name);
      }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      r=SQLFetch(hstmt);
    }
  }
  
  else if(i==3)
  {
    strcpy(sql,"SELECT Name \"Nurse Name\" "
"FROM Nurse "
"WHERE EmployeeID IN (SELECT Nurse "
                    "FROM On_Call "
                    "WHERE (BlockFloor,BlockCode) IN (SELECT BlockFloor,BlockCode "
                                                    "FROM Room "
                                                    "WHERE Number = 123));"); 
    
    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    r=SQLFetch(hstmt);
    if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
    printf("\nNurse Name\n");
    else if(SQL_NO_DATA==r) printf("No Data Found related to the query\n");
    while(1)
    {
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
        printf("%s\n",sql_name);
      }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      r=SQLFetch(hstmt);
    }
  }
  

  else if(i==4)
  { 
    strcpy(sql,"SELECT Name \"Patient Name\",Address \"Patient Address\" "
"FROM Patient "
"WHERE SSN IN (SELECT Patient "
                    "FROM Prescribes "
                    "WHERE Medication IN (SELECT Code "
                                    "FROM Medication "
                                    "WHERE Name = 'Remdesivir'));");
    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    
    
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    r=SQLFetch(hstmt);
    if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO){
    printf("\nPatient Name");
    for(int j=0;j<40-strlen("Patient Name");++j)printf(" ");
    printf("Patient Address\n");
    }
    else if(SQL_NO_DATA==r) printf("No Data Found related to the query\n");
    
    while(1)
    {
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
        r=SQLGetData(hstmt, 2, SQL_C_CHAR, sql_address, MAX_ARR_LEN, &n);
        printf("%s",sql_name);
        for(int j=0;j<40-strlen(sql_name);++j)printf(" ");
        printf("%s\n",sql_address);
      }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      r=SQLFetch(hstmt);
    }
  }
  
  else if(i==5)
  {
    strcpy(sql,"SELECT Name \"Patient Name\",InsuranceID \"Patient Insurance ID\" "
"FROM Patient "
"WHERE SSN IN (SELECT Patient "
                    "FROM Stay "
                    "WHERE Room IN (SELECT Number "
                                    "FROM Room "
                                    "WHERE Type = 'ICU') AND DATEDIFF(`End`,Start)>15);");
            
    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    
    
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    r=SQLFetch(hstmt);
    if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO){
    printf("\nPatient Name");
    for(int j=0;j<25-strlen("Patient Name");++j)printf(" ");
    printf("Patient Inusrance ID\n");
    }
    else if(SQL_NO_DATA==r) printf("No Data Found related to the query\n");
    while(1)
    {
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
        r=SQLGetData(hstmt, 2, SQL_C_ULONG, &id, 0, &n);
        printf("%s",sql_name);
        for(int j=0;j<25-strlen(sql_name);++j)printf(" ");
        printf("%d\n",id);
      }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      r=SQLFetch(hstmt);
    }
  }
  
  else if(i==6)
  {
    strcpy(sql,"SELECT Name \"Nurse Name\" "
"FROM Nurse "
"WHERE EmployeeID IN (SELECT AssistingNurse "
                    "FROM Undergoes U "
                    "WHERE U.Procedure = (SELECT Code "
                                    "FROM Procedures "
                                    "WHERE Name = \"Bypass Surgery\"));");

    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    
    
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    r=SQLFetch(hstmt);
    if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
    printf("\nNurse Name \n");
    else if(SQL_NO_DATA==r) printf("No Data Found related to the query\n");
    while(1)
    {
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
       
        printf("%s\n",sql_name);
       }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      r=SQLFetch(hstmt);
    }
  }
  
  else if(i==7)
  {
    strcpy(sql,"SELECT N.Name 'Nurse Name',N.Position 'Nurse Position',P.Name 'Physician Name' "
"FROM Physician P,Nurse N "
"WHERE (P.EmployeeID,N.EmployeeID) IN (SELECT Physician,AssistingNurse "
                    "FROM Undergoes "
                    "WHERE Undergoes.Procedure = (SELECT Code "
                                    "FROM Procedures "
                                    "WHERE Name = \"Bypass Surgery\"));");

    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    
    
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    r=SQLFetch(hstmt);
    if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO){
    printf("\nNurse Name");
    for(int j=0;j<25-strlen("Nurse Name");++j)printf(" ");
    printf("Nurse Position");
    for(int j=0;j<25-strlen("Nurse Position");++j)printf(" ");
    printf("Physician Name\n");
    }
    else if(SQL_NO_DATA==r) printf("No Data Found related to the query\n");
    while(1)
    {
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
        r=SQLGetData(hstmt, 2, SQL_C_CHAR, sql_physician, MAX_ARR_LEN, &n);
        r=SQLGetData(hstmt, 3, SQL_C_CHAR, sql_address, MAX_ARR_LEN, &n);
        
        printf("%s",sql_name);
        for(int j=0;j<25-strlen(sql_name);++j)printf(" ");
        printf("%s",sql_physician);
        for(int j=0;j<25-strlen(sql_physician);++j)printf(" ");
        printf("%s\n",sql_address);
      }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      r=SQLFetch(hstmt);
    }
  }
  
  else if(i==8)
  {
    strcpy(sql,"SELECT Name \"Physician Name\" "
"FROM Physician "
"WHERE EmployeeID IN (SELECT Physician "
        "FROM Undergoes U "
        "WHERE (Physician,U.Procedure) NOT IN (SELECT Physician,Treatment "
                                    "FROM Trained_In));");

    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    
    
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    r=SQLFetch(hstmt);
    if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
    printf("\nPhysician Name \n");
    else if(SQL_NO_DATA==r) printf("No Data Found related to the query\n");
    while(1)
    {
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
       
        printf("%s\n",sql_name);
       }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      r=SQLFetch(hstmt);
    }
  }
  
  else if(i==9)
  {
    strcpy(sql,"SELECT P.Name \"Physician Name\" "
"FROM ((Undergoes U "
"INNER JOIN Trained_In T "
"ON U.Physician = T.Physician AND U.Procedure = T.Treatment AND DATEDIFF(U.Date,T.CertificationExpires)>0) "
"INNER JOIN Physician P ON U.Physician = P.EmployeeID);");

    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    
    
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    r=SQLFetch(hstmt);
    if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
    printf("\nPhysician Name \n");
    else if(SQL_NO_DATA==r) printf("No Data Found related to the query\n");
    while(1)
    {
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
       
        printf("%s\n",sql_name);
       }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      r=SQLFetch(hstmt);
    }
  }
  
  else if(i==10)
  {
    strcpy(sql,"SELECT P.Name \"Physician Name\",PR.Name \"Procedure Name\",U.Date \"Procedure Date\",PA.Name \"Patient Name\" "
"FROM ((((Undergoes U "
"INNER JOIN Trained_In T "
"ON U.Physician = T.Physician AND U.Procedure = T.Treatment AND DATEDIFF(U.Date,T.CertificationExpires)>0) "
"INNER JOIN Procedures PR ON U.Procedure = PR.Code) "
"INNER JOIN Patient PA ON U.Patient = PA.SSN) "
"INNER JOIN Physician P ON U.Physician = P.EmployeeID);");

    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    
    
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    r=SQLFetch(hstmt);
    if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO){
    printf("\nPhysician Name");
    for(int j=0;j<25-strlen("Physician Name");++j)printf(" ");
    printf("Procedure Name");
    for(int j=0;j<25-strlen("Procedure Name");++j)printf(" ");
    printf("Date");
    for(int j=0;j<38-strlen("Date");++j)printf(" ");
    printf("Patient Name\n");
    }
    else if(SQL_NO_DATA==r) printf("No Data Found related to the query\n");
    while(1)
    {
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
        r=SQLGetData(hstmt, 2, SQL_C_CHAR, sql_address, MAX_ARR_LEN, &n);
        r=SQLGetData(hstmt, 3, SQL_C_TYPE_TIMESTAMP,&sql_date_time,0,&n);
        r=SQLGetData(hstmt, 4, SQL_C_CHAR, sql_physician, MAX_ARR_LEN, &n);

        printf("%s",sql_name);
        for(int j=0;j<25-strlen(sql_name);++j)printf(" ");
        printf("%s",sql_address);
        for(int j=0;j<25-strlen(sql_address);++j)printf(" ");
        printf("%d-%d-%d 0%d:0%d:0%d",sql_date_time.year,sql_date_time.month,sql_date_time.day,sql_date_time.hour,sql_date_time.minute,sql_date_time.second);
        for(int j=0;j<20;++j)printf(" ");
        printf("%s\n",sql_physician);
       }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      r=SQLFetch(hstmt);
    }
  }
  
  else if(i==11)
  {
     strcpy(sql,"SELECT PA.Name \"Patient's Name\",P.Name \"Physician's Name\" "
"FROM (((SELECT Patient,Physician "
        "FROM Undergoes "
        "WHERE (Patient,Physician) IN (SELECT Patient,Physician "
                                        "FROM Prescribes "
                                        "WHERE (Patient,Physician) IN (SELECT Patient,Physician "
                                                                    "FROM Appointment "
                                                                    "WHERE Physician IN (SELECT Physician "
                                                                                        "FROM Affiliated_With "
                                                                                        "WHERE Department = (SELECT DepartmentID "
                                                                                                            "FROM Department "
                                                                                                            "WHERE Name = \"Cardiology\") AND Physician NOT IN (SELECT Head "
                                                                                                                                                            "FROM Department) "
                                                                                        ") "
                                                                    "GROUP BY Patient,Physician "
                                                                    "HAVING COUNT(*)>=2 "
                                                                    ") "
                                        "GROUP BY Patient,Physician "
                                        "HAVING COUNT(*)>=1 "
                                        ") AND `Procedure` IN (SELECT Code "
                                                            "FROM Procedures "
                                                            "WHERE Cost>5000) "
        "GROUP BY Patient,Physician "
        "HAVING COUNT(*)>=1) AS A "
"INNER JOIN Physician P ON A.Physician = P.EmployeeID) "
"INNER JOIN Patient PA ON A.Patient = PA.SSN);");

    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    
    
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    r=SQLFetch(hstmt);
    if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO){
    printf("\nPatient Name");
    for(int j=0;j<25-strlen("Patient Name");++j)printf(" ");
    printf("Physician Name\n");
    }
    else if(SQL_NO_DATA==r) printf("No Data Found related to the query\n");
    while(1)
    {
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
        r=SQLGetData(hstmt, 2, SQL_C_CHAR, sql_address, MAX_ARR_LEN, &n);
       
        printf("%s",sql_name);
        for(int j=0;j<25-strlen(sql_name);++j)printf(" ");
        printf("%s\n",sql_address);
       }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      r=SQLFetch(hstmt);
    }
  }
  
  else if(i==12)
  {
    strcpy(sql,"SELECT Name \"Medication Name\",Brand \"Medication Brand\" "
"FROM Medication "
"WHERE Code = (SELECT Medication "
                "FROM Prescribes "
                "GROUP BY Medication "
                "ORDER BY COUNT(*) DESC "
                "LIMIT 1);");
    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    
    
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    r=SQLFetch(hstmt);
    if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO){
    printf("\nMedicine Name");
    for(int j=0;j<25-strlen("Medicine Name");++j)printf(" ");
    printf("Medicine Brand\n");
    }
    else if(SQL_NO_DATA==r) printf("No Data Found related to the query\n");
    while(1)
    {
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
        r=SQLGetData(hstmt, 2, SQL_C_CHAR, sql_address, MAX_ARR_LEN, &n);
       
        printf("%s",sql_name);
        for(int j=0;j<25-strlen(sql_name);++j)printf(" ");
        printf("%s\n",sql_address);
       }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      r=SQLFetch(hstmt);
    }
  }
  
  else if(i==13)
  {
    char input[1000];
    printf("\nEnter a Procedure: ");
    fgets(input,1000,stdin);
    input[strlen(input)-1]='\0';
    strcpy(sql,"SELECT Name \"Physician Name\" "
                "FROM Physician "
                "WHERE EmployeeID IN (SELECT Physician "
                                     "FROM Trained_In "
                                     "WHERE Treatment = (SELECT Code "
                                                        "FROM Procedures "
                                                        "WHERE Name = '");
    strcat(sql,input);
    strcat(sql,"'));");

    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    r=SQLFetch(hstmt);
    if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
    printf("\nPhysician Name\n");
    else if(SQL_NO_DATA==r) printf("No Data Found related to the query\n");
    while(1)
    {
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
        printf("%s\n",sql_name);
      }
      else if(SQL_NO_DATA==r){
        break;
      } 
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      r=SQLFetch(hstmt);
    }
   }
  
  SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
}
int main(void)
{ 
  ODBCConnectDB("PRANAV_MYSQL", "20CS10085", "20CS10085");
  db_fetch();
  ODBCDisconnectDB();
}