#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>

#define MAX_ARR_LEN 200

SQLHENV  sql_henv=NULL;
SQLHDBC  sql_hdbc=NULL;

int ODBCConnectDB(SQLCHAR* ds, SQLCHAR* user, SQLCHAR* pw)
{
  SQLRETURN  sql_return_val;

  sql_hdbc=NULL;
  sql_henv=NULL;

  // Allocate environment handle
  sql_return_val=SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sql_henv);  
  if (sql_return_val==SQL_SUCCESS || sql_return_val==SQL_SUCCESS_WITH_INFO) 
  {
    //  Set the ODBC version environment attribute
    sql_return_val=SQLSetEnvAttr(sql_henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0); 

    if (sql_return_val==SQL_SUCCESS || sql_return_val==SQL_SUCCESS_WITH_INFO) 
    {
      // Allocate connection handle 
      sql_return_val=SQLAllocHandle(SQL_HANDLE_DBC, sql_henv, &sql_hdbc); 

      if (sql_return_val==SQL_SUCCESS || sql_return_val==SQL_SUCCESS_WITH_INFO) 
      {
        // Set login timeout to 5 seconds. 
        SQLSetConnectAttr(sql_hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

        // Connect to data source
        sql_return_val=SQLConnect(sql_hdbc, ds, SQL_NTS, user, SQL_NTS, pw, SQL_NTS);
	
        if (sql_return_val==SQL_SUCCESS || sql_return_val==SQL_SUCCESS_WITH_INFO)
        {
          return 1;
        }
        SQLDisconnect(sql_hdbc);
      }
      SQLFreeHandle(SQL_HANDLE_DBC, sql_hdbc);
    }
    else(printf("Failure in connecting\n"));
  }
  SQLFreeHandle(SQL_HANDLE_ENV, sql_henv);

  sql_hdbc=NULL;
  sql_henv=NULL;

  return 0;
}
int ODBCDisconnectDB()
{
  if(sql_hdbc) 
  {
    SQLDisconnect(sql_hdbc);	
    SQLFreeHandle(SQL_HANDLE_DBC, sql_hdbc);
  }
  if(sql_henv) SQLFreeHandle(SQL_HANDLE_ENV, sql_henv);
  sql_hdbc=NULL;
  sql_henv=NULL;
  return 1;
}
int db_exec_stmt(char* stmt_str)
{
  SQLHSTMT sql_hstmt;
  SQLRETURN  sql_return_val;

  if(sql_henv==NULL || sql_hdbc==NULL) return 0;

  /* Allocate statement handle */
  sql_return_val = SQLAllocHandle(SQL_HANDLE_STMT, sql_hdbc, &sql_hstmt); 
  if(!(sql_return_val==SQL_SUCCESS || sql_return_val==SQL_SUCCESS_WITH_INFO) ) return 0;
	
  sql_return_val = SQLExecDirect(sql_hstmt, stmt_str, SQL_NTS);
  SQLFreeHandle(SQL_HANDLE_STMT, sql_hstmt);
  if(sql_return_val==SQL_SUCCESS || sql_return_val==SQL_SUCCESS_WITH_INFO) return 1;
  if(sql_return_val==SQL_NO_DATA) return 1;

  printf("sqlerr: %d\n", sql_return_val);
  return 0;
}
int db_fetch()
{
  SQLRETURN sql_return_val;
  SQLHSTMT sql_hstmt;
  SQLCHAR sql_query[4096];
  SQLLEN n;
  SQLINTEGER sql_num;
  SQLCHAR sql_name[MAX_ARR_LEN];
  SQLCHAR sql_address[MAX_ARR_LEN];
  SQLCHAR sql_physician[MAX_ARR_LEN];
  SQL_TIMESTAMP_STRUCT sql_date_time;

  if(sql_hdbc==NULL || sql_henv==NULL) return 0;

  sql_return_val=SQLAllocHandle(SQL_HANDLE_STMT, sql_hdbc, &sql_hstmt);
  if(!(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO)) return 0;

  int i;
  printf("Enter the query number: ");
  scanf("%d", &i);
  if(i==1)
  {
    strcpy(sql_query,"SELECT Name \"Physician Name\" "
                "FROM Physician "
                "WHERE EmployeeID IN (SELECT Physician "
                                     "FROM Trained_In "
                                     "WHERE Treatment = (SELECT Code "
                                                        "FROM Procedures "
                                                        "WHERE Name = \"Bypass Surgery\"));");
    // strcpy(sql_query, query);
    sql_return_val=SQLExecDirect(sql_hstmt, sql_query, SQL_NTS);
    if(sql_return_val!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql_query);
      SQLFreeHandle(SQL_HANDLE_STMT, sql_hstmt);
      return 0;
    }

    sql_return_val=SQLFetch(sql_hstmt);
    if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO)
      printf("\nPhysician Name\n");
    else if(SQL_NO_DATA==sql_return_val) printf("No Data Found related to the query\n");
    while(1)
    {
      if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO)
      {
        sql_return_val=SQLGetData(sql_hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
        printf("%s\n",sql_name);
      }
      else if(SQL_NO_DATA==sql_return_val) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      sql_return_val=SQLFetch(sql_hstmt);
    }
  }
  
  else if(i==2)
  {
    strcpy(sql_query,"SELECT Name \"Physician Name\" "
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
    sql_return_val=SQLExecDirect(sql_hstmt, sql_query, SQL_NTS);
    if(sql_return_val!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql_query);
      SQLFreeHandle(SQL_HANDLE_STMT, sql_hstmt);
      return 0;
    }

    sql_return_val=SQLFetch(sql_hstmt);
    if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO)
    printf("\nPhysician Name\n");
    else if(SQL_NO_DATA==sql_return_val) printf("No Data Found related to the query\n");
    while(1)
    {
      if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO)
      {
        sql_return_val=SQLGetData(sql_hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
        printf("%s\n",sql_name);
      }
      else if(SQL_NO_DATA==sql_return_val) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      sql_return_val=SQLFetch(sql_hstmt);
    }
  }
  
  else if(i==3)
  {
    strcpy(sql_query,"SELECT Name \"Nurse Name\" "
"FROM Nurse "
"WHERE EmployeeID IN (SELECT Nurse "
                    "FROM On_Call "
                    "WHERE (BlockFloor,BlockCode) IN (SELECT BlockFloor,BlockCode "
                                                    "FROM Room "
                                                    "WHERE Number = 123));"); 
    
    sql_return_val=SQLExecDirect(sql_hstmt, sql_query, SQL_NTS);
    if(sql_return_val!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql_query);
      SQLFreeHandle(SQL_HANDLE_STMT, sql_hstmt);
      return 0;
    }

    sql_return_val=SQLFetch(sql_hstmt);
    if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO)
    printf("\nNurse Name\n");
    else if(SQL_NO_DATA==sql_return_val) printf("No Data Found related to the query\n");
    while(1)
    {
      if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO)
      {
        sql_return_val=SQLGetData(sql_hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
        printf("%s\n",sql_name);
      }
      else if(SQL_NO_DATA==sql_return_val) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      sql_return_val=SQLFetch(sql_hstmt);
    }
  }
  

  else if(i==4)
  { 
    strcpy(sql_query,"SELECT Name \"Patient Name\",Address \"Patient Address\" "
"FROM Patient "
"WHERE SSN IN (SELECT Patient "
                    "FROM Prescribes "
                    "WHERE Medication IN (SELECT Code "
                                    "FROM Medication "
                                    "WHERE Name = 'Remdesivir'));");
    sql_return_val=SQLExecDirect(sql_hstmt, sql_query, SQL_NTS);
    
    
    if(sql_return_val!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql_query);
      SQLFreeHandle(SQL_HANDLE_STMT, sql_hstmt);
      return 0;
    }

    sql_return_val=SQLFetch(sql_hstmt);
    if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO){
    printf("\nPatient Name");
    for(int j=0;j<40-strlen("Patient Name");++j)printf(" ");
    printf("Patient Address\n");
    }
    else if(SQL_NO_DATA==sql_return_val) printf("No Data Found related to the query\n");
    
    while(1)
    {
      if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO)
      {
        sql_return_val=SQLGetData(sql_hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
        sql_return_val=SQLGetData(sql_hstmt, 2, SQL_C_CHAR, sql_address, MAX_ARR_LEN, &n);
        printf("%s",sql_name);
        for(int j=0;j<40-strlen(sql_name);++j)printf(" ");
        printf("%s\n",sql_address);
      }
      else if(SQL_NO_DATA==sql_return_val) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      sql_return_val=SQLFetch(sql_hstmt);
    }
  }
  
  else if(i==5)
  {
    strcpy(sql_query,"SELECT Name \"Patient Name\",InsuranceID \"Patient Insurance ID\" "
"FROM Patient "
"WHERE SSN IN (SELECT Patient "
                    "FROM Stay "
                    "WHERE Room IN (SELECT Number "
                                    "FROM Room "
                                    "WHERE Type = 'ICU') AND DATEDIFF(`End`,Start)>15);");
            
    sql_return_val=SQLExecDirect(sql_hstmt, sql_query, SQL_NTS);
    
    
    if(sql_return_val!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql_query);
      SQLFreeHandle(SQL_HANDLE_STMT, sql_hstmt);
      return 0;
    }

    sql_return_val=SQLFetch(sql_hstmt);
    if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO){
    printf("\nPatient Name");
    for(int j=0;j<25-strlen("Patient Name");++j)printf(" ");
    printf("Patient Inusrance ID\n");
    }
    else if(SQL_NO_DATA==sql_return_val) printf("No Data Found related to the query\n");
    while(1)
    {
      if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO)
      {
        sql_return_val=SQLGetData(sql_hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
        sql_return_val=SQLGetData(sql_hstmt, 2, SQL_C_ULONG, &sql_num, 0, &n);
        printf("%s",sql_name);
        for(int j=0;j<25-strlen(sql_name);++j)printf(" ");
        printf("%d\n",sql_num);
      }
      else if(SQL_NO_DATA==sql_return_val) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      sql_return_val=SQLFetch(sql_hstmt);
    }
  }
  
  else if(i==6)
  {
    strcpy(sql_query,"SELECT Name \"Nurse Name\" "
"FROM Nurse "
"WHERE EmployeeID IN (SELECT AssistingNurse "
                    "FROM Undergoes U "
                    "WHERE U.Procedure = (SELECT Code "
                                    "FROM Procedures "
                                    "WHERE Name = \"Bypass Surgery\"));");

    sql_return_val=SQLExecDirect(sql_hstmt, sql_query, SQL_NTS);
    
    
    if(sql_return_val!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql_query);
      SQLFreeHandle(SQL_HANDLE_STMT, sql_hstmt);
      return 0;
    }

    sql_return_val=SQLFetch(sql_hstmt);
    if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO)
    printf("\nNurse Name \n");
    else if(SQL_NO_DATA==sql_return_val) printf("No Data Found related to the query\n");
    while(1)
    {
      if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO)
      {
        sql_return_val=SQLGetData(sql_hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
       
        printf("%s\n",sql_name);
       }
      else if(SQL_NO_DATA==sql_return_val) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      sql_return_val=SQLFetch(sql_hstmt);
    }
  }
  
  else if(i==7)
  {
    strcpy(sql_query,"SELECT N.Name 'Nurse Name',N.Position 'Nurse Position',P.Name 'Physician Name' "
"FROM Physician P,Nurse N "
"WHERE (P.EmployeeID,N.EmployeeID) IN (SELECT Physician,AssistingNurse "
                    "FROM Undergoes "
                    "WHERE Undergoes.Procedure = (SELECT Code "
                                    "FROM Procedures "
                                    "WHERE Name = \"Bypass Surgery\"));");

    sql_return_val=SQLExecDirect(sql_hstmt, sql_query, SQL_NTS);
    
    
    if(sql_return_val!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql_query);
      SQLFreeHandle(SQL_HANDLE_STMT, sql_hstmt);
      return 0;
    }

    sql_return_val=SQLFetch(sql_hstmt);
    if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO){
    printf("\nNurse Name");
    for(int j=0;j<25-strlen("Nurse Name");++j)printf(" ");
    printf("Nurse Position");
    for(int j=0;j<25-strlen("Nurse Position");++j)printf(" ");
    printf("Physician Name\n");
    }
    else if(SQL_NO_DATA==sql_return_val) printf("No Data Found related to the query\n");
    while(1)
    {
      if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO)
      {
        sql_return_val=SQLGetData(sql_hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
        sql_return_val=SQLGetData(sql_hstmt, 2, SQL_C_CHAR, sql_physician, MAX_ARR_LEN, &n);
        sql_return_val=SQLGetData(sql_hstmt, 3, SQL_C_CHAR, sql_address, MAX_ARR_LEN, &n);
        
        printf("%s",sql_name);
        for(int j=0;j<25-strlen(sql_name);++j)printf(" ");
        printf("%s",sql_physician);
        for(int j=0;j<25-strlen(sql_physician);++j)printf(" ");
        printf("%s\n",sql_address);
      }
      else if(SQL_NO_DATA==sql_return_val) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      sql_return_val=SQLFetch(sql_hstmt);
    }
  }
  
  else if(i==8)
  {
    strcpy(sql_query,"SELECT Name \"Physician Name\" "
"FROM Physician "
"WHERE EmployeeID IN (SELECT Physician "
        "FROM Undergoes U "
        "WHERE (Physician,U.Procedure) NOT IN (SELECT Physician,Treatment "
                                    "FROM Trained_In));");

    sql_return_val=SQLExecDirect(sql_hstmt, sql_query, SQL_NTS);
    
    
    if(sql_return_val!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql_query);
      SQLFreeHandle(SQL_HANDLE_STMT, sql_hstmt);
      return 0;
    }

    sql_return_val=SQLFetch(sql_hstmt);
    if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO)
    printf("\nPhysician Name \n");
    else if(SQL_NO_DATA==sql_return_val) printf("No Data Found related to the query\n");
    while(1)
    {
      if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO)
      {
        sql_return_val=SQLGetData(sql_hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
       
        printf("%s\n",sql_name);
       }
      else if(SQL_NO_DATA==sql_return_val) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      sql_return_val=SQLFetch(sql_hstmt);
    }
  }
  
  else if(i==9)
  {
    strcpy(sql_query,"SELECT P.Name \"Physician Name\" "
"FROM ((Undergoes U "
"INNER JOIN Trained_In T "
"ON U.Physician = T.Physician AND U.Procedure = T.Treatment AND DATEDIFF(U.Date,T.CertificationExpires)>0) "
"INNER JOIN Physician P ON U.Physician = P.EmployeeID);");

    sql_return_val=SQLExecDirect(sql_hstmt, sql_query, SQL_NTS);
    
    
    if(sql_return_val!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql_query);
      SQLFreeHandle(SQL_HANDLE_STMT, sql_hstmt);
      return 0;
    }

    sql_return_val=SQLFetch(sql_hstmt);
    if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO)
    printf("\nPhysician Name \n");
    else if(SQL_NO_DATA==sql_return_val) printf("No Data Found related to the query\n");
    while(1)
    {
      if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO)
      {
        sql_return_val=SQLGetData(sql_hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
       
        printf("%s\n",sql_name);
       }
      else if(SQL_NO_DATA==sql_return_val) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      sql_return_val=SQLFetch(sql_hstmt);
    }
  }
  
  else if(i==10)
  {
    strcpy(sql_query,"SELECT P.Name \"Physician Name\",PR.Name \"Procedure Name\",U.Date \"Procedure Date\",PA.Name \"Patient Name\" "
"FROM ((((Undergoes U "
"INNER JOIN Trained_In T "
"ON U.Physician = T.Physician AND U.Procedure = T.Treatment AND DATEDIFF(U.Date,T.CertificationExpires)>0) "
"INNER JOIN Procedures PR ON U.Procedure = PR.Code) "
"INNER JOIN Patient PA ON U.Patient = PA.SSN) "
"INNER JOIN Physician P ON U.Physician = P.EmployeeID);");

    sql_return_val=SQLExecDirect(sql_hstmt, sql_query, SQL_NTS);
    
    
    if(sql_return_val!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql_query);
      SQLFreeHandle(SQL_HANDLE_STMT, sql_hstmt);
      return 0;
    }

    sql_return_val=SQLFetch(sql_hstmt);
    if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO){
    printf("\nPhysician Name");
    for(int j=0;j<25-strlen("Physician Name");++j)printf(" ");
    printf("Procedure Name");
    for(int j=0;j<25-strlen("Procedure Name");++j)printf(" ");
    printf("Date");
    for(int j=0;j<38-strlen("Date");++j)printf(" ");
    printf("Patient Name\n");
    }
    else if(SQL_NO_DATA==sql_return_val) printf("No Data Found related to the query\n");
    while(1)
    {
      if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO)
      {
        sql_return_val=SQLGetData(sql_hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
        sql_return_val=SQLGetData(sql_hstmt, 2, SQL_C_CHAR, sql_address, MAX_ARR_LEN, &n);
        sql_return_val=SQLGetData(sql_hstmt, 3, SQL_C_TYPE_TIMESTAMP,&sql_date_time,0,&n);
        sql_return_val=SQLGetData(sql_hstmt, 4, SQL_C_CHAR, sql_physician, MAX_ARR_LEN, &n);

        printf("%s",sql_name);
        for(int j=0;j<25-strlen(sql_name);++j)printf(" ");
        printf("%s",sql_address);
        for(int j=0;j<25-strlen(sql_address);++j)printf(" ");
        printf("%d-%d-%d 0%d:0%d:0%d",sql_date_time.year,sql_date_time.month,sql_date_time.day,sql_date_time.hour,sql_date_time.minute,sql_date_time.second);
        for(int j=0;j<20;++j)printf(" ");
        printf("%s\n",sql_physician);
       }
      else if(SQL_NO_DATA==sql_return_val) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      sql_return_val=SQLFetch(sql_hstmt);
    }
  }
  
  else if(i==11)
  {
     strcpy(sql_query,"SELECT PA.Name \"Patient's Name\",P.Name \"Physician's Name\" "
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

    sql_return_val=SQLExecDirect(sql_hstmt, sql_query, SQL_NTS);
    
    
    if(sql_return_val!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql_query);
      SQLFreeHandle(SQL_HANDLE_STMT, sql_hstmt);
      return 0;
    }

    sql_return_val=SQLFetch(sql_hstmt);
    if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO){
    printf("\nPatient Name");
    for(int j=0;j<25-strlen("Patient Name");++j)printf(" ");
    printf("Physician Name\n");
    }
    else if(SQL_NO_DATA==sql_return_val) printf("No Data Found related to the query\n");
    while(1)
    {
      if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO)
      {
        sql_return_val=SQLGetData(sql_hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
        sql_return_val=SQLGetData(sql_hstmt, 2, SQL_C_CHAR, sql_address, MAX_ARR_LEN, &n);
       
        printf("%s",sql_name);
        for(int j=0;j<25-strlen(sql_name);++j)printf(" ");
        printf("%s\n",sql_address);
       }
      else if(SQL_NO_DATA==sql_return_val) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      sql_return_val=SQLFetch(sql_hstmt);
    }
  }
  
  else if(i==12)
  {
    strcpy(sql_query,"SELECT Name \"Medication Name\",Brand \"Medication Brand\" "
"FROM Medication "
"WHERE Code = (SELECT Medication "
                "FROM Prescribes "
                "GROUP BY Medication "
                "ORDER BY COUNT(*) DESC "
                "LIMIT 1);");
    sql_return_val=SQLExecDirect(sql_hstmt, sql_query, SQL_NTS);
    
    
    if(sql_return_val!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql_query);
      SQLFreeHandle(SQL_HANDLE_STMT, sql_hstmt);
      return 0;
    }

    sql_return_val=SQLFetch(sql_hstmt);
    if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO){
    printf("\nMedicine Name");
    for(int j=0;j<25-strlen("Medicine Name");++j)printf(" ");
    printf("Medicine Brand\n");
    }
    else if(SQL_NO_DATA==sql_return_val) printf("No Data Found related to the query\n");
    while(1)
    {
      if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO)
      {
        sql_return_val=SQLGetData(sql_hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
        sql_return_val=SQLGetData(sql_hstmt, 2, SQL_C_CHAR, sql_address, MAX_ARR_LEN, &n);
       
        printf("%s",sql_name);
        for(int j=0;j<25-strlen(sql_name);++j)printf(" ");
        printf("%s\n",sql_address);
       }
      else if(SQL_NO_DATA==sql_return_val) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      sql_return_val=SQLFetch(sql_hstmt);
    }
  }
  
  else if(i==13)
  {
    char input[1000];
    printf("\nEnter a Procedure: ");
    fflush(stdin);
    fgets(input,1000,stdin);
    input[strlen(input)-1]='\0';
    strcpy(sql_query,"SELECT Name \"Physician Name\" "
                "FROM Physician "
                "WHERE EmployeeID IN (SELECT Physician "
                                     "FROM Trained_In "
                                     "WHERE Treatment = (SELECT Code "
                                                        "FROM Procedures "
                                                        "WHERE Name = '");
    strcat(sql_query,input);
    strcat(sql_query,"'));");

    sql_return_val=SQLExecDirect(sql_hstmt, sql_query, SQL_NTS);
    if(sql_return_val!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql_query);
      SQLFreeHandle(SQL_HANDLE_STMT, sql_hstmt);
      return 0;
    }

    sql_return_val=SQLFetch(sql_hstmt);
    if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO)
    printf("\nPhysician Name\n");
    else if(SQL_NO_DATA==sql_return_val) printf("No Data Found related to the query\n");
    while(1)
    {
      if(sql_return_val==SQL_SUCCESS||sql_return_val==SQL_SUCCESS_WITH_INFO)
      {
        sql_return_val=SQLGetData(sql_hstmt, 1, SQL_C_CHAR, sql_name, MAX_ARR_LEN, &n);
        printf("%s\n",sql_name);
      }
      else if(SQL_NO_DATA==sql_return_val){
        break;
      } 
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
      sql_return_val=SQLFetch(sql_hstmt);
    }
   }
   else printf("Invalid Query Number\n");
  
  SQLFreeHandle(SQL_HANDLE_STMT, sql_hstmt);
}
int main(void)
{ 
  ODBCConnectDB("PRANAV_MYSQL", "20CS10085", "20CS10085");
  db_fetch();
  ODBCDisconnectDB();
}