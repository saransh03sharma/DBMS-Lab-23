import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.Scanner;

public class App {

    private static String MYSQL_JDBC_DRIVER_CLASS = "com.mysql.cj.jdbc.Driver";
    private static String MYSQL_DB_URL = "jdbc:mysql://10.5.18.72:3306/20CS30065";
    private static String MYSQL_DB_USER = "20CS30065";
    private static String MYSQL_DB_USER_PASSWORD = "20CS30065";



    public static void main(String[] args) {
            
        try(Connection connection = DriverManager.getConnection(MYSQL_DB_URL,MYSQL_DB_USER,MYSQL_DB_USER_PASSWORD)) {

            Class.forName(MYSQL_JDBC_DRIVER_CLASS); 
            Statement statement =connection.createStatement();  
           
            Scanner sc= new Scanner(System.in);
            System.out.print("Enter first number- ");  
            int a= sc.nextInt(); 
            String SQL_QUERY;
            ResultSet resultSet;
            
            switch(a) {
                case 1:
                SQL_QUERY = "SELECT Physician.Name FROM Physician INNER JOIN Trained_in t ON t.Physician = Physician.EmployeeID INNER JOIN Procedures pr ON pr.Code = t.Treatment and pr.Name = 'Bypass Surgery';";
                resultSet = statement.executeQuery(SQL_QUERY); 

                System.out.println("Physician Name");
                while(resultSet.next())  {
                    System.out.println(resultSet.getString(1));
                }     
                  break;
                case 2:
                SQL_QUERY = "SELECT p.Name FROM Department " 
                + "INNER JOIN Affiliated_with a ON a.Department = Department.DepartmentID and Department.Name = \'Cardiology\' "
                + "INNER JOIN Physician p ON p.EmployeeID=a.Physician "
                + "INNER JOIN Trained_in t ON t.Physician = p.EmployeeID "
                + "INNER JOIN Procedures pr ON pr.Code = t.Treatment and pr.Name = \'Bypass Surgery\';";
                resultSet = statement.executeQuery(SQL_QUERY); 

                System.out.println("Physician Name");
                while(resultSet.next())  {
                    System.out.println(resultSet.getString(1));
                }     
                break;
                case 3:
                SQL_QUERY ="SELECT Nurse.Name "
                + "FROM  Nurse "
                + "INNER JOIN On_Call n ON n.Nurse = Nurse.EmployeeID "
                + "INNER JOIN Room ON Room.BlockCode = n.BlockCode and Room.BlockFloor = n.BlockFloor and Room.Number = 123;"; 
                resultSet = statement.executeQuery(SQL_QUERY); 

                System.out.println("Nurse Name");
                while(resultSet.next())  {
                    System.out.println(resultSet.getString(1));
                }     
                break;
                case 4:
                SQL_QUERY ="SELECT pa.Name, pa.Address "
                + "FROM Medication "
                + "INNER JOIN Prescribes p ON p.Medication = Medication.Code and Medication.Name='Remdesivir' "
                + "INNER JOIN Patient pa ON pa.SSN = p.Patient;";
                resultSet = statement.executeQuery(SQL_QUERY); 

                System.out.println("Patient Name" + " ".repeat(18) + "Patient Address");
                while(resultSet.next())  {
                    System.out.println(resultSet.getString(1)+ " ".repeat(30-(resultSet.getString(1)).length()) + resultSet.getString(2));
                }     
                break;
                case 5:

                SQL_QUERY ="SELECT p.Name, p.InsuranceID "
                + "FROM Room "
                + "INNER JOIN Stay s ON s.Room = Room.Number and Room.Type = \"ICU\" "
                + "INNER JOIN Patient p ON p.SSN = s.Patient and  DATEDIFF(s.End, s.Start)>15;";
                resultSet = statement.executeQuery(SQL_QUERY); 

               System.out.println("Patient Name" + " ".repeat(18) + "Patient Insurance ID");
                while(resultSet.next())  {
                    System.out.println(resultSet.getString(1)+ " ".repeat(30-(resultSet.getString(1)).length()) + resultSet.getInt(2));
                }     
                break;
                
                case 6:

                SQL_QUERY ="SELECT n.Name "
                + "FROM Procedures "
                + "INNER JOIN Undergoes u ON u.Procedures = Procedures.Code and Procedures.Name = \"Bypass Surgery\" "
                + "INNER JOIN Nurse n ON n.EmployeeID = u.AssistingNurse;";
                resultSet = statement.executeQuery(SQL_QUERY); 

               System.out.println("Nurse Name");
                while(resultSet.next())  {
                    System.out.println(resultSet.getString(1));
                }     
                break;
                
                case 7:

                SQL_QUERY ="SELECT n.Name as Nurse, n.Position, p.Name as Physician "
                + "FROM Procedures "
                + "INNER JOIN Undergoes u ON u.Procedures = Procedures.Code and Procedures.Name = \"Bypass Surgery\" "
                + "INNER JOIN Nurse n ON n.EmployeeID = u.AssistingNurse "
                + "INNER JOIN Physician p ON p.EmployeeID = u.Physician; ";
                resultSet = statement.executeQuery(SQL_QUERY); 

                System.out.println("Nurse Name" + " ".repeat(20) + "Nurse Position" + " ".repeat(16) + "Physician Name");
                while(resultSet.next())  {
                    System.out.println(resultSet.getString(1)+ " ".repeat(30-(resultSet.getString(1)).length()) + resultSet.getString(2)+ " ".repeat(30-(resultSet.getString(2)).length()) + resultSet.getString(3));
                }     
                break;
                
                case 8:

                SQL_QUERY ="SELECT Physician.Name "
                + "FROM Physician "
                + "INNER JOIN Undergoes ON Undergoes.Physician = Physician.EmployeeID "
                + "WHERE (Undergoes.Physician , Undergoes.Procedures ) NOT IN (SELECT Physician, Treatment FROM Trained_in);";
      
                resultSet = statement.executeQuery(SQL_QUERY); 

               System.out.println("Physician Name");
                while(resultSet.next())  {
                    System.out.println(resultSet.getString(1));
                }     
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

               System.out.println("Physician Name");
                while(resultSet.next())  {
                    System.out.println(resultSet.getString(1));
                }     
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
                resultSet = statement.executeQuery(SQL_QUERY); 

               System.out.println("Physician Name"+ " ".repeat(25 - "Procedure Name".length()) + "Date"+ " ".repeat(25 - "Date".length())+ "Patient Name");
                while(resultSet.next())  {
                    System.out.println(resultSet.getString(1)+ " ".repeat(25-(resultSet.getString(1)).length()) + resultSet.getString(2)+ " ".repeat(25-(resultSet.getString(2)).length()) + resultSet.getString(3)+ " ".repeat(25-(resultSet.getString(4)).length()));
                }     
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

                System.out.println("Patient Name" + " ".repeat(18) + "Physician Name");
                while(resultSet.next())  {
                    System.out.println(resultSet.getString(1)+ " ".repeat(30-(resultSet.getString(1)).length()) + resultSet.getString(2));
                }     
               
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

                System.out.println("Medicine Name" + " ".repeat(17) + "Medicine Brand");
                while(resultSet.next())  {
                    System.out.println(resultSet.getString(1)+ " ".repeat(30-(resultSet.getString(1)).length()) + resultSet.getString(2));
                }
                break;
                
                case 13:
                sc.nextLine();
                System.out.print("Enter a Procedure: "); 
                String b= sc.nextLine(); 
                
                
                SQL_QUERY ="SELECT Physician.Name FROM Physician INNER JOIN Trained_in t ON t.Physician = Physician.EmployeeID INNER JOIN Procedures pr ON pr.Code = t.Treatment and pr.Name = '"+b+"';"
                ;
                
                
                resultSet = statement.executeQuery(SQL_QUERY); 

                System.out.println("Physician Name");
                while(resultSet.next())  {
                    System.out.println(resultSet.getString(1));
                }  
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
