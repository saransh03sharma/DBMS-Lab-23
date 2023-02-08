import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.Scanner;
// import java.awt.BorderLayosut;
import java.util.ArrayList;
// import javax.swing.JFrame;
// import javax.swing.JScrollPane;
// import javax.swing.JTable;
// import javax.swing.table.AbstractTableModel;

public class App {

    private static String MYSQL_JDBC_DRIVER_CLASS = "com.mysql.cj.jdbc.Driver";
    private static String MYSQL_DB_URL = "jdbc:mysql://10.5.18.70:3306/20CS10085";
    private static String MYSQL_DB_USER = "20CS10085";
    private static String MYSQL_DB_USER_PASSWORD = "20CS10085";

    public static void printTable(ArrayList<ArrayList<String>> table) {
        int[] columnWidths = new int[table.get(0).size()];
        for (ArrayList<String> row : table) {
            for (int i = 0; i < row.size(); i++) {
                columnWidths[i] = Math.max(columnWidths[i], row.get(i).length());
            }
        }
        for (int i = 0; i < table.get(0).size(); i++) {
            columnWidths[i] += 2;
        }
        StringBuilder divider = new StringBuilder();
        divider.append("|");
        for (int width : columnWidths) {
            for (int i = 0; i <= width; i++) {
                divider.append("-");
            }
            divider.append("|");
        }
        System.out.println(divider.toString());
        for (ArrayList<String> row : table) {
            System.out.print("|");
            for (int i = 0; i < row.size(); i++) {
                System.out.print(" " + row.get(i));
                int padding = columnWidths[i] - row.get(i).length() - 1;
                for (int j = 0; j < padding; j++) {
                    System.out.print(" ");
                }
                System.out.print(" |");
            }
            System.out.println();
            System.out.println(divider.toString());
        }
    }



