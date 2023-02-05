import datetime
import mysql.connector

cnx = mysql.connector.connect(host='10.5.18.72',
                        database='20CS30065',
                        user='20CS30065',
                        password='20CS30065')
cursor = cnx.cursor()

input_a = input("Enter a Query Number: ")
input_a = int(input_a)

match input_a:
        case 1:
            query = ("SELECT Physician.Name FROM Physician INNER JOIN Trained_in t ON t.Physician = Physician.EmployeeID INNER JOIN Procedures pr ON pr.Code = t.Treatment and pr.Name = 'Bypass Surgery';")
            cursor.execute(query)
            print("Physician Name")
            for (Name) in cursor:
                print(Name[0])
            
        case 2:
            query = """SELECT p.Name
            FROM Department INNER JOIN Affiliated_with a ON a.Department = Department.DepartmentID and Department.Name = 'Cardiology'
            INNER JOIN Physician p ON p.EmployeeID=a.Physician
            INNER JOIN Trained_in t ON t.Physician = p.EmployeeID 
            INNER JOIN Procedures pr ON pr.Code = t.Treatment and pr.Name = 'Bypass Surgery';"""
            cursor.execute(query)
            print("Physician Name")
            for (Name) in cursor:
                print(Name[0])
        case 3:
            query = """ SELECT Nurse.Name
            FROM  Nurse
            INNER JOIN On_Call n ON n.Nurse = Nurse.EmployeeID
            INNER JOIN Room ON Room.BlockCode = n.BlockCode and Room.BlockFloor = n.BlockFloor and Room.Number = 123;"""    
            cursor.execute(query)
            print("Nurse Name")
            for (Name) in cursor:
                print(Name[0])
            
        case 4:
            query = """SELECT pa.Name, pa.Address
            FROM Medication
            INNER JOIN Prescribes p ON p.Medication = Medication.Code and Medication.Name='Remdesivir'
            INNER JOIN Patient pa ON pa.SSN = p.Patient;"""
            cursor.execute(query)
            print("Patient Name"+" "*(30-len("Patient Name"))+"Patient Address")
            for (Name) in cursor:
                print(Name[0]+" "*(30-len(Name[0]))+Name[1])
        
        case 5:   
            query = """SELECT p.Name, p.InsuranceID
            FROM Room
            INNER JOIN Stay s ON s.Room = Room.Number and Room.Type = "ICU"
            INNER JOIN Patient p ON p.SSN = s.Patient and  DATEDIFF(s.End, s.Start)>15;"""
            cursor.execute(query)
            print("Patient Name"+" "*(30-len("Patient Name"))+"Patient Insurance ID")
            for (Name) in cursor:
                print(Name[0]+" "*(30-len(Name[0]))+str(Name[1]))
            
        case 6:
            query = """SELECT n.Name
            FROM Procedures
            INNER JOIN Undergoes u ON u.Procedures = Procedures.Code and Procedures.Name = "Bypass Surgery"
            INNER JOIN Nurse n ON n.EmployeeID = u.AssistingNurse;"""
            cursor.execute(query)
            print("Nurse Name")
            for (Name) in cursor:
                print(Name[0])
        
        case 7:
            query = """SELECT n.Name as Nurse, n.Position, p.Name as Physician
            FROM Procedures
            INNER JOIN Undergoes u ON u.Procedures = Procedures.Code and Procedures.Name = "Bypass Surgery"
            INNER JOIN Nurse n ON n.EmployeeID = u.AssistingNurse
            INNER JOIN Physician p ON p.EmployeeID = u.Physician; """
            cursor.execute(query)
            print("Nurse Name"+" "*(25-len("Nurse Name"))+"Nurse Position" + " "*(20 -len("Nurse Position"))+ "Physician")
            for (Name) in cursor:
                print(Name[0] + " "*(25-len(Name[0])) + Name[1] + " "*(20-len(Name[1])) + Name[2])
            
        case 8:
            query = """ SELECT Physician.Name
            FROM Physician
            INNER JOIN Undergoes ON Undergoes.Physician = Physician.EmployeeID
            WHERE (Undergoes.Physician , Undergoes.Procedures ) NOT IN (SELECT Physician, Treatment FROM Trained_in);"""
            cursor.execute(query)
            print("Physician Name")
            for (Name) in cursor:
                print(Name[0])
        
        case 9:
            query = """WITH train AS (
            SELECT p.Name, p.EmployeeID as te, Trained_in.Treatment as a, Trained_in.CertificationExpires as ce
            From Trained_in
            INNER JOIN Physician p ON p.EmployeeID = Trained_in.Physician
            )
            SELECT t.Name
            FROM Physician
            INNER JOIN Undergoes u ON u.Physician = Physician.EmployeeID
            INNER JOIN train t ON t.te = u. Physician and t.a = u.Procedures and DATEDIFF(u.Date, t.ce)>0;"""
            print("Physician Name")
            cursor.execute(query)
            for (Name) in cursor:
                print(Name[0])
        
        case 10:
            query = """WITH train AS (
                SELECT p.Name, p.EmployeeID as te, Trained_in.Treatment as a, Trained_in.CertificationExpires as ce
                From Trained_in
                INNER JOIN Physician p ON p.EmployeeID = Trained_in.Physician
            )
            SELECT t.Name as Physician, pr.Name, u.Date, pa.Name as Patient
            FROM Physician
            INNER JOIN Undergoes u ON u.Physician = Physician.EmployeeID
            INNER JOIN train t ON t.te = u. Physician and t.a = u.Procedures and DATEDIFF(u.Date, t.ce)>0
            INNER JOIN Patient pa ON pa.SSN = u.patient
            INNER JOIN Procedures pr ON u.Procedures = pr.Code;"""
            print("Physician Name"+" "*(30-len("Physician Name"))+"Procedure Name" + " "*6+"Date"+" "*26 +"Patient Name")
            cursor.execute(query)
            for (Name) in cursor:
                print(Name[0]+" "*(30-len(Name[0]))+Name[1]+" "*(20-len(Name[1]))+str(Name[2])+" "*(30-len(str(Name[2])))+Name[3])
            
        
        case 11:
            query = """WITH pat AS(
            SELECT Patient.SSN, Patient.Name, Patient.PCP, COUNT(Patient.SSN)
            FROM Patient
            INNER JOIN Appointment a ON a.Patient = Patient.SSN
            INNER JOIN Physician ph ON ph.EmployeeID = a.Physician
            INNER JOIN Affiliated_with af ON af.Physician = a.Physician
            INNER JOIN Department d ON af.Department = d.DepartmentID and d.Name="Cardiology"
            GROUP BY Patient.SSN
            HAVING COUNT(Patient.SSN) >= 2
            )
            SELECT distinct pa.Name, ph.Name as PhysicianName
            FROM pat
            INNER JOIN Patient pa ON pa.SSN = pat.SSN
            INNER JOIN Appointment a ON a.Patient = pa.SSN
            INNER JOIN Affiliated_with af ON af.Physician = a.Physician
            INNER JOIN Department d ON af.Department = d.DepartmentID and d.Head != a.Physician 
            INNER JOIN Undergoes u ON u.patient = pat.SSN 
            INNER JOIN Procedures p ON p.Code = u.Procedures 
            INNER JOIN Prescribes pr ON  pr.Patient = pat.SSN and pr.Physician = pat.PCP
            INNER JOIN Physician ph ON ph.EmployeeID = pa.PCP
            WHERE p.Cost > 5000;"""
            cursor.execute(query)
            print("Patient's Name"+" "*(25-len("Patient's Name"))+"Patient's Physician")
            for (Name) in cursor:
                print(Name[0] + " "*(25-len(Name[0])) + Name[1] )
            
    
        case 12:
            query = """WITH medicine AS (
                SELECT Medication, COUNT(*) as med_count
                FROM Prescribes
                GROUP BY Medication
            )
            SELECT Name, Brand
            FROM Medication
            INNER JOIN medicine ON Medication.Code = medicine.Medication
            WHERE medicine.med_count = (SELECT MAX(med_count) FROM medicine);"""
            cursor.execute(query)
            print("Medicine's Name"+" "*(25-len("Medicine's Name"))+"Medicine Brand")
            for (Name) in cursor:
                print(Name[0] + " "*(25-len(Name[0])) + Name[1] )
        case 13:
                    
            input_a = input("Enter a Procedure: ")
            query = ("SELECT Physician.Name FROM Physician INNER JOIN Trained_in t ON t.Physician = Physician.EmployeeID INNER JOIN Procedures pr ON pr.Code = t.Treatment and pr.Name = '"+input_a+"';")
            cursor.execute(query)
            print("Physician Name")
            for (Name) in cursor:
                print(Name[0])
             
            
        case default:
            print("Invalid query number.")

cursor.close()
cnx.close()