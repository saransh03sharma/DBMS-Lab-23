SELECT Name "Physician Name"
FROM Physician
WHERE EmployeeID IN (SELECT Physician
                     FROM Trained_In
                     WHERE Treatment = (SELECT Code
                                        FROM Procedures
                                        WHERE Name = "Bypass Surgery"));


SELECT Name "Physician Name"
FROM Physician
WHERE EmployeeID IN (SELECT Physician
                     FROM Trained_In
                     WHERE Treatment = (SELECT Code
                                        FROM Procedures
                                        WHERE Name = "Bypass Surgery") AND Physician IN (SELECT Physician
                                                                                          FROM Affiliated_With
                                                                                          WHERE Department = (SELECT DepartmentID
                                                                                                               FROM Department
                                                                                                               WHERE Name = "Cardiology"))); 


SELECT Name "Nurse Name"
FROM Nurse
WHERE EmployeeID IN (SELECT Nurse
                    FROM On_Call
                    WHERE (BlockFloor,BlockCode) IN (SELECT BlockFloor,BlockCode
                                                    FROM Room
                                                    WHERE Number = 123));


SELECT Name "Patient Name",Address "Patient Address"
FROM Patient
WHERE SSN IN (SELECT Patient
                    FROM Prescribes
                    WHERE Medication IN (SELECT Code
                                    FROM Medication
                                    WHERE Name = 'Remdesivir'));


SELECT Name "Patient Name",InsuranceID "Patient Insurance ID"
FROM Patient
WHERE SSN IN (SELECT Patient
                    FROM Stay
                    WHERE Room IN (SELECT Number
                                    FROM Room
                                    WHERE Type = 'ICU') AND DATEDIFF(`End`,Start)>15);


SELECT Name "Nurse Name"
FROM Nurse
WHERE EmployeeID IN (SELECT AssistingNurse
                    FROM Undergoes U
                    WHERE U.Procedure = (SELECT Code
                                    FROM Procedures
                                    WHERE Name = "Bypass Surgery"));


SELECT N.Name 'Nurse Name',N.Position 'Nurse Position',P.Name 'Physician Name'
FROM Physician P,Nurse N
WHERE (P.EmployeeID,N.EmployeeID) IN (SELECT Physician,AssistingNurse
                    FROM Undergoes
                    WHERE Undergoes.Procedure = (SELECT Code
                                    FROM Procedures
                                    WHERE Name = "Bypass Surgery"));


SELECT Name "Physician Name"
FROM Physician
WHERE EmployeeID IN (SELECT Physician
        FROM Undergoes U
        WHERE (Physician,U.Procedure) NOT IN (SELECT Physician,Treatment
                                    FROM Trained_In));


SELECT P.Name "Physician Name"
FROM ((Undergoes U
INNER JOIN Trained_In T        
ON U.Physician = T.Physician AND U.Procedure = T.Treatment AND DATEDIFF(U.Date,T.CertificationExpires)>0)
INNER JOIN Physician P ON U.Physician = P.EmployeeID);


SELECT P.Name "Physician Name",PR.Name "Procedure Name",U.Date "Procedure Date",PA.Name "Patient Name"
FROM ((((Undergoes U
INNER JOIN Trained_In T        
ON U.Physician = T.Physician AND U.Procedure = T.Treatment AND DATEDIFF(U.Date,T.CertificationExpires)>0)
INNER JOIN Procedures PR ON U.Procedure = PR.Code)
INNER JOIN Patient PA ON U.Patient = PA.SSN)
INNER JOIN Physician P ON U.Physician = P.EmployeeID);


SELECT PA.Name "Patient's Name",P.Name "Physician's Name"
FROM (((SELECT Patient,Physician
        FROM Undergoes
        WHERE (Patient,Physician) IN (SELECT Patient,Physician
                                        FROM Prescribes
                                        WHERE (Patient,Physician) IN (SELECT Patient,Physician
                                                                    FROM Appointment
                                                                    WHERE Physician IN (SELECT Physician
                                                                                        FROM Affiliated_With
                                                                                        WHERE Department = (SELECT DepartmentID
                                                                                                            FROM Department
                                                                                                            WHERE Name = "Cardiology") AND Physician NOT IN (SELECT Head
                                                                                                                                                            FROM Department)
                                                                                        )
                                                                    GROUP BY Patient,Physician
                                                                    HAVING COUNT(*)>=2
                                                                    )
                                        GROUP BY Patient,Physician
                                        HAVING COUNT(*)>=1
                                        ) AND `Procedure` IN (SELECT Code
                                                            FROM Procedures
                                                            WHERE Cost>5000)
        GROUP BY Patient,Physician
        HAVING COUNT(*)>=1) AS A
INNER JOIN Physician P ON A.Physician = P.EmployeeID)
INNER JOIN Patient PA ON A.Patient = PA.SSN);


SELECT Name "Medication Name",Brand "Medication Brand"
FROM Medication
WHERE Code = (SELECT Medication
                FROM Prescribes
                GROUP BY Medication
                ORDER BY COUNT(*) DESC
                LIMIT 1);