    public static void main(String[] args) {
            
        try(Connection connection = DriverManager.getConnection(MYSQL_DB_URL,MYSQL_DB_USER,MYSQL_DB_USER_PASSWORD)) {

            Class.forName(MYSQL_JDBC_DRIVER_CLASS); 
            Statement statement =connection.createStatement();  
           
            Scanner sc= new Scanner(System.in);
            System.out.print("Enter query number: ");  
            int a= sc.nextInt(); 
            String SQL_QUERY;
            ResultSet resultSet;
            ArrayList<ArrayList<String>> arr = new ArrayList<>();
            ArrayList<String> row = new ArrayList<>();
            // ArrayList<String> col = new ArrayList<>();
            
            switch(a) {
                case 1:
                SQL_QUERY = "SELECT Physician.Name FROM Physician INNER JOIN Trained_in t ON t.Physician = Physician.EmployeeID INNER JOIN Procedures pr ON pr.Code = t.Treatment and pr.Name = 'Bypass Surgery';";
                resultSet = statement.executeQuery(SQL_QUERY); 
                arr.clear();
                row.clear();
                row.add("Physician Name");
                arr.add(new ArrayList<>(row));
                while(resultSet.next())  {
                    row.clear();
                    row.add(resultSet.getString(1));
                    arr.add(new ArrayList<>(row));
                }
                printTable(arr);
                  break;
                case 2:
                SQL_QUERY = "SELECT p.Name FROM Department " 
                + "INNER JOIN Affiliated_with a ON a.Department = Department.DepartmentID and Department.Name = \'Cardiology\' "
                + "INNER JOIN Physician p ON p.EmployeeID=a.Physician "
                + "INNER JOIN Trained_in t ON t.Physician = p.EmployeeID "
                + "INNER JOIN Procedures pr ON pr.Code = t.Treatment and pr.Name = \'Bypass Surgery\';";
                resultSet = statement.executeQuery(SQL_QUERY); 

                arr.clear();
                row.clear();
                row.add("Physician Name");
                arr.add(new ArrayList<>(row));
                while(resultSet.next())  {
                    row.clear();
                    row.add(resultSet.getString(1));
                    arr.add(new ArrayList<>(row));
                }
                printTable(arr);     
                break;
                case 3:
                SQL_QUERY ="SELECT Nurse.Name "
                + "FROM  Nurse "
                + "INNER JOIN On_Call n ON n.Nurse = Nurse.EmployeeID "
                + "INNER JOIN Room ON Room.BlockCode = n.BlockCode and Room.BlockFloor = n.BlockFloor and Room.Number = 123;"; 
                resultSet = statement.executeQuery(SQL_QUERY); 

                arr.clear();
                row.clear();
                row.add("Nurse Name");
                arr.add(new ArrayList<>(row));
                while(resultSet.next())  {
                    row.clear();
                    row.add(resultSet.getString(1));
                    arr.add(new ArrayList<>(row));
                }
                printTable(arr);   
                break;
                case 4:
                SQL_QUERY ="SELECT Name \"Patient Name\",Address \"Patient Address\" "+
                "FROM Patient "+
                "WHERE SSN IN (SELECT Patient "+
                                    "FROM Prescribes "+
                                    "WHERE Medication IN (SELECT Code "+
                                                    "FROM Medication "+
                                                    "WHERE Name = 'Remdesivir'));";
                resultSet = statement.executeQuery(SQL_QUERY); 

                arr.clear();
                row.clear();
                row.add("Patient Name");
                row.add("Patient Address");
                arr.add(new ArrayList<>(row));
                while(resultSet.next())  {
                    row.clear();
                    row.add(resultSet.getString(1));
                    row.add(resultSet.getString(2));
                    arr.add(new ArrayList<>(row));
                }
                printTable(arr);    
                break;
                case 5:

                SQL_QUERY ="SELECT p.Name, p.InsuranceID "
                + "FROM Room "
                + "INNER JOIN Stay s ON s.Room = Room.Number and Room.Type = \"ICU\" "
                + "INNER JOIN Patient p ON p.SSN = s.Patient and  DATEDIFF(s.End, s.Start)>15;";
                resultSet = statement.executeQuery(SQL_QUERY); 

                arr.clear();
                row.clear();
                row.add("Patient Name");
                row.add("Patient InsuranceID");
                arr.add(new ArrayList<>(row));
                while(resultSet.next())  {
                    row.clear();
                    row.add(resultSet.getString(1));
                    row.add(resultSet.getString(2));
                    arr.add(new ArrayList<>(row));
                }
                printTable(arr);  
                break;
                
                case 6:

                SQL_QUERY ="SELECT n.Name "
                + "FROM Procedures "
                + "INNER JOIN Undergoes u ON u.Procedures = Procedures.Code and Procedures.Name = \"Bypass Surgery\" "
                + "INNER JOIN Nurse n ON n.EmployeeID = u.AssistingNurse;";
                resultSet = statement.executeQuery(SQL_QUERY); 

                arr.clear();
                row.clear();
                row.add("Nurse Name");
                arr.add(new ArrayList<>(row));
                while(resultSet.next())  {
                    row.clear();
                    row.add(resultSet.getString(1));
                    arr.add(new ArrayList<>(row));
                }
                printTable(arr);      
                break;
                
                case 7:

                SQL_QUERY ="SELECT n.Name as Nurse, n.Position, p.Name as Physician "
                + "FROM Procedures "
                + "INNER JOIN Undergoes u ON u.Procedures = Procedures.Code and Procedures.Name = \"Bypass Surgery\" "
                + "INNER JOIN Nurse n ON n.EmployeeID = u.AssistingNurse "
                + "INNER JOIN Physician p ON p.EmployeeID = u.Physician; ";
                resultSet = statement.executeQuery(SQL_QUERY); 

                arr.clear();
                row.clear();
                row.add("Nurse Name");
                row.add("Nurse Position");
                row.add("Physician Name");
                arr.add(new ArrayList<>(row));
                while(resultSet.next())  {
                    row.clear();
                    row.add(resultSet.getString(1));
                    row.add(resultSet.getString(2));
                    row.add(resultSet.getString(3));
                    arr.add(new ArrayList<>(row));
                }
                printTable(arr);     
                break;
                
                case 8:

                SQL_QUERY ="SELECT Physician.Name "
                + "FROM Physician "
                + "INNER JOIN Undergoes ON Undergoes.Physician = Physician.EmployeeID "
                + "WHERE (Undergoes.Physician , Undergoes.Procedures ) NOT IN (SELECT Physician, Treatment FROM Trained_in);";
      
                resultSet = statement.executeQuery(SQL_QUERY); 

                arr.clear();
                row.clear();
                row.add("Physician Name");
                arr.add(new ArrayList<>(row));
                while(resultSet.next())  {
                    row.clear();
                    row.add(resultSet.getString(1));
                    arr.add(new ArrayList<>(row));
                }
                printTable(arr);     
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
    
                resultSet = statement.executeQuery(SQL_QUERY); 

                arr.clear();
                row.clear();
                row.add("Physician Name");
                arr.add(new ArrayList<>(row));
                while(resultSet.next())  {
                    row.clear();
                    row.add(resultSet.getString(1));
                    arr.add(new ArrayList<>(row));
                }
                printTable(arr);    
                break;

                case 10:

                SQL_QUERY ="SELECT P.Name \"Physician Name\",PR.Name \"Procedure Name\",U.Date \"Procedure Date\",PA.Name \"Patient Name\" "+
                "FROM ((((Undergoes U "+
                "INNER JOIN Trained_In T "+
                "ON U.Physician = T.Physician AND U.Procedure = T.Treatment AND DATEDIFF(U.Date,T.CertificationExpires)>0) "+
                "INNER JOIN Procedures PR ON U.Procedure = PR.Code) "+
                "INNER JOIN Patient PA ON U.Patient = PA.SSN) "+
                "INNER JOIN Physician P ON U.Physician = P.EmployeeID);";
                resultSet = statement.executeQuery(SQL_QUERY); 

                arr.clear();
                row.clear();
                row.add("Physician Name");
                row.add("Procedure Name");
                row.add("Procedure Date");
                row.add("Patient Name");
                arr.add(new ArrayList<>(row));
                while(resultSet.next())  {
                    row.clear();
                    row.add(resultSet.getString(1));
                    row.add(resultSet.getString(2));
                    row.add(resultSet.getString(3));
                    row.add(resultSet.getString(4));
                    arr.add(new ArrayList<>(row));
                }
                printTable(arr);     
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
                resultSet = statement.executeQuery(SQL_QUERY); 

                arr.clear();
                row.clear();
                row.add("Patient Name");
                row.add("Physician Name");
                arr.add(new ArrayList<>(row));
                while(resultSet.next())  {
                    row.clear();
                    row.add(resultSet.getString(1));
                    row.add(resultSet.getString(2));
                    arr.add(new ArrayList<>(row));
                }
                printTable(arr); 
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
      
                resultSet = statement.executeQuery(SQL_QUERY); 

                arr.clear();
                row.clear();
                row.add("Medicine Name");
                row.add("Medicine Brand");
                arr.add(new ArrayList<>(row));
                while(resultSet.next())  {
                    row.clear();
                    row.add(resultSet.getString(1));
                    row.add(resultSet.getString(2));
                    arr.add(new ArrayList<>(row));
                }
                printTable(arr); 
                break;
                
                case 13:
                sc.nextLine();
                System.out.print("Enter a Procedure: "); 
                String b= sc.nextLine(); 
                
                
                SQL_QUERY ="SELECT Physician.Name FROM Physician INNER JOIN Trained_in t ON t.Physician = Physician.EmployeeID INNER JOIN Procedures pr ON pr.Code = t.Treatment and pr.Name = '"+b+"';"
                ;
                
                
                resultSet = statement.executeQuery(SQL_QUERY); 

                arr.clear();
                row.clear();
                row.add("Physician Name");
                arr.add(new ArrayList<>(row));
                while(resultSet.next())  {
                    row.clear();
                    row.add(resultSet.getString(1));
                    arr.add(new ArrayList<>(row));
                }
                printTable(arr);  
                break;

                default:
                System.out.print("Invalid Input"); 
                  // code block
              }
           
           
               
            sc.close();
        } catch (Exception e) {
            System.out.println(e);
        }
       
    }

}
