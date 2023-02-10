#include <stdio.h>
#include<windows.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>

SQLHENV  env_handle=NULL;
SQLHDBC  ODBC_handle=NULL;

int DATABASE_CONNECT(SQLCHAR* DSN, SQLCHAR* USER, SQLCHAR* PASSWORD)
{
  SQLRETURN  mysql_ret;

  ODBC_handle=NULL;
  env_handle=NULL;

  //assign a connection handle to the environment
  //SQL_NULL_HANDLE:  no parent handle is being used
  //henv: store the allocated handle
  mysql_ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env_handle);  
  
  //on success of above function
  if (mysql_ret ==SQL_SUCCESS || mysql_ret ==SQL_SUCCESS_WITH_INFO) 
  {
    //control environment attributes in accordance to ODBC
    //SQL_ATTR_ODBC_VERSION: specifies the ODBC version.
    //SQL_OV_ODBC3: Stores ODBC version
    mysql_ret = SQLSetEnvAttr(env_handle, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0); 

    //on success of above function
    if (mysql_ret ==SQL_SUCCESS || mysql_ret ==SQL_SUCCESS_WITH_INFO) 
    {
      // Assign ODBC connection handle 
      mysql_ret = SQLAllocHandle(SQL_HANDLE_DBC, env_handle, &ODBC_handle); 

      //on success of above function
      if (mysql_ret ==SQL_SUCCESS || mysql_ret ==SQL_SUCCESS_WITH_INFO) 
      {
        //Try logging in for 10 seconds
        SQLSetConnectAttr(ODBC_handle, SQL_LOGIN_TIMEOUT, (SQLPOINTER)10, 0);

        // Connect to DATA source that actually links driver to database
        //SQL_NTS indicates prev variable is in characters not bytes.
        //USER and PASSWORD: null-terminated strings for username and password
        mysql_ret = SQLConnect(ODBC_handle, DSN, SQL_NTS, USER, SQL_NTS, PASSWORD, SQL_NTS);

        //on success of above function
        if (mysql_ret ==SQL_SUCCESS || mysql_ret ==SQL_SUCCESS_WITH_INFO)
        {
          return 1;
        }
        
      }
      
    }
    else(printf("Coudn't Connect to DataBase\n"));
  }
  return 0;
}

int DATABASE_DISCONNECT()
{
  //free all alloted handles
  if(ODBC_handle) 
  {
    SQLDisconnect(ODBC_handle);	
    SQLFreeHandle(SQL_HANDLE_DBC, ODBC_handle);
  }
  if(env_handle) SQLFreeHandle(SQL_HANDLE_ENV, env_handle);

  //reset the values of macros
  ODBC_handle=NULL;
  env_handle=NULL;
  return 1;
}

