import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.Scanner;
import java.util.ArrayList;

public class java_20CS30065 {

    private static String MYSQL_JDBC_DRIVER_CLASS = "com.mysql.cj.jdbc.Driver";
    private static String MYSQL_DB_URL = "jdbc:mysql://10.5.18.72:3306/20CS30065";
    private static String MYSQL_DB_USER = "20CS30065";
    private static String MYSQL_DB_USER_PASSWORD = "20CS30065";


    public static void format_data(ArrayList<ArrayList<String>> data) {
        
        int[] width_col = new int[data.get(0).size()];
        for (ArrayList<String> data_row : data) {
            for (int i = 0; i < data_row.size(); i++) {
                //for column i, traverse through all rows, get the row with longest string in this column and set the 
                //max witdth of column as the length of this string
                width_col[i] = Math.max(width_col[i], data_row.get(i).length());
            }
        }
        for (int i = 0; i < data.get(0).size(); i++) {
            width_col[i] += 4; //add a padding of 2 character on both side of a column
        }

        StringBuilder border = new StringBuilder();
        //
        border.append("|");
        for (int width : width_col) {
            for (int i = 0; i <= width; i++) {
                border.append("-");
            }
            border.append("|");
        }
        //horizontal divider of the form |---|---|---|
        
        System.out.println(border.toString());
        for (ArrayList<String> data_row : data) {
            System.out.print("|");
            for (int i = 0; i < data_row.size(); i++) {
                System.out.print(" " + data_row.get(i));
                int padding = width_col[i] - data_row.get(i).length() - 1;
                System.out.print(" ".repeat(padding));
                System.out.print(" |");
            }
            System.out.println();
            System.out.println(border.toString());
        }
    }
    /* 
    For each row, first print the left boundary of the cell,
    followed by the value in the cell, padded with spaces to make the width 
    of the cell equal to the calculated width of the column. After printing 
    each cell of the row, it prints a newline character, followed by the horizontal 
    and then repeats the process for the next row.
    */
    public static void main(String[] args) {
            
        try(Connection connection = DriverManager.getConnection(MYSQL_DB_URL,MYSQL_DB_USER,MYSQL_DB_USER_PASSWORD)) {
            //mysql connection established and mysql_connector api is created which will help in executing 
            //mysql functions
            Class.forName(MYSQL_JDBC_DRIVER_CLASS); 
            Statement mysql_connector =connection.createStatement(); 

            ArrayList<ArrayList<String>> query_set = new ArrayList<>();
            ArrayList<String>query_row = new ArrayList<>();
            
            int flag=0;

            Scanner stdin_scanner= new Scanner(System.in);
            
            while(true){
            System.out.print("\nPress -1 to Exit\n");
            System.out.print("Enter Query Number which you want to execute: ");  

            //scan an integer
            int a= stdin_scanner.nextInt(); 
            String SQL_QUERY;
            ResultSet query_output;
            
            
            switch(a) {
                case -1:
                    //exit if user enter -1 and set flag as 1
                    System.out.print("Exiting.......");
                    flag=1;
                    break;
                
                case 1:
                SQL_QUERY = "SELECT Physician.Name FROM Physician INNER JOIN Trained_in t ON t.Physician = Physician.EmployeeID INNER JOIN Procedures pr ON pr.Code = t.Treatment and pr.Name = 'Bypass Surgery';";
                query_output = mysql_connector.executeQuery(SQL_QUERY); 

                if(!query_output.isBeforeFirst() && !query_output.isAfterLast()) 
                {
                    System.out.print("MYSQL Query has returned empty set !\n!");
                    break;
                }

                //flush out whatever was stored in the arrays
                query_set.clear();
                query_row.clear();

                //add header row
                query_row.add("Physician's Name");

                //push this header row to table
                query_set.add(new ArrayList<>(query_row));

                //iterate over all rows received as output of the query
                while(query_output.next())  
                {
                    query_row.clear();
                    //add the row to the table
                    query_row.add(query_output.getString(1));
                    query_set.add(new ArrayList<>(query_row));
                }
                //print the content stored in query_set data structure
                format_data(query_set);
                break;
                
                case 2:
                SQL_QUERY = "SELECT p.Name FROM Department " 
                + "INNER JOIN Affiliated_with a ON a.Department = Department.DepartmentID and Department.Name = \'Cardiology\' "
                + "INNER JOIN Physician p ON p.EmployeeID=a.Physician "
                + "INNER JOIN Trained_in t ON t.Physician = p.EmployeeID "
                + "INNER JOIN Procedures pr ON pr.Code = t.Treatment and pr.Name = \'Bypass Surgery\';";
                query_output = mysql_connector.executeQuery(SQL_QUERY); 
                
                if(!query_output.isBeforeFirst() && !query_output.isAfterLast()) 
                {
                    System.out.print("MYSQL Query has returned empty set !\n!");
                    break;
                }

                //flush out the existing content of the arrays
                query_set.clear();
                query_row.clear();

                //add header row
                query_row.add("Physician's Name");

                //push this header row to table
                query_set.add(new ArrayList<>(query_row));

                //iterate over all rows received as output of the query
                while(query_output.next())  
                {
                    query_row.clear();
                    //add the row to the table
                    query_row.add(query_output.getString(1));
                    query_set.add(new ArrayList<>(query_row));
                }
                //print the content stored in query_set data structure
                format_data(query_set);
                break;
                
                case 3:
                SQL_QUERY ="SELECT Nurse.Name "
                + "FROM  Nurse "
                + "INNER JOIN On_Call n ON n.Nurse = Nurse.EmployeeID "
                + "INNER JOIN Room ON Room.BlockCode = n.BlockCode and Room.BlockFloor = n.BlockFloor and Room.Number = 123;"; 
                query_output = mysql_connector.executeQuery(SQL_QUERY); 

                if(!query_output.isBeforeFirst() && !query_output.isAfterLast()) 
                {
                    System.out.print("MYSQL Query has returned empty set !\n!");
                    break;
                }

                //flush out the existing content of the arrays
                query_set.clear();
                query_row.clear();

                //add header row
                query_row.add("Nurse's Name");

                //push this header row to table
                query_set.add(new ArrayList<>(query_row));

                //iterate over all rows received as output of the query
                while(query_output.next())  
                {
                    query_row.clear();
                    //add the row to the table
                    query_row.add(query_output.getString(1));
                    query_set.add(new ArrayList<>(query_row));
                }
                //print the content stored in query_set data structure
                format_data(query_set);
                     
                break;

                case 4:
                SQL_QUERY ="SELECT pa.Name, pa.Address "
                + "FROM Medication "
                + "INNER JOIN Prescribes p ON p.Medication = Medication.Code and Medication.Name='Remdesivir' "
                + "INNER JOIN Patient pa ON pa.SSN = p.Patient;";
                query_output = mysql_connector.executeQuery(SQL_QUERY); 

                if(!query_output.isBeforeFirst() && !query_output.isAfterLast()) 
                {
                    System.out.print("MYSQL Query has returned empty set !\n!");
                    break;
                }

                //flush out the existing content of the arrays
                query_set.clear();
                query_row.clear();

                //add header row
                query_row.add("Patient's Name");
                query_row.add("Patient's Address");

                //push this header row to table
                query_set.add(new ArrayList<>(query_row));

                //iterate over all rows received as output of the query
                while(query_output.next())  
                {
                    query_row.clear();
                    //add the row to the table
                    query_row.add(query_output.getString(1));
                    query_row.add(query_output.getString(2));
                    query_set.add(new ArrayList<>(query_row));
                }
                //print the content stored in query_set data structure
                format_data(query_set);
                     
                break;
               
                case 5:
                SQL_QUERY ="SELECT p.Name, p.InsuranceID "
                + "FROM Room "
                + "INNER JOIN Stay s ON s.Room = Room.Number and Room.Type = \"ICU\" "
                + "INNER JOIN Patient p ON p.SSN = s.Patient and  DATEDIFF(s.End, s.Start)>15;";
                query_output = mysql_connector.executeQuery(SQL_QUERY); 
               
                if(!query_output.isBeforeFirst() && !query_output.isAfterLast()) 
                {
                    System.out.print("MYSQL Query has returned empty set !\n!");
                    break;
                }

                //flush out the existing content of the arrays
                query_set.clear();
                query_row.clear();

                //add header row
                query_row.add("Patient's Name");
                query_row.add("Patient's InsuranceId");

                //push this header row to table
                query_set.add(new ArrayList<>(query_row));

                //iterate over all rows received as output of the query
                while(query_output.next())  
                {
                    query_row.clear();
                    //add the row to the table
                    query_row.add(query_output.getString(1));
                    query_row.add(query_output.getString(2));
                    query_set.add(new ArrayList<>(query_row));
                }
                //print the content stored in query_set data structure
                format_data(query_set);

                break;
                
                case 6:

                SQL_QUERY ="SELECT n.Name "
                + "FROM Procedures "
                + "INNER JOIN Undergoes u ON u.Procedures = Procedures.Code and Procedures.Name = \"Bypass Surgery\" "
                + "INNER JOIN Nurse n ON n.EmployeeID = u.AssistingNurse;";
                query_output = mysql_connector.executeQuery(SQL_QUERY); 

               //flush out the existing content of the arrays
               query_set.clear();
               query_row.clear();

               //add header row
               query_row.add("Nurse's Name");

               //push this header row to table
               query_set.add(new ArrayList<>(query_row));

               //iterate over all rows received as output of the query
               while(query_output.next())  
               {
                   query_row.clear();
                   //add the row to the table
                   query_row.add(query_output.getString(1));
                   query_set.add(new ArrayList<>(query_row));
               }
               //print the content stored in query_set data structure
               format_data(query_set);
               break;
                
                case 7:
                SQL_QUERY ="SELECT n.Name as Nurse, n.Position, p.Name as Physician "
                + "FROM Procedures "
                + "INNER JOIN Undergoes u ON u.Procedures = Procedures.Code and Procedures.Name = \"Bypass Surgery\" "
                + "INNER JOIN Nurse n ON n.EmployeeID = u.AssistingNurse "
                + "INNER JOIN Physician p ON p.EmployeeID = u.Physician; ";
                query_output = mysql_connector.executeQuery(SQL_QUERY); 

                if(!query_output.isBeforeFirst() && !query_output.isAfterLast()) 
                {
                    System.out.print("MYSQL Query has returned empty set !\n!");
                    break;
                }

                //flush out the existing content of the arrays
                query_set.clear();
                query_row.clear();

                //add header row
                query_row.add("Nurse's Name");
                query_row.add("Nurse's Position");
                query_row.add("Physician's Name");

                //push this header row to table
                query_set.add(new ArrayList<>(query_row));

                //iterate over all rows received as output of the query
                while(query_output.next())  
                {
                    query_row.clear();
                    //add the row to the table
                    query_row.add(query_output.getString(1));
                    query_row.add(query_output.getString(2));
                    query_row.add(query_output.getString(3));
                    query_set.add(new ArrayList<>(query_row));
                }
                //print the content stored in query_set data structure
                format_data(query_set);
                     
                break;
                
                case 8:

                SQL_QUERY ="SELECT Physician.Name "
                + "FROM Physician "
                + "INNER JOIN Undergoes ON Undergoes.Physician = Physician.EmployeeID "
                + "WHERE (Undergoes.Physician , Undergoes.Procedures ) NOT IN (SELECT Physician, Treatment FROM Trained_in);";
      
                query_output = mysql_connector.executeQuery(SQL_QUERY); 

                if(!query_output.isBeforeFirst() && !query_output.isAfterLast()) 
                {
                    System.out.print("MYSQL Query has returned empty set !\n!");
                    break;
                }

                //flush out the existing content of the arrays
                query_set.clear();
                query_row.clear();

                //add header row
                query_row.add("Physician's Name");

                //push this header row to table
                query_set.add(new ArrayList<>(query_row));

                //iterate over all rows received as output of the query
                while(query_output.next())  
                {
                   query_row.clear();
                   //add the row to the table
                   query_row.add(query_output.getString(1));
                   query_set.add(new ArrayList<>(query_row));
                }
                //print the content stored in query_set data structure
                format_data(query_set);
                break;

                case 9:

                SQL_QUERY ="WITH train AS ( "
                + "SELECT p.Name, p.EmployeeID as te, Trained_in.Treatment as a, Trained_in.CertificationExpires as ce "
                + "From Trained_in "
                + "INNER JOIN Physician p ON p.EmployeeID = Trained_in.Physician "
                + ") "
                + "SELECT distinct t.Name "
                + "FROM Physician "
                + "INNER JOIN Undergoes u ON u.Physician = Physician.EmployeeID "
                + "INNER JOIN train t ON t.te = u. Physician and t.a = u.Procedures and DATEDIFF(u.Date, t.ce)>0;";
    
                query_output = mysql_connector.executeQuery(SQL_QUERY); 

                if(!query_output.isBeforeFirst() && !query_output.isAfterLast()) 
                {
                    System.out.print("MYSQL Query has returned empty set !\n!");
                    break;
                }

                //flush out the existing content of the arrays
                query_set.clear();
                query_row.clear();

                //add header row
                query_row.add("Physicians's Name");

                //push this header row to table
                query_set.add(new ArrayList<>(query_row));

                //iterate over all rows received as output of the query
                while(query_output.next())  
                {
                    query_row.clear();
                    //add the row to the table
                    query_row.add(query_output.getString(1));
                    query_set.add(new ArrayList<>(query_row));
                }
                //print the content stored in query_set data structure
                format_data(query_set);    
                break;

                case 10:
                SQL_QUERY ="WITH train AS ( "
                +"    SELECT p.Name, p.EmployeeID as te, Trained_in.Treatment as a, Trained_in.CertificationExpires as ce "
                +"    From Trained_in "
                +"    INNER JOIN Physician p ON p.EmployeeID = Trained_in.Physician "
                +") "
                +"SELECT t.Name as Physician, pr.Name, u.Date, pa.Name as Patient "
                +"FROM Physician "
                +"INNER JOIN Undergoes u ON u.Physician = Physician.EmployeeID "
                +"INNER JOIN train t ON t.te = u. Physician and t.a = u.Procedures and DATEDIFF(u.Date, t.ce)>0 "
                +"INNER JOIN Patient pa ON pa.SSN = u.patient "
                +"INNER JOIN Procedures pr ON u.Procedures = pr.Code;";
                query_output = mysql_connector.executeQuery(SQL_QUERY); 
                if(!query_output.isBeforeFirst() && !query_output.isAfterLast()) 
                {
                    System.out.print("MYSQL Query has returned empty set !\n!");
                    break;
                }

                //flush out the existing content of the arrays
                query_set.clear();
                query_row.clear();

                //add header row
                query_row.add("Physicians's Name");
                query_row.add("Procedure Name");
                query_row.add("Procedure Date");
                query_row.add("Patient's Name");

                //push this header row to table
                query_set.add(new ArrayList<>(query_row));

                //iterate over all rows received as output of the query
                while(query_output.next())  
                {
                    query_row.clear();
                    //add the row to the table
                    query_row.add(query_output.getString(1));
                    query_row.add(query_output.getString(2));
                    query_row.add(query_output.getString(3));
                    query_row.add(query_output.getString(4));
                    query_set.add(new ArrayList<>(query_row));
                }
                //print the content stored in query_set data structure
                format_data(query_set);  
                  
                break;

                case 11:

                SQL_QUERY ="WITH pat AS( "
                + "SELECT Patient.SSN, Patient.Name, Patient.PCP, COUNT(Patient.SSN) "
                + "FROM Patient "
                + "INNER JOIN Appointment a ON a.Patient = Patient.SSN "
                + "INNER JOIN Physician ph ON ph.EmployeeID = a.Physician "
                + "INNER JOIN Affiliated_with af ON af.Physician = a.Physician "
                + "INNER JOIN Department d ON af.Department = d.DepartmentID and d.Name=\"Cardiology\" "
                + "GROUP BY Patient.SSN "
                + "HAVING COUNT(Patient.SSN) >= 2 "
                + ") "
                + "SELECT distinct pa.Name, ph.Name as PhysicianName "
                + "FROM pat "
                + "INNER JOIN Patient pa ON pa.SSN = pat.SSN "
                + "INNER JOIN Appointment a ON a.Patient = pa.SSN "
                + "INNER JOIN Affiliated_with af ON af.Physician = a.Physician "
                + "INNER JOIN Department d ON af.Department = d.DepartmentID and d.Head != a.Physician "
                + "INNER JOIN Undergoes u ON u.patient = pat.SSN "
                + "INNER JOIN Procedures p ON p.Code = u.Procedures " 
                + "INNER JOIN Prescribes pr ON  pr.Patient = pat.SSN and pr.Physician = pat.PCP "
                + "INNER JOIN Physician ph ON ph.EmployeeID = pa.PCP "
                + "WHERE p.Cost > 5000;";
                query_output = mysql_connector.executeQuery(SQL_QUERY); 

                if(!query_output.isBeforeFirst() && !query_output.isAfterLast()) 
                {
                    System.out.print("MYSQL Query has returned empty set !\n!");
                    break;
                }

                //flush out whatever was stored in the arrays
                query_set.clear();
                query_row.clear();

                //add header row
                query_row.add("Patients's Name");
                query_row.add("Physician's Name");

                //push this header row to table
                query_set.add(new ArrayList<>(query_row));

                //iterate over all rows received as output of the query
                while(query_output.next())  
                {
                    query_row.clear();
                    //add the row to the table
                    query_row.add(query_output.getString(1));
                    query_row.add(query_output.getString(2));
                    query_set.add(new ArrayList<>(query_row));
                }
                //print the content stored in query_set data structure
                format_data(query_set);     
               
                break;

                case 12:

                SQL_QUERY ="WITH medicine AS ( "
                + "SELECT Medication, COUNT(*) as med_count "
                + "FROM Prescribes "
                + "GROUP BY Medication "
                + ") "
                + "SELECT Name, Brand "
                + "FROM Medication "
                + "INNER JOIN medicine ON Medication.Code = medicine.Medication "
                + "WHERE medicine.med_count = (SELECT MAX(med_count) FROM medicine);";
      
                query_output = mysql_connector.executeQuery(SQL_QUERY); 

                if(!query_output.isBeforeFirst() && !query_output.isAfterLast()) 
                {
                    System.out.print("MYSQL Query has returned empty set !\n!");
                    break;
                }

                //flush out whatever was stored in the arrays
                query_set.clear();
                query_row.clear();

                //add header row
                query_row.add("Medicine's Name");
                query_row.add("Medicine's Brand");

                //push this header row to table
                query_set.add(new ArrayList<>(query_row));

                //iterate over all rows received as output of the query
                while(query_output.next())  
                {
                    query_row.clear();
                    //add the row to the table
                    query_row.add(query_output.getString(1));
                    query_row.add(query_output.getString(2));
                    query_set.add(new ArrayList<>(query_row));
                }
                //print the content stored in query_set data structure
                format_data(query_set);
                break;
                
                case 13:
                stdin_scanner.nextLine();
                System.out.print("Enter a Procedure: "); 
                String b= stdin_scanner.nextLine(); 
                
                
                SQL_QUERY ="SELECT Physician.Name FROM Physician INNER JOIN Trained_in t ON t.Physician = Physician.EmployeeID INNER JOIN Procedures pr ON pr.Code = t.Treatment and pr.Name = '"+b+"';"
                ;
                
                query_output = mysql_connector.executeQuery(SQL_QUERY); 


                if(!query_output.isBeforeFirst() && !query_output.isAfterLast()) 
                {
                    System.out.print("MYSQL Query has returned empty set !!\n");
                    break;
                }
                
                //flush out whatever was stored in the arrays
                query_set.clear();
                query_row.clear();

                //add header row
                query_row.add("Physician's Name");

                //push this header row to table
                query_set.add(new ArrayList<>(query_row));

                //iterate over all rows received as output of the query
                while(query_output.next())  
                {
                    query_row.clear();
                    //add the row to the table
                    query_row.add(query_output.getString(1));
                    query_set.add(new ArrayList<>(query_row));
                }
                //print the content stored in query_set data structure
                format_data(query_set);
                break;

                default:
                System.out.print("Invalid Input"); 
                  // code block
              }
              if(flag==1)break;
              
            }
            stdin_scanner.close(); 
           
        } catch (Exception e) {
            System.out.println(e);
        }
       
    }

}
