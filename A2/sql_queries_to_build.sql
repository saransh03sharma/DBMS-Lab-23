---------------Entities----------------
CREATE TABLE Block (
    Floor int NOT NULL,
    Code int NOT NULL,
    CONSTRAINT PK_Block PRIMARY KEY (Floor,Code)
);
INSERT INTO Block
VALUES (0,200),(1,500),(2,400),(3,300),(4,100);
CREATE TABLE Medication (
    Code int NOT NULL,
    Name TINYTEXT NOT NULL,
    Brand TINYTEXT NOT NULL,
    Description TINYTEXT NOT NULL,
    CONSTRAINT PK_Medication PRIMARY KEY (Code)
);
INSERT INTO Medication
VALUES (393620,"Azee-500","St. Mungo's","Antibiotic"),
(406589,"Remdesivir","Flamel Co.","Covid"),
(412368,"Calpol-500","St. Mungo's","Fever"),
(436589,"Valsartan","St. Mungo's","Cardiology")
;
CREATE TABLE Nurse (
    EmployeeID int NOT NULL,
    Name TINYTEXT NOT NULL,
    Position TINYTEXT NOT NULL,
    Registered BOOLEAN NOT NULL,
    SSN int NOT NULL,
    CONSTRAINT PK_Nurse PRIMARY KEY (EmployeeID)
);
INSERT INTO Nurse
VALUES (170023,"Petunia Dursley","Staff Nurse",TRUE,20231522),
(170034,"Nicolas Flamel","Staff Nurse",FALSE,20231527),
(170039,"Cedric Diggory","OT Nurse",TRUE,20231533),
(170045,"Fleur Delacour","OT Nurse",TRUE,20231539),
(170056,"Narcissa Malfoy","Staff Nurse",FALSE,20231542),
(170063,"Bill Weasley","OT Nurse",TRUE,20231551),
(170077,"Poppy Pomfrey","OT Nurse",TRUE,20231557)
;
CREATE TABLE Physician (
    EmployeeID int NOT NULL,
    Name TINYTEXT NOT NULL,
    Position TINYTEXT NOT NULL,
    SSN int NOT NULL,
    CONSTRAINT PK_Physician PRIMARY KEY (EmployeeID)
);
INSERT INTO Physician
VALUES 
(10006,"Filius Flitwick","Attending Surgeon",20231467),
(10013,"Helga Hufflepuff","Surgeon",20231474),
(10022,"Hermione Granger","HOD",20231477),
(20018,"Luna Lovegood","Emergency",20231482),
(20021,"Harry Potter","HOD",20231486),
(30007,"Neville Longbottom","HOD",20231489),
(30023,"Parvati Patil","Surgeon",20231493),
(40017,"Minerva McGonagall","HOD",20231497),
(40019,"Ronald Weasley","Fellow",20231499),
(50033,"Alastor Moody","HOD",20231507)
;
CREATE TABLE 20CS10085.Procedure (
    Code int NOT NULL,
    Name TINYTEXT NOT NULL,
    Cost int NOT NULL,
    CONSTRAINT PK_Procedure PRIMARY KEY (Code)
);
INSERT INTO 20CS10085.Procedure
VALUES
(132,"Bypass Surgery",120000),
(165,"Pacemaker Surgery",270000),
(177,"Bone Surgery",90000),
(213,"Brain Surgery",320000),
(303,"Regular Checkup",3000)
;
CREATE TABLE Patient (
    SSN int NOT NULL,
    Name TINYTEXT NOT NULL,
    Address TINYTEXT NOT NULL,
    Phone TINYTEXT NOT NULL,
    InsuranceID int NOT NULL,
    PCP int NOT NULL,
    CONSTRAINT PK_Patient PRIMARY KEY (SSN),
    CONSTRAINT FK_Patient_Physician FOREIGN KEY (PCP) REFERENCES Physician(EmployeeID)
);
INSERT INTO Patient
VALUES
(20231356,"Amelia Bones","58656 Jamarcus Hollow, Apt. 036, 92422-8259, South Stephanie, Iowa, United States","231-671-8369",231356,40017),
(20231372,"Charity Burbage","50715 Mitchel Square, Apt. 275, 98946, Princeview, Delaware, United States","303-953-0418",231372,10013),
(20231396,"Vincent Crabbe","5638 Merl Locks, Suite 336, 87757, Parkertown, Texas, United States","505-304-1251",231396,50033),
(20231398,"Colin Creevey","34707 Myrna Forge, Apt. 997, 38992, Wilmafort, Maine, United States","505-342-5073",231398,30023),
(20231412,"Antonin Dolohov" ,"0640 Vickie Points, Apt. 873, 97822, Jabarichester, Arkansas, United States","505-317-3858",231412,40019),
(20231425,"Marge Dursley","08397 Roob Roads, Apt. 129, 63643, West Rubye, Florida, United States","505-286-3358",231425,10022),
(20231436,"Argus Filch" ,"701 Rohan Orchard, Apt. 995, 16113-7152, East Roselyn, Alabama, United States","307-237-8667",231436,30023)
;
CREATE TABLE Appointment (
    AppointmentID int NOT NULL,
    Patient int NOT NULL,
    PrepNurse int,
    Physician int NOT NULL,
    Start datetime NOT NULL,
    `End` datetime NOT NULL,
    ExaminationRoom TINYTEXT NOT NULL,
    CONSTRAINT PK_Appointment PRIMARY KEY (AppointmentID),
    CONSTRAINT FK_Appointment_Nurse FOREIGN KEY (PrepNurse) REFERENCES Nurse(EmployeeID),
    CONSTRAINT FK_Appointment_Physician FOREIGN KEY (Physician) REFERENCES Physician(EmployeeID),
    CONSTRAINT FK_Appointment_Patient FOREIGN KEY (Patient) REFERENCES Patient(SSN)
);
INSERT INTO Appointment
VALUES
(1,20231356,170056,40017,(SELECT STR_TO_DATE('11/01/2022 13:30:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('11/01/2022 14:30:00', '%d/%m/%Y %H:%i:%s') AS `End`),1205),
(2,20231372,170063,10013,(SELECT STR_TO_DATE('13/02/2022 11:30:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('13/02/2022 12:30:00', '%d/%m/%Y %H:%i:%s') AS `End`),1409),
(3,20231396,170023,50033,(SELECT STR_TO_DATE('17/03/2022 18:30:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('17/03/2022 19:30:00', '%d/%m/%Y %H:%i:%s') AS `End`),1113),
(4,20231398,170077,30023,(SELECT STR_TO_DATE('05/04/2022 14:30:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('05/04/2022 15:30:00', '%d/%m/%Y %H:%i:%s') AS `End`),1307),
(5,20231412,170034,40019,(SELECT STR_TO_DATE('23/05/2022 12:45:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('23/05/2022 14:45:00', '%d/%m/%Y %H:%i:%s') AS `End`),1205),
(6,20231425,170039,10022,(SELECT STR_TO_DATE('21/06/2022 13:15:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('21/06/2022 14:15:00', '%d/%m/%Y %H:%i:%s') AS `End`),1409),
(7,20231436,170077,30023,(SELECT STR_TO_DATE('27/07/2022 10:30:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('27/07/2022 11:30:00', '%d/%m/%Y %H:%i:%s') AS `End`),1307),
(8,20231398,170045,30023,(SELECT STR_TO_DATE('29/08/2022 15:30:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('29/08/2022 16:30:00', '%d/%m/%Y %H:%i:%s') AS `End`),1307),
(9,20231372,170063,10013,(SELECT STR_TO_DATE('16/09/2022 16:30:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('16/09/2022 18:30:00', '%d/%m/%Y %H:%i:%s') AS `End`),1409),
(10,20231436,170045,30023,(SELECT STR_TO_DATE('10/10/2022 17:00:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('10/10/2022 18:00:00', '%d/%m/%Y %H:%i:%s') AS `End`),1307)
;
CREATE TABLE Department (
    DepartmentID int NOT NULL,
    Name TINYTEXT NOT NULL,
    Head int NOT NULL,
    CONSTRAINT PK_Department PRIMARY KEY (DepartmentID),
    CONSTRAINT FK_Department_Physician FOREIGN KEY (Head) REFERENCES Physician(EmployeeID)
);
INSERT INTO Department
VALUES
(100,"Neuro Surgery",10022),
(200,"Emergency",20021),
(300,"Cardiology",30007),
(400,"Pediatrics",40017),
(500,"Orthopedics",50033)
;
CREATE TABLE Room (
    Number int NOT NULL,
    Type TINYTEXT NOT NULL,
    BlockFloor int NOT NULL,
    BlockCode int NOT NULL,
    Unavailable boolean NOT NULL,
    CONSTRAINT PK_Room PRIMARY KEY (Number),
    CONSTRAINT FK_Room_Block FOREIGN KEY (BlockFloor,BlockCode) REFERENCES Block(Floor,Code)
);
INSERT INTO Room
VALUES
(1007,"Deluxe",0,200,FALSE),
(1113,"Deluxe",1,500,FALSE),
(1205,"Deluxe",2,400,TRUE),
(1303,"ICU",3,300,TRUE),
(1307,"Deluxe",3,300,TRUE),
(1402,"ICU",4,100,FALSE),
(1409,"Deluxe",4,100,TRUE),
(123,"ICU",0,200,FALSE)
;
CREATE TABLE Stay (
    StayID int NOT NULL,
    Patient int NOT NULL,
    Room int NOT NULL,
    Start datetime NOT NULL,
    `End` datetime NOT NULL,
    CONSTRAINT PK_Stay PRIMARY KEY (StayID),
    CONSTRAINT FK_Stay_Patient FOREIGN KEY (Patient) REFERENCES Patient(SSN),
    CONSTRAINT FK_Stay_Room FOREIGN KEY (Room) REFERENCES Room(Number)
);
INSERT INTO Stay
VALUES
(1,20231398,1303,(SELECT STR_TO_DATE('13/10/2022 11:30:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('30/10/2022 10:30:00', '%d/%m/%Y %H:%i:%s') AS `End`)),
(2,20231436,1303,(SELECT STR_TO_DATE('22/10/2022 14:30:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('26/10/2022 17:30:00', '%d/%m/%Y %H:%i:%s') AS `End`)),
(3,20231372,1402,(SELECT STR_TO_DATE('23/10/2022 15:30:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('20/11/2022 10:30:00', '%d/%m/%Y %H:%i:%s') AS `End`)),
(4,20231356,1205,(SELECT STR_TO_DATE('23/10/2022 18:30:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('24/10/2022 11:30:00', '%d/%m/%Y %H:%i:%s') AS `End`)),
(5,20231396,1113,(SELECT STR_TO_DATE('07/12/2022 13:30:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('13/12/2022 12:30:00', '%d/%m/%Y %H:%i:%s') AS `End`)),
(6,20231412,1007,(SELECT STR_TO_DATE('17/12/2022 14:30:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('24/12/2022 18:30:00', '%d/%m/%Y %H:%i:%s') AS `End`)),
(7,20231425,123,(SELECT STR_TO_DATE('19/12/2022 10:30:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('23/12/2022 10:30:00', '%d/%m/%Y %H:%i:%s') AS `End`))
;
---------------Relations----------------
CREATE TABLE Affiliated_with (
    Physician int NOT NULL,
    Department int NOT NULL,
    PrimaryAffiliation boolean NOT NULL,
    CONSTRAINT PK_Affiliated_with PRIMARY KEY (Physician,Department),
    CONSTRAINT FK_Affiliated_with_Physician FOREIGN KEY (Physician) REFERENCES Physician(EmployeeID),
    CONSTRAINT FK_Affiliated_with_Department FOREIGN KEY (Department) REFERENCES Department(DepartmentID)
);
INSERT INTO Affiliated_with
VALUES
(10006,100,TRUE),
(10013,100,TRUE),
(10022,100,TRUE),
(20018,200,TRUE),
(20021,200,TRUE),
(30007,300,TRUE),
(30023,300,TRUE),
(40017,400,TRUE),
(40019,400,TRUE),
(50033,500,TRUE),
(10022,200,FALSE)
;
CREATE TABLE On_Call (
    Nurse int NOT NULL,
    BlockFloor int NOT NULL,
    BlockCode int NOT NULL,
    Start datetime NOT NULL,
    `End` datetime NOT NULL,
    CONSTRAINT PK_On_Call PRIMARY KEY (Nurse,BlockFloor,BlockCode,Start,`End`),
    CONSTRAINT FK_On_Call_Nurse FOREIGN KEY (Nurse) REFERENCES Nurse(EmployeeID),
    CONSTRAINT FK_On_Call_Block FOREIGN KEY (BlockFloor,BlockCode) REFERENCES Block(Floor,Code)
);
INSERT INTO On_Call
VALUES
(170056,2,400,(SELECT STR_TO_DATE('01/01/2022 09:00:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('31/12/2022 20:00:00', '%d/%m/%Y %H:%i:%s') AS `End`)),
(170063,4,100,(SELECT STR_TO_DATE('01/01/2022 09:00:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('31/12/2022 20:00:00', '%d/%m/%Y %H:%i:%s') AS `End`)),
(170023,1,500,(SELECT STR_TO_DATE('01/01/2022 09:00:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('31/12/2022 20:00:00', '%d/%m/%Y %H:%i:%s') AS `End`)),
(170077,3,300,(SELECT STR_TO_DATE('01/01/2022 09:00:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('31/12/2022 20:00:00', '%d/%m/%Y %H:%i:%s') AS `End`)),
(170034,2,400,(SELECT STR_TO_DATE('01/01/2022 09:00:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('31/12/2022 20:00:00', '%d/%m/%Y %H:%i:%s') AS `End`)),
(170039,4,100,(SELECT STR_TO_DATE('01/01/2022 09:00:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('31/12/2022 20:00:00', '%d/%m/%Y %H:%i:%s') AS `End`)),
(170045,3,300,(SELECT STR_TO_DATE('01/01/2022 09:00:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('31/12/2022 20:00:00', '%d/%m/%Y %H:%i:%s') AS `End`)),
(170039,0,200,(SELECT STR_TO_DATE('17/12/2022 14:30:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('24/12/2022 18:30:00', '%d/%m/%Y %H:%i:%s') AS `End`))
;
CREATE TABLE Prescribes (
    Physician int NOT NULL,
    Patient int NOT NULL,
    Medication int NOT NULL,
    Date datetime NOT NULL,
    Appointment int,
    Dose TINYTEXT NOT NULL,
    CONSTRAINT PK_Prescribes PRIMARY KEY (Physician,Patient,Medication,Date),
    CONSTRAINT FK_Prescribes_Physician FOREIGN KEY (Physician) REFERENCES Physician(EmployeeID),
    CONSTRAINT FK_Prescribes_Patient FOREIGN KEY (Patient) REFERENCES Patient(SSN),
    CONSTRAINT FK_Prescribes_Medication FOREIGN KEY (Medication) REFERENCES Medication(Code),
    CONSTRAINT FK_Prescribes_Appointment FOREIGN KEY (Appointment) REFERENCES Appointment(AppointmentID)
);
INSERT INTO Prescribes
VALUES
(40017,20231356,406589,(SELECT STR_TO_DATE('11/01/2022 14:30:00', '%d/%m/%Y %H:%i:%s') AS Date),1,"Twice a day"),
(10013,20231372,393620,(SELECT STR_TO_DATE('13/02/2022 12:30:00', '%d/%m/%Y %H:%i:%s') AS Date),2,"Twice a day"),
(50033,20231396,412368,(SELECT STR_TO_DATE('17/03/2022 19:30:00', '%d/%m/%Y %H:%i:%s') AS Date),3,"Twice a day"),
(30023,20231398,436589,(SELECT STR_TO_DATE('05/04/2022 15:30:00', '%d/%m/%Y %H:%i:%s') AS Date),4,"Once a day"),
(40019,20231412,406589,(SELECT STR_TO_DATE('23/05/2022 14:45:00', '%d/%m/%Y %H:%i:%s') AS Date),5,"Twice a day"),
(10022,20231425,393620,(SELECT STR_TO_DATE('21/06/2022 14:15:00', '%d/%m/%Y %H:%i:%s') AS Date),6,"Twice a day"),
(30023,20231436,436589,(SELECT STR_TO_DATE('27/07/2022 11:30:00', '%d/%m/%Y %H:%i:%s') AS Date),7,"Once a day"),
(30023,20231398,436589,(SELECT STR_TO_DATE('29/08/2022 16:30:00', '%d/%m/%Y %H:%i:%s') AS Date),8,"Once a day"),
(10013,20231372,393620,(SELECT STR_TO_DATE('16/09/2022 18:30:00', '%d/%m/%Y %H:%i:%s') AS Date),9,"Twice a day"),
(30023,20231436,436589,(SELECT STR_TO_DATE('10/10/2022 18:00:00', '%d/%m/%Y %H:%i:%s') AS Date),10,"Once a day")
;
CREATE TABLE Trained_in (
    Physician int NOT NULL,
    Treatment int NOT NULL,
    CertificationDate datetime NOT NULL,
    CertificationExpires datetime NOT NULL,
    CONSTRAINT PK_Trained_in PRIMARY KEY (Physician,Treatment),
    CONSTRAINT FK_Trained_in_Physician FOREIGN KEY (Physician) REFERENCES Physician(EmployeeID),
    CONSTRAINT FK_Trained_in_Procedure FOREIGN KEY (Treatment) REFERENCES 20CS10085.Procedure(Code)
);
INSERT INTO Trained_in
VALUES
(10006,213,(SELECT STR_TO_DATE('14/05/2001 14:30:00', '%d/%m/%Y %H:%i:%s') AS CertificationDate),(SELECT STR_TO_DATE('14/05/2031 14:30:00', '%d/%m/%Y %H:%i:%s') AS CertificationExpires)),
(10013,213,(SELECT STR_TO_DATE('17/02/2007 10:30:00', '%d/%m/%Y %H:%i:%s') AS CertificationDate),(SELECT STR_TO_DATE('17/02/2037 10:30:00', '%d/%m/%Y %H:%i:%s') AS CertificationExpires)),
(10022,213,(SELECT STR_TO_DATE('12/07/2011 14:30:00', '%d/%m/%Y %H:%i:%s') AS CertificationDate),(SELECT STR_TO_DATE('12/07/2041 14:30:00', '%d/%m/%Y %H:%i:%s') AS CertificationExpires)),
(20018,177,(SELECT STR_TO_DATE('10/02/2012 11:00:00', '%d/%m/%Y %H:%i:%s') AS CertificationDate),(SELECT STR_TO_DATE('10/02/2032 11:00:00', '%d/%m/%Y %H:%i:%s') AS CertificationExpires)),
(20021,177,(SELECT STR_TO_DATE('29/07/2012 09:30:00', '%d/%m/%Y %H:%i:%s') AS CertificationDate),(SELECT STR_TO_DATE('29/07/2032 09:30:00', '%d/%m/%Y %H:%i:%s') AS CertificationExpires)),
(30007,132,(SELECT STR_TO_DATE('07/05/2013 18:30:00', '%d/%m/%Y %H:%i:%s') AS CertificationDate),(SELECT STR_TO_DATE('07/05/2028 18:30:00', '%d/%m/%Y %H:%i:%s') AS CertificationExpires)),
(30023,165,(SELECT STR_TO_DATE('09/07/2013 10:30:00', '%d/%m/%Y %H:%i:%s') AS CertificationDate),(SELECT STR_TO_DATE('09/07/2028 10:30:00', '%d/%m/%Y %H:%i:%s') AS CertificationExpires)),
(40017,303,(SELECT STR_TO_DATE('16/09/2017 11:30:00', '%d/%m/%Y %H:%i:%s') AS CertificationDate),(SELECT STR_TO_DATE('16/09/2027 11:30:00', '%d/%m/%Y %H:%i:%s') AS CertificationExpires)),
(40019,303,(SELECT STR_TO_DATE('23/10/2018 17:00:00', '%d/%m/%Y %H:%i:%s') AS CertificationDate),(SELECT STR_TO_DATE('23/10/2028 17:00:00', '%d/%m/%Y %H:%i:%s') AS CertificationExpires)),
(50033,177,(SELECT STR_TO_DATE('20/05/2002 15:45:00', '%d/%m/%Y %H:%i:%s') AS CertificationDate),(SELECT STR_TO_DATE('20/05/2022 15:45:00', '%d/%m/%Y %H:%i:%s') AS CertificationExpires))
;
CREATE TABLE Undergoes (
    Patient int NOT NULL,
    `Procedure` int NOT NULL,
    Stay int NOT NULL,
    Date datetime NOT NULL,
    Physician int NOT NULL,
    AssistingNurse int,
    CONSTRAINT PK_Undergoes PRIMARY KEY (Patient,`Procedure`,Stay,Date),
    CONSTRAINT FK_Undergoes_Patient FOREIGN KEY (Patient) REFERENCES Patient(SSN),
    CONSTRAINT FK_Undergoes_Procedure FOREIGN KEY (`Procedure`) REFERENCES 20CS10085.Procedure(Code),
    CONSTRAINT FK_Undergoes_Stay FOREIGN KEY (Stay) REFERENCES Stay(StayID),
    CONSTRAINT FK_Undergoes_Physician FOREIGN KEY (Physician) REFERENCES Physician(EmployeeID),
    CONSTRAINT FK_Undergoes_AssistingNurse FOREIGN KEY (AssistingNurse) REFERENCES Nurse(EmployeeID)
);
INSERT INTO Undergoes
VALUES
(20231398,132,1,(SELECT STR_TO_DATE('17/10/2022 09:30:00', '%d/%m/%Y %H:%i:%s') AS Date),30023,170045),
(20231436,165,2,(SELECT STR_TO_DATE('24/10/2022 13:30:00', '%d/%m/%Y %H:%i:%s') AS Date),30023,170077),
(20231372,213,3,(SELECT STR_TO_DATE('03/11/2022 17:30:00', '%d/%m/%Y %H:%i:%s') AS Date),10013,170063),
(20231356,303,4,(SELECT STR_TO_DATE('23/10/2022 20:30:00', '%d/%m/%Y %H:%i:%s') AS Date),40017,170056),
(20231396,177,5,(SELECT STR_TO_DATE('10/12/2022 10:30:00', '%d/%m/%Y %H:%i:%s') AS Date),50033,170023),
(20231412,177,6,(SELECT STR_TO_DATE('17/12/2022 15:30:00', '%d/%m/%Y %H:%i:%s') AS Date),10022,170039),
(20231425,132,7,(SELECT STR_TO_DATE('19/12/2022 13:00:00', '%d/%m/%Y %H:%i:%s') AS Date),50033,170039)
;
---------------------------------------------------------- Done till here
/*

BUILDING OF THE DATABASE DONE, NOW THE QUERIES

*/

--1. Names of all physicians who are trained in procedure name “Bypass Surgery”
SELECT Name
FROM Physician
WHERE EmployeeID IN (SELECT Physician
                     FROM Trained_in
                     WHERE Treatment = (SELECT Code
                                        FROM 20CS10085.Procedure
                                        WHERE Name = "Bypass Surgery"));

--2. Names of all physicians affiliated with the department name “cardiology” and trained in “bypass surgery”
SELECT Name
FROM Physician
WHERE EmployeeID IN (SELECT Physician
                     FROM Trained_in
                     WHERE Treatment = (SELECT Code
                                        FROM 20CS10085.Procedure
                                        WHERE Name = "Bypass Surgery") AND Physician IN (SELECT Physician
                                                                                          FROM Affiliated_with
                                                                                          WHERE Department = (SELECT DepartmentID
                                                                                                               FROM Department
                                                                                                               WHERE Name = "Cardiology"))); 

--3. Names of all the nurses who have ever been on call for room 1402
SELECT Name
FROM Nurse
WHERE EmployeeID IN (SELECT Nurse
                    FROM On_Call
                    WHERE (BlockFloor,BlockCode) IN (SELECT BlockFloor,BlockCode
                                                    FROM Room
                                                    WHERE Number = 1402));

--4. Names and addresses of all patients who were prescribed the medication named “remdesivir”
SELECT Name,Address
FROM Patient
WHERE SSN IN (SELECT Patient
                    FROM Prescribes
                    WHERE Medication IN (SELECT Code
                                    FROM Medication
                                    WHERE Name = 'Remdesivir'));

--5. Name and insurance id of all patients who stayed in the “icu” room type for more than 15 days
SELECT Name,InsuranceID
FROM Patient
WHERE SSN IN (SELECT Patient
                    FROM Stay
                    WHERE Room IN (SELECT Number
                                    FROM Room
                                    WHERE Type = 'ICU') AND DATEDIFF(`End`,Start)>15);

--6. Names of all nurses who assisted in the procedure name “bypass surgery”
SELECT Name
FROM Nurse
WHERE EmployeeID IN (SELECT AssistingNurse
                    FROM Undergoes U
                    WHERE U.Procedure = (SELECT Code
                                    FROM 20CS10085.Procedure
                                    WHERE Name = "Bypass Surgery"));

--7. Name and position of all nurses who assisted in the procedure name “bypass surgery” along with the names of and the accompanying physicians
SELECT N.Name 'Nurse Name',N.Position 'Nurse Position',P.Name 'Physician Name'
FROM Physician P,Nurse N
WHERE (P.EmployeeID,N.EmployeeID) IN (SELECT Physician,AssistingNurse
                    FROM Undergoes
                    WHERE Undergoes.Procedure = (SELECT Code
                                    FROM 20CS10085.Procedure
                                    WHERE Name = "Bypass Surgery"));

--8. Obtain the names of all physicians who have performed a medical procedure they have never been trained to perform
SELECT Name
FROM Physician
WHERE EmployeeID IN (SELECT Physician
        FROM Undergoes U
        WHERE (Physician,U.Procedure) NOT IN (SELECT Physician,Treatment
                                    FROM Trained_in));

--9. Names of all physicians who have performed a medical procedure that they are trained to perform, 
-- but such that the procedure was done at a date (Undergoes.Date) after the physician's certification expired (Trained_In.CertificationExpires)
SELECT P.Name
FROM ((Undergoes U
INNER JOIN Trained_in T        
ON U.Physician = T.Physician AND U.Procedure = T.Treatment AND DATEDIFF(U.Date,T.CertificationExpires)>0)
INNER JOIN Physician P ON U.Physician = P.EmployeeID);

--10. Same as the previous query, but include the following information in the results: Physician name, name of procedure, 
-- date when the procedure was carried out, name of the patient the procedure was carried out on
SELECT P.Name,PR.Name,U.Date,PA.Name
FROM ((((Undergoes U
INNER JOIN Trained_in T        
ON U.Physician = T.Physician AND U.Procedure = T.Treatment AND DATEDIFF(U.Date,T.CertificationExpires)>0)
INNER JOIN 20CS10085.Procedure PR ON U.Procedure = PR.Code)
INNER JOIN Patient PA ON U.Patient = PA.SSN)
INNER JOIN Physician P ON U.Physician = P.EmployeeID);

/* 11. Names of all patients (also include, for each patient, the name of the patient's physician), such that all the following are true:
1. The patient has been prescribed some medication by his/her physician
2. The patient has undergone a procedure with a cost larger that 5000
3. Name of all the patient who had at least two appointment where the physician was affiliated with the cardiology department
4. The patient's physician is not the head of any department */

SELECT PA.Name "Patient's Name",P.Name "Physician's Name"
FROM (((SELECT Patient,Physician
        FROM Undergoes
        WHERE (Patient,Physician) IN (SELECT Patient,Physician
                                        FROM Prescribes
                                        WHERE (Patient,Physician) IN (SELECT Patient,Physician
                                                                    FROM Appointment
                                                                    WHERE Physician IN (SELECT Physician
                                                                                        FROM Affiliated_with
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
                                                            FROM 20CS10085.Procedure
                                                            WHERE Cost>5000)
        GROUP BY Patient,Physician
        HAVING COUNT(*)>=1) AS A
INNER JOIN Physician P ON A.Physician = P.EmployeeID)
INNER JOIN Patient PA ON A.Patient = PA.SSN);

--12. Name and brand of the medication which has been prescribed to the highest number of patients
SELECT Name,Brand
FROM Medication
WHERE Code = (SELECT Medication
                FROM Prescribes
                GROUP BY Medication
                ORDER BY COUNT(*) DESC
                LIMIT 1);