int EXECUTE_MYSQL_QUERY()
{
  //to execute mysql query
  SQLRETURN query_ret;

  //handle  to identify and manage the execution of an SQL statement. 
  SQLHSTMT MYSQL_QUERY;
  
  //variables to store data retreived from MYSQL
  SQLCHAR sql[1000];
  SQLLEN n;
  SQLINTEGER int_var;
  SQLCHAR string_var_1[100];
  SQLCHAR string_var_2[100];
  SQLCHAR string_var_3[100];
  SQL_TIMESTAMP_STRUCT date_time;

  //check if ODBC handles are available
  if(ODBC_handle==NULL || env_handle==NULL) return 0;
  query_ret =SQLAllocHandle(SQL_HANDLE_STMT, ODBC_handle, &MYSQL_QUERY);
  if(!(query_ret ==SQL_SUCCESS||query_ret ==SQL_SUCCESS_WITH_INFO)) return 0;

  int i;
  printf("\nEnter a number: ");
  scanf("%d", &i);
  if(i==-1){
    printf("\nExiting......\n");
    return -1;
  }
  if(i==1)
  {
    strcpy(sql,"SELECT Physician.Name FROM Physician INNER JOIN Trained_in t ON t.Physician = Physician.EmployeeID INNER JOIN Procedures pr ON pr.Code = t.Treatment and pr.Name = 'Bypass Surgery';");
    
    //access relational database using the query
    query_ret =SQLExecDirect(MYSQL_QUERY, sql, SQL_NTS);//execute the query and store the data in mysql_query
    if(query_ret !=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, MYSQL_QUERY);
      return 0;
    }

    //fetch the data stored in mysql-query set in the above function
    query_ret=SQLFetch(MYSQL_QUERY);
    if(query_ret==SQL_SUCCESS||query_ret==SQL_SUCCESS_WITH_INFO)//indicating that MYSQL has data corresponding to the query
      {
        printf("---------------------------------------");
        printf("\nPhysician Name\n");
        printf("---------------------------------------\n");
      }
    else if(SQL_NO_DATA==query_ret) printf("MYSQL Query returned Empty Set!\n");//mysql query has returned empty dataset//empty set returned from mysql query
    

    while(1)
    {
      
      if(query_ret ==SQL_SUCCESS||query_ret ==SQL_SUCCESS_WITH_INFO)
      {
        //extract the first column of the data for one row and store it in string_var_1
        query_ret =SQLGetData(MYSQL_QUERY, 1, SQL_C_CHAR, string_var_1, 20, &n);
        printf("%s\n",string_var_1);
      }
      //no more data left to be fetched
      else if(SQL_NO_DATA== query_ret ) break;
      else
      {
        //error in sql SQLFetch
        printf("%s\n", "fail to fetch data");
        break;
      }
      //fetch another row 
      query_ret=SQLFetch(MYSQL_QUERY);
    }
    printf("---------------------------------------\n");
  }
  
  else if(i==2)
  {
    char query[10000] = "SELECT p.Name FROM Department " 
    "INNER JOIN Affiliated_with a ON a.Department = Department.DepartmentID and Department.Name = \'Cardiology\' "
    "INNER JOIN Physician p ON p.EmployeeID=a.Physician "
    "INNER JOIN Trained_in t ON t.Physician = p.EmployeeID "
    "INNER JOIN Procedures pr ON pr.Code = t.Treatment and pr.Name = \'Bypass Surgery\';";
    
    strcpy(sql, query);
    query_ret =SQLExecDirect(MYSQL_QUERY, sql, SQL_NTS);//execute the query and store the data in mysql_query
    if(query_ret !=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, MYSQL_QUERY);
      return 0;
    }
    query_ret=SQLFetch(MYSQL_QUERY);//fetch the data
    if(query_ret==SQL_SUCCESS||query_ret==SQL_SUCCESS_WITH_INFO)
    {
      printf("---------------------------------------");
      printf("\nPhysician Name\n");
      printf("---------------------------------------\n");
    }
    else if(SQL_NO_DATA==query_ret) printf("MYSQL Query returned Empty Set!\n");//mysql query has returned empty dataset
    
    while(1)
    {
      
      if(query_ret ==SQL_SUCCESS||query_ret ==SQL_SUCCESS_WITH_INFO)
      {
        query_ret =SQLGetData(MYSQL_QUERY, 1, SQL_C_CHAR, string_var_1, 20, &n);
        printf("%s\n",string_var_1);
      }
      else if(SQL_NO_DATA== query_ret ) break;
      else
      {
        //error in sql SQLFetch
        printf("%s\n", "fail to fetch data");
        break;
      }//fetch another row of data
      query_ret=SQLFetch(MYSQL_QUERY);
    }
    printf("---------------------------------------\n");
  }
  
  else if(i==3)
  {
    char query[10000] = "SELECT Nurse.Name "
            "FROM  Nurse "
            "INNER JOIN On_Call n ON n.Nurse = Nurse.EmployeeID "
            "INNER JOIN Room ON Room.BlockCode = n.BlockCode and Room.BlockFloor = n.BlockFloor and Room.Number = 123;"; 
    
    strcpy(sql, query);
    
    query_ret =SQLExecDirect(MYSQL_QUERY, sql, SQL_NTS);//execute the query and store the data in mysql_query
    if(query_ret !=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, MYSQL_QUERY);
      return 0;
    }
    query_ret=SQLFetch(MYSQL_QUERY);//fetch the data
    if(query_ret==SQL_SUCCESS||query_ret==SQL_SUCCESS_WITH_INFO)
    {
      printf("---------------------------------------");
      printf("\nNurse Name\n");
      printf("---------------------------------------\n");
    }
    else if(SQL_NO_DATA==query_ret) printf("MYSQL Query returned Empty Set!\n");//mysql query has returned empty dataset
    
    while(1)
    {
      
      if(query_ret ==SQL_SUCCESS||query_ret ==SQL_SUCCESS_WITH_INFO)
      {
        query_ret =SQLGetData(MYSQL_QUERY, 1, SQL_C_CHAR, string_var_1, 20, &n);
        printf("%s\n",string_var_1);
      }
      else if(SQL_NO_DATA== query_ret ) break;
      else
      {
        //error in sql SQLFetch
        printf("%s\n", "fail to fetch data");
        break;
      }//fetch another row of data
      query_ret=SQLFetch(MYSQL_QUERY);
    }
    printf("---------------------------------------\n");
  }

  else if(i==4)
  {
    
    char query[10000] ="SELECT pa.name, pa.address "
            "FROM Medication "
            "INNER JOIN Prescribes p ON p.Medication = Medication.Code and Medication.Name='Remdesivir' "
            "INNER JOIN Patient pa ON pa.SSN = p.Patient;";
    strcpy(sql, query);
    query_ret =SQLExecDirect(MYSQL_QUERY, sql, SQL_NTS);//execute the query and store the data in mysql_query
    
    
    if(query_ret !=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, MYSQL_QUERY);
      return 0;
    }
    query_ret=SQLFetch(MYSQL_QUERY);//fetch the data
    if(query_ret==SQL_SUCCESS||query_ret==SQL_SUCCESS_WITH_INFO)
    {//if the query hasn't returned empty set
      printf("----------------------------------------------------------------------------------------");
      printf("\nPatient Name"); 
      for(int j=0;j<40-strlen("Patient Name");++j)printf(" ");
      printf("Patient address\n");
      printf("----------------------------------------------------------------------------------------\n");
    }
    else if(SQL_NO_DATA==query_ret) printf("MYSQL Query returned Empty Set!\n");//mysql query has returned empty dataset
    
    while(1)
    {
      
      if(query_ret ==SQL_SUCCESS||query_ret ==SQL_SUCCESS_WITH_INFO)
      {
        query_ret =SQLGetData(MYSQL_QUERY, 1, SQL_C_CHAR, string_var_1, 100, &n);
        query_ret =SQLGetData(MYSQL_QUERY, 2, SQL_C_CHAR, string_var_2, 100, &n);
        printf("%s",string_var_1);
        for(int j=0;j<40-strlen(string_var_1);++j)printf(" ");
        printf("%s\n",string_var_2);
      }
      else if(SQL_NO_DATA== query_ret ) break;
      else
      {
        //error in sql SQLFetch
        printf("%s\n", "fail to fetch data");
        break;
      }//fetch another row of data
      query_ret=SQLFetch(MYSQL_QUERY);
    }
    printf("----------------------------------------------------------------------------------------\n");
  }
  
  else if(i==5)
  {
    
    char query[10000] ="SELECT p.name, p.InsuranceID "
            "FROM Room "
            "INNER JOIN Stay s ON s.Room = Room.Number and Room.Type = \"ICU\" "
            "INNER JOIN Patient p ON p.SSN = s.Patient and  DATEDIFF(s.End, s.Start)>15;";
            
    strcpy(sql, query);
    query_ret =SQLExecDirect(MYSQL_QUERY, sql, SQL_NTS);//execute the query and store the data in mysql_query
    
    
    if(query_ret !=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, MYSQL_QUERY);
      return 0;
    }
    query_ret=SQLFetch(MYSQL_QUERY);//fetch the data
    if(query_ret==SQL_SUCCESS||query_ret==SQL_SUCCESS_WITH_INFO)
    {//if the query hasn't returned empty set
      printf("------------------------------------------------------------\n");
      printf("\nPatient Name");
      for(int j=0;j<40-strlen("Patient Name");++j)printf(" ");
      printf("Patient Inusrance ID\n");
      printf("------------------------------------------------------------\n");
    }
    else if(SQL_NO_DATA==query_ret) printf("MYSQL Query returned Empty Set!\n");//mysql query has returned empty dataset
    
    while(1)
    {
      
      if(query_ret ==SQL_SUCCESS||query_ret ==SQL_SUCCESS_WITH_INFO)
      {
        query_ret =SQLGetData(MYSQL_QUERY, 1, SQL_C_CHAR, string_var_1, 100, &n);
        query_ret =SQLGetData(MYSQL_QUERY, 2, SQL_C_ULONG, &int_var, 0, &n);
        printf("%s",string_var_1);
        for(int j=0;j<40-strlen(string_var_1);++j)printf(" ");
        printf("%d\n",int_var);
      }
      else if(SQL_NO_DATA== query_ret ) break;
      else
      {
        //error in sql SQLFetch
        printf("%s\n", "fail to fetch data");
        break;
      }//fetch another row of data
      query_ret=SQLFetch(MYSQL_QUERY);
    }
      printf("------------------------------------------------------------\n");
  }
  
  else if(i==6)
  {
   
    char query[10000] ="SELECT n.Name "
            "FROM Procedures "
            "INNER JOIN Undergoes u ON u.Procedures = Procedures.Code and Procedures.Name = \"Bypass Surgery\" "
            "INNER JOIN Nurse n ON n.EmployeeID = u.AssistingNurse;";
    strcpy(sql, query);
    query_ret =SQLExecDirect(MYSQL_QUERY, sql, SQL_NTS);//execute the query and store the data in mysql_query
    
    
    if(query_ret !=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, MYSQL_QUERY);
      return 0;
    }
    query_ret=SQLFetch(MYSQL_QUERY);//fetch the data
    if(query_ret==SQL_SUCCESS||query_ret==SQL_SUCCESS_WITH_INFO)
    {//if the query hasn't returned empty set
      printf("------------------------------------------------------------");
      printf("\nNurse Name \n");
      printf("------------------------------------------------------------\n");
    }
    else if(SQL_NO_DATA==query_ret) printf("MYSQL Query returned Empty Set!\n");//mysql query has returned empty dataset

    while(1)
    {
      
      if(query_ret ==SQL_SUCCESS||query_ret ==SQL_SUCCESS_WITH_INFO)
      {
        query_ret =SQLGetData(MYSQL_QUERY, 1, SQL_C_CHAR, string_var_1, 100, &n);
       
        printf("%s\n",string_var_1);
       }
      else if(SQL_NO_DATA== query_ret ) break;
      else
      {
        //error in sql SQLFetch
        printf("%s\n", "fail to fetch data");
        break;
      }//fetch another row of data
      query_ret=SQLFetch(MYSQL_QUERY);
    }
    printf("------------------------------------------------------------\n");
  }
  
  else if(i==7)
  {
    
    char query[10000] ="SELECT n.Name as Nurse, n.Position, p.Name as Physician "
            "FROM Procedures "
            "INNER JOIN Undergoes u ON u.Procedures = Procedures.Code and Procedures.Name = \"Bypass Surgery\" "
            "INNER JOIN Nurse n ON n.EmployeeID = u.AssistingNurse "
            "INNER JOIN Physician p ON p.EmployeeID = u.Physician; ";
    strcpy(sql, query);
    query_ret =SQLExecDirect(MYSQL_QUERY, sql, SQL_NTS);//execute the query and store the data in mysql_query
    
    
    if(query_ret !=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, MYSQL_QUERY);
      return 0;
    }
    query_ret=SQLFetch(MYSQL_QUERY);//fetch the data
    if(query_ret==SQL_SUCCESS||query_ret==SQL_SUCCESS_WITH_INFO)
    {//if the query hasn't returned empty set
      printf("----------------------------------------------------------------------------------------------");
      printf("\nNurse Name");
      for(int j=0;j<40-strlen("Nurse Name");++j)printf(" ");
      printf("Nurse Position");
      for(int j=0;j<40-strlen("Nurse Position");++j)printf(" ");
      printf("Physician Name\n");
      printf("----------------------------------------------------------------------------------------------\n");
      
    }
    else if(SQL_NO_DATA==query_ret) printf("MYSQL Query returned Empty Set!\n");//mysql query has returned empty dataset
    

    while(1)
    {
      
      if(query_ret ==SQL_SUCCESS||query_ret ==SQL_SUCCESS_WITH_INFO)
      {
        query_ret =SQLGetData(MYSQL_QUERY, 1, SQL_C_CHAR, string_var_1, 100, &n);
        query_ret =SQLGetData(MYSQL_QUERY, 2, SQL_C_CHAR, string_var_2, 100, &n);
        query_ret =SQLGetData(MYSQL_QUERY, 3, SQL_C_CHAR, string_var_3, 100, &n);
        
        printf("%s",string_var_1);
        for(int j=0;j<40-strlen(string_var_1);++j)printf(" ");
        printf("%s",string_var_2);
        for(int j=0;j<40-strlen(string_var_2);++j)printf(" ");
        printf("%s\n",string_var_3);
      }
      else if(SQL_NO_DATA== query_ret ) break;
      else
      {
        //error in sql SQLFetch
        printf("%s\n", "fail to fetch data");
        break;
      }//fetch another row of data
      query_ret=SQLFetch(MYSQL_QUERY);
    }
    printf("----------------------------------------------------------------------------------------------\n");
  }
  
  else if(i==8)
  {
    
    
    char query[10000] ="SELECT Physician.Name "
            "FROM Physician "
            "INNER JOIN Undergoes ON Undergoes.Physician = Physician.EmployeeID "
            "WHERE (Undergoes.Physician , Undergoes.Procedures ) NOT IN (SELECT Physician, Treatment FROM Trained_in);";
    strcpy(sql, query);
    query_ret =SQLExecDirect(MYSQL_QUERY, sql, SQL_NTS);//execute the query and store the data in mysql_query
    
    
    if(query_ret !=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, MYSQL_QUERY);
      return 0;
    }
    query_ret=SQLFetch(MYSQL_QUERY);//fetch the data
    if(query_ret==SQL_SUCCESS||query_ret==SQL_SUCCESS_WITH_INFO)
    {//if the query hasn't returned empty set
      printf("-------------------------------------------------------------------\n");
      printf("\nPhysician Name \n");
      printf("-------------------------------------------------------------------\n");
    }
    else if(SQL_NO_DATA==query_ret) printf("MYSQL Query returned Empty Set!\n");//mysql query has returned empty dataset
    

    while(1)
    {
      
      if(query_ret ==SQL_SUCCESS||query_ret ==SQL_SUCCESS_WITH_INFO)
      {
        query_ret =SQLGetData(MYSQL_QUERY, 1, SQL_C_CHAR, string_var_1, 100, &n);
       
        printf("%s\n",string_var_1);
       }
      else if(SQL_NO_DATA== query_ret ) break;
      else
      {
        //error in sql SQLFetch
        printf("%s\n", "fail to fetch data");
        break;
      }//fetch another row of data
      query_ret=SQLFetch(MYSQL_QUERY);
    }
    printf("-------------------------------------------------------------------\n");
  }
  
  else if(i==9)
  {

    
    char query[10000] ="WITH train AS ( "
            "SELECT p.name, p.EmployeeID as te, Trained_in.Treatment as a, Trained_in.CertificationExpires as ce "
            "From Trained_in "
            "INNER JOIN Physician p ON p.EmployeeID = Trained_in.Physician "
            ") "
            "SELECT distinct t.Name "
            "FROM Physician "
            "INNER JOIN Undergoes u ON u.Physician = Physician.EmployeeID "
            "INNER JOIN train t ON t.te = u. Physician and t.a = u.Procedures and DATEDIFF(u.Date, t.ce)>0;";
    strcpy(sql, query);
    query_ret =SQLExecDirect(MYSQL_QUERY, sql, SQL_NTS);//execute the query and store the data in mysql_query
    
    
    if(query_ret !=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, MYSQL_QUERY);
      return 0;
    }
    query_ret=SQLFetch(MYSQL_QUERY);//fetch the data
    if(query_ret==SQL_SUCCESS||query_ret==SQL_SUCCESS_WITH_INFO)
    {
      printf("-------------------------------------------------------------------\n");
       printf("\nPhysician Name \n");
       printf("-------------------------------------------------------------------\n");
    }
    else if(SQL_NO_DATA==query_ret) printf("MYSQL Query returned Empty Set!\n");//mysql query has returned empty dataset
    

    while(1)
    {
      
      if(query_ret ==SQL_SUCCESS||query_ret ==SQL_SUCCESS_WITH_INFO)
      {
        query_ret =SQLGetData(MYSQL_QUERY, 1, SQL_C_CHAR, string_var_1, 100, &n);
       
        printf("%s\n",string_var_1);
       }
      else if(SQL_NO_DATA== query_ret ) break;
      else
      {
        //error in sql SQLFetch
        printf("%s\n", "fail to fetch data");
        break;
      }//fetch another row of data
      query_ret=SQLFetch(MYSQL_QUERY);
    }
    printf("-------------------------------------------------------------------\n");
  }
  
  else if(i==10)
  {
   
    char query[10000] ="WITH train AS ( "
            "    SELECT p.name, p.EmployeeID as te, Trained_in.Treatment as a, Trained_in.CertificationExpires as ce "
            "    From Trained_in "
            "    INNER JOIN Physician p ON p.EmployeeID = Trained_in.Physician "
            ") "
            "SELECT t.Name as Physician, pr.name, u.Date, pa.Name as Patient "
            "FROM Physician "
            "INNER JOIN Undergoes u ON u.Physician = Physician.EmployeeID "
            "INNER JOIN train t ON t.te = u. Physician and t.a = u.Procedures and DATEDIFF(u.Date, t.ce)>0 "
            "INNER JOIN Patient pa ON pa.SSN = u.patient "
            "INNER JOIN Procedures pr ON u.Procedures = pr.Code;";
    strcpy(sql, query);
    query_ret =SQLExecDirect(MYSQL_QUERY, sql, SQL_NTS);//execute the query and store the data in mysql_query
    
    
    if(query_ret !=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, MYSQL_QUERY);
      return 0;
    }
    query_ret=SQLFetch(MYSQL_QUERY);//fetch the data
    if(query_ret==SQL_SUCCESS||query_ret==SQL_SUCCESS_WITH_INFO)
    {//if the query hasn't returned empty set
    printf("----------------------------------------------------------------------------------------------------------\n");
      printf("\nPhysician Name");
      for(int j=0;j<25-strlen("Physician Name");++j)printf(" ");
      printf("Procedure Name");
      for(int j=0;j<25-strlen("Procedure Name");++j)printf(" ");
      printf("Date");
      for(int j=0;j<38-strlen("Date");++j)printf(" ");
      printf("Patient Name\n");
      printf("----------------------------------------------------------------------------------------------------------\n");
    }
    else if(SQL_NO_DATA==query_ret) printf("MYSQL Query returned Empty Set!\n");//mysql query has returned empty dataset
    

    while(1)
    {
      
      if(query_ret ==SQL_SUCCESS||query_ret ==SQL_SUCCESS_WITH_INFO)
      {
        query_ret =SQLGetData(MYSQL_QUERY, 1, SQL_C_CHAR, string_var_1, 100, &n);
        query_ret =SQLGetData(MYSQL_QUERY, 2, SQL_C_CHAR, string_var_2, 100, &n);
        query_ret =SQLGetData(MYSQL_QUERY, 3, SQL_C_TYPE_TIMESTAMP,&date_time,0,&n);
        query_ret =SQLGetData(MYSQL_QUERY, 4, SQL_C_CHAR, string_var_2, 100, &n);
        printf("%s",string_var_1);
        for(int j=0;j<25-strlen(string_var_1);++j)printf(" ");
        printf("%s",string_var_2);
        for(int j=0;j<25-strlen(string_var_2);++j)printf(" ");
        printf("%d-%d-%d 0%d:0%d:0%d",date_time.year,date_time.month,date_time.day,date_time.hour,date_time.minute,date_time.second);
        for(int j=0;j<20;++j)printf(" ");
        printf("%s\n",string_var_3);
       }
      else if(SQL_NO_DATA== query_ret ) break;
      else
      {
        //error in sql SQLFetch
        printf("%s\n", "fail to fetch data");
        break;
      }//fetch another row of data
      query_ret=SQLFetch(MYSQL_QUERY);
    }
    printf("----------------------------------------------------------------------------------------------------------\n");
  }
  
  else if(i==11)
  {

    char query[10000] ="WITH pat AS( "
            "SELECT Patient.SSN, Patient.name, Patient.PCP, COUNT(Patient.SSN) "
            "FROM Patient "
            "INNER JOIN Appointment a ON a.Patient = Patient.SSN "
            "INNER JOIN Physician ph ON ph.EmployeeID = a.Physician "
            "INNER JOIN Affiliated_with af ON af.Physician = a.Physician "
            "INNER JOIN Department d ON af.Department = d.DepartmentID and d.Name=\"Cardiology\" "
            "GROUP BY Patient.SSN "
            "HAVING COUNT(Patient.SSN) >= 2 "
            ") "
            "SELECT distinct pa.name, ph.Name as PhysicianName "
            "FROM pat "
            "INNER JOIN Patient pa ON pa.SSN = pat.SSN "
            "INNER JOIN Appointment a ON a.Patient = pa.SSN "
            "INNER JOIN Affiliated_with af ON af.Physician = a.Physician "
            "INNER JOIN Department d ON af.Department = d.DepartmentID and d.Head != a.Physician "
            "INNER JOIN Undergoes u ON u.patient = pat.SSN "
            "INNER JOIN Procedures p ON p.Code = u.Procedures " 
            "INNER JOIN Prescribes pr ON  pr.Patient = pat.SSN and pr.Physician = pat.PCP "
            "INNER JOIN Physician ph ON ph.EmployeeID = pa.PCP "
            "WHERE p.Cost > 5000;";
    strcpy(sql, query);
    query_ret =SQLExecDirect(MYSQL_QUERY, sql, SQL_NTS);//execute the query and store the data in mysql_query
    
    
    if(query_ret !=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, MYSQL_QUERY);
      return 0;
    }
    query_ret=SQLFetch(MYSQL_QUERY);//fetch the data
    if(query_ret==SQL_SUCCESS||query_ret==SQL_SUCCESS_WITH_INFO)
    {//if the query hasn't returned empty set
    printf("-------------------------------------------------------------------------\n");
      printf("\nPatient Name");
      for(int j=0;j<25-strlen("Patient Name");++j)printf(" ");
      printf("Physician Name\n");
      printf("-------------------------------------------------------------------------\n");
    }
    else if(SQL_NO_DATA==query_ret) printf("MYSQL Query returned Empty Set!\n");//mysql query has returned empty dataset
    

    while(1)
    {
      
      if(query_ret ==SQL_SUCCESS||query_ret ==SQL_SUCCESS_WITH_INFO)
      {
        query_ret =SQLGetData(MYSQL_QUERY, 1, SQL_C_CHAR, string_var_1, 100, &n);
        query_ret =SQLGetData(MYSQL_QUERY, 2, SQL_C_CHAR, string_var_2, 100, &n);
       
        printf("%s",string_var_1);
        for(int j=0;j<25-strlen(string_var_1);++j)printf(" ");
        printf("%s\n",string_var_2);
       }
      else if(SQL_NO_DATA== query_ret ) break;
      else
      {
        //error in sql SQLFetch
        printf("%s\n", "fail to fetch data");
        break;
      }//fetch another row of data
      query_ret=SQLFetch(MYSQL_QUERY);
    }
    printf("-------------------------------------------------------------------------\n");
  }
  
  else if(i==12)
  {
    
    char query[10000] ="WITH medicine AS ( "
                "SELECT Medication, COUNT(*) as med_count "
                "FROM Prescribes "
                "GROUP BY Medication "
            ") "
            "SELECT Name, Brand "
            "FROM Medication "
            "INNER JOIN medicine ON Medication.Code = medicine.Medication "
            "WHERE medicine.med_count = (SELECT MAX(med_count) FROM medicine);";
    strcpy(sql, query);
    query_ret =SQLExecDirect(MYSQL_QUERY, sql, SQL_NTS);//execute the query and store the data in mysql_query
    
    
    if(query_ret !=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, MYSQL_QUERY);
      return 0;
    }
    query_ret=SQLFetch(MYSQL_QUERY);//fetch the data
    if(query_ret==SQL_SUCCESS||query_ret==SQL_SUCCESS_WITH_INFO)
    {//if the query hasn't returned empty set
    printf("-------------------------------------------------------------------------\n");
      printf("\nMedicine Name");
      for(int j=0;j<25-strlen("Medicine Name");++j)printf(" ");
      printf("Medicine Brand\n");
      printf("-------------------------------------------------------------------------\n");
    }
    else if(SQL_NO_DATA==query_ret) printf("MYSQL Query returned Empty Set!\n");//mysql query has returned empty dataset
    

    while(1)
    {
      
      if(query_ret ==SQL_SUCCESS||query_ret ==SQL_SUCCESS_WITH_INFO)
      {
        query_ret =SQLGetData(MYSQL_QUERY, 1, SQL_C_CHAR, string_var_1, 100, &n);
        query_ret =SQLGetData(MYSQL_QUERY, 2, SQL_C_CHAR, string_var_2, 100, &n);
       
        printf("%s",string_var_1);
        for(int j=0;j<25-strlen(string_var_1);++j)printf(" ");
        printf("%s\n",string_var_2);
       }
      else if(SQL_NO_DATA== query_ret ) break;
      else
      {
        //error in sql SQLFetch
        printf("%s\n", "fail to fetch data");
        break;
      }//fetch another row of data
      query_ret=SQLFetch(MYSQL_QUERY);
    }
    printf("-------------------------------------------------------------------------\n");
  }
  
  else if(i==13)
  {
    char input[1000];
    char c;
    printf("Enter a Procedure: ");
    scanf("%c", &c);
    int j=0;
    

    while(1){
      scanf("%c",&input[j]);
      if (input[j]=='\n'){
        input[j]='\0'; 
        break;
      }
      j++;
      
    }
    char query[10000] = "SELECT Physician.Name FROM Physician INNER JOIN Trained_in t ON t.Physician = Physician.EmployeeID INNER JOIN Procedures pr ON pr.Code = t.Treatment and pr.Name = '";
    strcat(query,input);
    strcat(query,"';");
    
    strcpy(sql, query);
    
    query_ret =SQLExecDirect(MYSQL_QUERY, sql, SQL_NTS);//execute the query and store the data in mysql_query
    if(query_ret !=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, MYSQL_QUERY);
      return 0;
    }

    query_ret=SQLFetch(MYSQL_QUERY);
    if(query_ret==SQL_SUCCESS||query_ret==SQL_SUCCESS_WITH_INFO)
    {
      printf("-------------------------------------------------------------------------\n");
      printf("\nPhysician Name\n");
    }
    else if(SQL_NO_DATA==query_ret) printf("MYSQL Query returned Empty Set!\n");//mysql query has returned empty dataset
    
    while(1)
    {
      
      if(query_ret ==SQL_SUCCESS||query_ret ==SQL_SUCCESS_WITH_INFO)
      {
        query_ret =SQLGetData(MYSQL_QUERY, 1, SQL_C_CHAR, string_var_1, 20, &n);
        printf("%s\n",string_var_1);
      }
      else if(SQL_NO_DATA== query_ret ) 
      {
        break;
      }
      else
      {
        //error in sql SQLFetch
        printf("%s\n", "fail to fetch data");
        break;
      }
      query_ret=SQLFetch(MYSQL_QUERY);
    }
    printf("-------------------------------------------------------------------------\n");
   }

}
int main(void)
{
  //Connect to database using DSN, username and password
  DATABASE_CONNECT("MYSQL_ODBC", "20CS30065", "20CS30065");
  
  //Ask for query until -1 is pressed
  while(1){
    printf("\nEnter -1 to exit");
    if(EXECUTE_MYSQL_QUERY()==-1)break;
  }

  //database disconnected
  DATABASE_DISCONNECT();
  return 0;
}