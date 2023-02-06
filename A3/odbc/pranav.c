#include <stdio.h>
#include<windows.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>

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
  SQLCHAR sql[1000];
  SQLLEN n;
  SQLINTEGER id;
  SQLCHAR name[100];
  SQLCHAR addr[100];
  SQLCHAR phy[100];
  SQLREAL age;
  SQL_DATE_STRUCT birthday;
  SQL_TIMESTAMP_STRUCT create_timestamp;

  if(hdbc==NULL || henv==NULL) return 0;

  r=SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
  if(!(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)) return 0;

  int i;
  printf("Enter a number: ");
  scanf("%d", &i);
  if(i==1)
  {
    printf("\nPhysician Name\n");
    strcpy(sql,"SELECT Name \"Physician Name\" "
"FROM Physician "
"WHERE EmployeeID IN (SELECT Physician "
                     "FROM Trained_In "
                     "WHERE Treatment = (SELECT Code "
                                        "FROM Procedures "
                                        "WHERE Name = \"Bypass Surgery\")); ");
    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    while(1)
    {
      r=SQLFetch(hstmt);
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, name, 20, &n);
        printf("%s\n",name);
      }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
    }
  }
  
  else if(i==2)
  {
    char query[10000] = "SELECT p.Name FROM Department " 
    "INNER JOIN Affiliated_with a ON a.Department = Department.DepartmentID and Department.Name = \'Cardiology\' "
    "INNER JOIN Physician p ON p.EmployeeID=a.Physician "
    "INNER JOIN Trained_in t ON t.Physician = p.EmployeeID "
    "INNER JOIN Procedures pr ON pr.Code = t.Treatment and pr.Name = \'Bypass Surgery\';";
    printf("\nPhysician Name\n");
    strcpy(sql, query);
    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    while(1)
    {
      r=SQLFetch(hstmt);
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, name, 20, &n);
        printf("%s\n",name);
      }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
    }
  }
  
  else if(i==3)
  {
    char query[10000] = "SELECT Nurse.Name "
            "FROM  Nurse "
            "INNER JOIN On_Call n ON n.Nurse = Nurse.EmployeeID "
            "INNER JOIN Room ON Room.BlockCode = n.BlockCode and Room.BlockFloor = n.BlockFloor and Room.Number = 123;"; 
    printf("\nNurse Name\n");
    strcpy(sql, query);
    
    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    while(1)
    {
      r=SQLFetch(hstmt);
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, name, 20, &n);
        printf("%s\n",name);
      }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
    }
  }
  

  else if(i==4)
  {
    printf("\nPatient Name");
    for(int j=0;j<40-strlen("Patient Name");++j)printf(" ");
    printf("Patient Address\n");
    
    char query[10000] ="SELECT pa.Name, pa.Address "
            "FROM Medication "
            "INNER JOIN Prescribes p ON p.Medication = Medication.Code and Medication.Name='Remdesivir' "
            "INNER JOIN Patient pa ON pa.SSN = p.Patient;";
    strcpy(sql, query);
    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    
    
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    while(1)
    {
      r=SQLFetch(hstmt);
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, name, 100, &n);
        r=SQLGetData(hstmt, 2, SQL_C_CHAR, addr, 100, &n);
        printf("%s",name);
        for(int j=0;j<40-strlen(name);++j)printf(" ");
        printf("%s\n",addr);
      }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
    }
  }
  
  else if(i==5)
  {
    printf("\nPatient Name");
    for(int j=0;j<25-strlen("Patient Name");++j)printf(" ");
    printf("Patient Inusrance ID\n");
    
    char query[10000] ="SELECT p.Name, p.InsuranceID "
            "FROM Room "
            "INNER JOIN Stay s ON s.Room = Room.Number and Room.Type = \"ICU\" "
            "INNER JOIN Patient p ON p.SSN = s.Patient and  DATEDIFF(s.End, s.Start)>15;";
            
    strcpy(sql, query);
    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    
    
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    while(1)
    {
      r=SQLFetch(hstmt);
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, name, 100, &n);
        r=SQLGetData(hstmt, 2, SQL_C_ULONG, &id, 0, &n);
        printf("%s",name);
        for(int j=0;j<25-strlen(name);++j)printf(" ");
        printf("%d\n",id);
      }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
    }
  }
  
  else if(i==6)
  {
    printf("\nNurse Name \n");

    
    char query[10000] ="SELECT n.Name "
            "FROM Procedures "
            "INNER JOIN Undergoes u ON u.Procedures = Procedures.Code and Procedures.Name = \"Bypass Surgery\" "
            "INNER JOIN Nurse n ON n.EmployeeID = u.AssistingNurse;";
    strcpy(sql, query);
    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    
    
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    while(1)
    {
      r=SQLFetch(hstmt);
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, name, 100, &n);
       
        printf("%s\n",name);
       }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
    }
  }
  
  else if(i==7)
  {
    printf("\nNurse Name");
    for(int j=0;j<25-strlen("Nurse Name");++j)printf(" ");
    printf("Nurse Position");
    for(int j=0;j<25-strlen("Nurse Position");++j)printf(" ");
    printf("Physician Name\n");
    
    char query[10000] ="SELECT n.Name as Nurse, n.Position, p.Name as Physician "
            "FROM Procedures "
            "INNER JOIN Undergoes u ON u.Procedures = Procedures.Code and Procedures.Name = \"Bypass Surgery\" "
            "INNER JOIN Nurse n ON n.EmployeeID = u.AssistingNurse "
            "INNER JOIN Physician p ON p.EmployeeID = u.Physician; ";
    strcpy(sql, query);
    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    
    
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    while(1)
    {
      r=SQLFetch(hstmt);
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, name, 100, &n);
        r=SQLGetData(hstmt, 2, SQL_C_CHAR, phy, 100, &n);
        r=SQLGetData(hstmt, 3, SQL_C_CHAR, addr, 100, &n);
        
        printf("%s",name);
        for(int j=0;j<25-strlen(name);++j)printf(" ");
        printf("%s",phy);
        for(int j=0;j<25-strlen(phy);++j)printf(" ");
        printf("%s\n",addr);
      }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
    }
  }
  
  else if(i==8)
  {
    printf("\nPhysician Name \n");

    
    char query[10000] ="SELECT Physician.Name "
            "FROM Physician "
            "INNER JOIN Undergoes ON Undergoes.Physician = Physician.EmployeeID "
            "WHERE (Undergoes.Physician , Undergoes.Procedures ) NOT IN (SELECT Physician, Treatment FROM Trained_in);";
    strcpy(sql, query);
    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    
    
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    while(1)
    {
      r=SQLFetch(hstmt);
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, name, 100, &n);
       
        printf("%s\n",name);
       }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
    }
  }
  
  else if(i==9)
  {
    printf("\nPhysician Name \n");

    
    char query[10000] ="WITH train AS ( "
            "SELECT p.Name, p.EmployeeID as te, Trained_in.Treatment as a, Trained_in.CertificationExpires as ce "
            "From Trained_in "
            "INNER JOIN Physician p ON p.EmployeeID = Trained_in.Physician "
            ") "
            "SELECT distinct t.Name "
            "FROM Physician "
            "INNER JOIN Undergoes u ON u.Physician = Physician.EmployeeID "
            "INNER JOIN train t ON t.te = u. Physician and t.a = u.Procedures and DATEDIFF(u.Date, t.ce)>0;";
    strcpy(sql, query);
    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    
    
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    while(1)
    {
      r=SQLFetch(hstmt);
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, name, 100, &n);
       
        printf("%s\n",name);
       }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
    }
  }
  
  else if(i==10)
  {
    printf("\nPhysician Name");
    for(int j=0;j<25-strlen("Physician Name");++j)printf(" ");
    printf("Procedure Name");
    for(int j=0;j<25-strlen("Procedure Name");++j)printf(" ");
    printf("Date");
    for(int j=0;j<38-strlen("Date");++j)printf(" ");
    printf("Patient Name\n");
    
    char query[10000] ="WITH train AS ( "
            "    SELECT p.Name, p.EmployeeID as te, Trained_in.Treatment as a, Trained_in.CertificationExpires as ce "
            "    From Trained_in "
            "    INNER JOIN Physician p ON p.EmployeeID = Trained_in.Physician "
            ") "
            "SELECT t.Name as Physician, pr.Name, u.Date, pa.Name as Patient "
            "FROM Physician "
            "INNER JOIN Undergoes u ON u.Physician = Physician.EmployeeID "
            "INNER JOIN train t ON t.te = u. Physician and t.a = u.Procedures and DATEDIFF(u.Date, t.ce)>0 "
            "INNER JOIN Patient pa ON pa.SSN = u.patient "
            "INNER JOIN Procedures pr ON u.Procedures = pr.Code;";
    strcpy(sql, query);
    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    
    
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    while(1)
    {
      r=SQLFetch(hstmt);
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, name, 100, &n);
        r=SQLGetData(hstmt, 2, SQL_C_CHAR, addr, 100, &n);
        r=SQLGetData(hstmt, 3, SQL_C_TYPE_TIMESTAMP,&create_timestamp,0,&n);
        r=SQLGetData(hstmt, 4, SQL_C_CHAR, phy, 100, &n);

        printf("%s",name);
        for(int j=0;j<25-strlen(name);++j)printf(" ");
        printf("%s",addr);
        for(int j=0;j<25-strlen(addr);++j)printf(" ");
        printf("%d-%d-%d 0%d:0%d:0%d",create_timestamp.year,create_timestamp.month,create_timestamp.day,create_timestamp.hour,create_timestamp.minute,create_timestamp.second);
        for(int j=0;j<20;++j)printf(" ");
        printf("%s\n",phy);
       }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
    }
  }
  
  else if(i==11)
  {
     printf("\nPatient Name");
    for(int j=0;j<25-strlen("Patient Name");++j)printf(" ");
    printf("Physician Name\n");

    
    char query[10000] ="WITH pat AS( "
            "SELECT Patient.SSN, Patient.Name, Patient.PCP, COUNT(Patient.SSN) "
            "FROM Patient "
            "INNER JOIN Appointment a ON a.Patient = Patient.SSN "
            "INNER JOIN Physician ph ON ph.EmployeeID = a.Physician "
            "INNER JOIN Affiliated_with af ON af.Physician = a.Physician "
            "INNER JOIN Department d ON af.Department = d.DepartmentID and d.Name=\"Cardiology\" "
            "GROUP BY Patient.SSN "
            "HAVING COUNT(Patient.SSN) >= 2 "
            ") "
            "SELECT distinct pa.Name, ph.Name as PhysicianName "
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
    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    
    
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    while(1)
    {
      r=SQLFetch(hstmt);
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, name, 100, &n);
        r=SQLGetData(hstmt, 2, SQL_C_CHAR, addr, 100, &n);
       
        printf("%s",name);
        for(int j=0;j<25-strlen(name);++j)printf(" ");
        printf("%s\n",addr);
       }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
    }
  }
  
  else if(i==12)
  {
    printf("\nMedicine Name");
    for(int j=0;j<25-strlen("Medicine Name");++j)printf(" ");
    printf("Medicine Brand\n");

    
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
    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    
    
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    while(1)
    {
      r=SQLFetch(hstmt);
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, name, 100, &n);
        r=SQLGetData(hstmt, 2, SQL_C_CHAR, addr, 100, &n);
       
        printf("%s",name);
        for(int j=0;j<25-strlen(name);++j)printf(" ");
        printf("%s\n",addr);
       }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
    }
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

    
    printf("\nPhysician Name\n");
    strcpy(sql, query);
    
    r=SQLExecDirect(hstmt, sql, SQL_NTS);
    if(r!=SQL_SUCCESS) 
    {
      printf("cannot access [%s]\n", sql);
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return 0;
    }

    while(1)
    {
      r=SQLFetch(hstmt);
      if(r==SQL_SUCCESS||r==SQL_SUCCESS_WITH_INFO)
      {
        r=SQLGetData(hstmt, 1, SQL_C_CHAR, name, 20, &n);
        printf("%s\n",name);
      }
      else if(SQL_NO_DATA==r) break;
      else
      {
        printf("%s\n", "fail to fetch data");
        break;
      }
    }
   }
  

  SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
}
int main(void)
{ 
  // char pass[20];
  // scanf("%s",pass);
  // printf("%s\n",pass);
  ODBCConnectDB("PRANAV_MYSQL", "20CS10085", "20CS10085");
  // printf("Hello\n");
  db_fetch();
  ODBCDisconnectDB();
}