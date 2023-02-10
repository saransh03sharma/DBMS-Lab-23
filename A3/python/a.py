import mysql.connector
from prettytable import PrettyTable

def format_data(data):
    #pretty table library used to print text in table format
    #the first entry in data would be header
    data_table = PrettyTable(data[0])
    
    #the other elements of data would correspond to one row
    for row in data[1:]:
        data_table.add_row(row)
    print(data_table)

def query(mysql_cursor):
    print("Press -1 to Exit")
    input_a = input("Enter a Query Number: ")
    input_a = int(input_a)

    match input_a:
        case (-1):
                #to exit from infinite loop
                print("Exiting...")
                return -1
        case 1:
            query = ("SELECT Physician.Name FROM Physician INNER JOIN Trained_in t ON t.Physician = Physician.EmployeeID INNER JOIN Procedures pr ON pr.Code = t.Treatment and pr.Name = 'Bypass Surgery';")
            #execute the query
            mysql_cursor.execute(query)
            print("\n")
            
            result = list(mysql_cursor)
            
            #check if the query is empty or not
            if(len(result)>0):
                query_output = []
                
                #add the header row
                query_output.append([ "Physician's Name" ])
                
                #add the output of query
                query_output.extend(result)
                format_data(query_output)
                print("\n")
            else:
                print("Query returned empty set!")
                print("\n")
            
        case 2:
            query = """SELECT p.Name
            FROM Department INNER JOIN Affiliated_with a ON a.Department = Department.DepartmentID and Department.Name = 'Cardiology'
            INNER JOIN Physician p ON p.EmployeeID=a.Physician
            INNER JOIN Trained_in t ON t.Physician = p.EmployeeID 
            INNER JOIN Procedures pr ON pr.Code = t.Treatment and pr.Name = 'Bypass Surgery';"""
            #execute the query
            mysql_cursor.execute(query)
            print("\n")
            
            result = list(mysql_cursor)
            
            #check if the query is empty or not
            if(len(result)>0):
                query_output = []
                
                #add the header row
                query_output.append([ "Physician's Name" ])
                
                #add the output of query
                query_output.extend(result)
                format_data(query_output)
                print("\n")
            else:
                print("Query returned empty set!")
                print("\n")
                
        case 3:
            query = """ SELECT Nurse.Name
            FROM  Nurse
            INNER JOIN On_Call n ON n.Nurse = Nurse.EmployeeID
            INNER JOIN Room ON Room.BlockCode = n.BlockCode and Room.BlockFloor = n.BlockFloor and Room.Number = 123;"""    
           #execute the query
            mysql_cursor.execute(query)
            print("\n")
            
            result = list(mysql_cursor)
            
            #check if the query is empty or not
            if(len(result)>0):
                query_output = []
                
                #add the header row
                query_output.append([ "Nurse's Name" ])
                
                #add the output of query
                query_output.extend(result)
                format_data(query_output)
                print("\n")
            else:
                print("Query returned empty set!")
                print("\n")
            
        case 4:
            query = """SELECT pa.Name, pa.Address
            FROM Medication
            INNER JOIN Prescribes p ON p.Medication = Medication.Code and Medication.Name='Remdesivir'
            INNER JOIN Patient pa ON pa.SSN = p.Patient;"""
            #execute the query
            mysql_cursor.execute(query)
            print("\n")
            
            result = list(mysql_cursor)
            
            #check if the query is empty or not
            if(len(result)>0):
                query_output = []
                
                #add the header row
                query_output.append([ "Patient's Name","Patient's Address" ])
                
                #add the output of query
                query_output.extend(result)
                format_data(query_output)
                print("\n")
            else:
                print("Query returned empty set!")
                print("\n")
        
        case 5:   
            query = """SELECT p.Name, p.InsuranceID
            FROM Room
            INNER JOIN Stay s ON s.Room = Room.Number and Room.Type = "ICU"
            INNER JOIN Patient p ON p.SSN = s.Patient and  DATEDIFF(s.End, s.Start)>15;"""
            #execute the query
            mysql_cursor.execute(query)
            print("\n")
            
            result = list(mysql_cursor)
            
            #check if the query is empty or not
            if(len(result)>0):
                query_output = []
                
                #add the header row
                query_output.append([ "Patient's Name","Insurance ID" ])
                
                #add the output of query
                query_output.extend(result)
                format_data(query_output)
                print("\n")
            else:
                print("Query returned empty set!")
                print("\n")
            
        case 6:
            query = """SELECT n.Name
            FROM Procedures
            INNER JOIN Undergoes u ON u.Procedures = Procedures.Code and Procedures.Name = "Bypass Surgery"
            INNER JOIN Nurse n ON n.EmployeeID = u.AssistingNurse;"""
            #execute the query
            mysql_cursor.execute(query)
            print("\n")
            
            result = list(mysql_cursor)
            
            #check if the query is empty or not
            if(len(result)>0):
                query_output = []
                
                #add the header row
                query_output.append([ "Nurse's Name" ])
                
                #add the output of query
                query_output.extend(result)
                format_data(query_output)
                print("\n")
            else:
                print("Query returned empty set!")
                print("\n")
        
        case 7:
            query = """SELECT n.Name as Nurse, n.Position, p.Name as Physician
            FROM Procedures
            INNER JOIN Undergoes u ON u.Procedures = Procedures.Code and Procedures.Name = "Bypass Surgery"
            INNER JOIN Nurse n ON n.EmployeeID = u.AssistingNurse
            INNER JOIN Physician p ON p.EmployeeID = u.Physician; """
            #execute the query
            mysql_cursor.execute(query)
            print("\n")
            
            result = list(mysql_cursor)
            
            #check if the query is empty or not
            if(len(result)>0):
                query_output = []
                
                #add the header row
                query_output.append([ "Nurse's Name","Nurse's Position","Physician's Name" ])
                
                #add the output of query
                query_output.extend(result)
                format_data(query_output)
                print("\n")
            else:
                print("Query returned empty set!")
                print("\n")
            
            
        case 8:
            query = """ SELECT Distinct Physician.Name
            FROM Physician
            INNER JOIN Undergoes ON Undergoes.Physician = Physician.EmployeeID
            WHERE (Undergoes.Physician , Undergoes.Procedures ) NOT IN (SELECT Physician, Treatment FROM Trained_in);"""
            #execute the query
            mysql_cursor.execute(query)
            print("\n")
            
            result = list(mysql_cursor)
            
            #check if the query is empty or not
            if(len(result)>0):
                query_output = []
                
                #add the header row
                query_output.append([ "Physician's Name" ])
                
                #add the output of query
                query_output.extend(result)
                format_data(query_output)
                print("\n")
            else:
                print("Query returned empty set!")
                print("\n")
        
        case 9:
            query = """WITH  train AS (
            SELECT p.Name, p.EmployeeID as te, Trained_in.Treatment as a, Trained_in.CertificationExpires as ce
            From Trained_in
            INNER JOIN Physician p ON p.EmployeeID = Trained_in.Physician
            )
            SELECT Distinct t.Name
            FROM Physician
            INNER JOIN Undergoes u ON u.Physician = Physician.EmployeeID
            INNER JOIN train t ON t.te = u. Physician and t.a = u.Procedures and DATEDIFF(u.Date, t.ce)>0;"""
            #execute the query
            mysql_cursor.execute(query)
            print("\n")
            
            result = list(mysql_cursor)
            
            #check if the query is empty or not
            if(len(result)>0):
                query_output = []
                
                #add the header row
                query_output.append([ "Physician's Name" ])
                
                #add the output of query
                query_output.extend(result)
                format_data(query_output)
                print("\n")
            else:
                print("Query returned empty set!")
                print("\n")
        
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
            #execute the query
            mysql_cursor.execute(query)
            print("\n")
            
            result = list(mysql_cursor)
            
            #check if the query is empty or not
            if(len(result)>0):
                query_output = []
                
                #add the header row
                query_output.append([ "Physician's Name","Procedure's Name","Procedure's Date","Patient's Name" ])
                
                #add the output of query
                query_output.extend(result)
                format_data(query_output)
                print("\n")
            else:
                print("Query returned empty set!")
                print("\n")
        
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
            #execute the query
            mysql_cursor.execute(query)
            print("\n")
            
            result = list(mysql_cursor)
            
            #check if the query is empty or not
            if(len(result)>0):
                query_output = []
                
                #add the header row
                query_output.append([ "Patient's Name", "Physician's Name" ])
                
                #add the output of query
                query_output.extend(result)
                format_data(query_output)
                print("\n")
            else:
                print("Query returned empty set!")
                print("\n")
            
    
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
            #execute the query
            mysql_cursor.execute(query)
            print("\n")
            
            result = list(mysql_cursor)
            
            #check if the query is empty or not
            if(len(result)>0):
                query_output = []
                
                #add the header row
                query_output.append([ "Medicine's Name", "Medicine's Brand" ])
                
                #add the output of query
                query_output.extend(result)
                format_data(query_output)
                print("\n")
            else:
                print("Query returned empty set!")
                print("\n")
        
        case 13:
                    
            input_a = input("Enter a Procedure: ")
            query = ("SELECT Physician.Name FROM Physician INNER JOIN Trained_in t ON t.Physician = Physician.EmployeeID INNER JOIN Procedures pr ON pr.Code = t.Treatment and pr.Name = '"+input_a+"';")
            #execute the query
            mysql_cursor.execute(query)
            print("\n")
            
            result = list(mysql_cursor)
            
            #check if the query is empty or not
            if(len(result)>0):
                query_output = []
                
                #add the header row
                query_output.append([ "Physician's Name" ])
                
                #add the output of query
                query_output.extend(result)
                format_data(query_output)
                print("\n")
            else:
                print("Query returned empty set!")
                print("\n")
                
             
            
        case default:
            print("Invalid query number.")


def main():
    #function to connect to mysql server
    mysql_con = mysql.connector.connect(host='10.5.18.72',
                                database='20CS30065',
                                user='20CS30065',
                                password='20CS30065')
    #to use mysql functions on the connected database
    mysql_cursor = mysql_con.cursor()
    
    while True:
    #take query number
        if query(mysql_cursor)==-1:
            break
    #close the connection
    mysql_cursor.close()
    mysql_con.close()

if __name__ == "__main__":
    main()

