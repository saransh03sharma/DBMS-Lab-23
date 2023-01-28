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
    AppointmtentID int NOT NULL,
    Patient int NOT NULL,
    PrepNurse int,
    Physician int NOT NULL,
    Start datetime NOT NULL,
    `End` datetime NOT NULL,
    ExaminationRoom TINYTEXT NOT NULL,
    CONSTRAINT PK_Appointment PRIMARY KEY (AppointmtentID),
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
(1409,"Deluxe",4,100,TRUE)
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
(6,20231412,1007,(SELECT STR_TO_DATE('17/12/2022 14:30:00', '%d/%m/%Y %H:%i:%s') AS Start),(SELECT STR_TO_DATE('24/12/2022 18:30:00', '%d/%m/%Y %H:%i:%s') AS `End`))
;
---------------------------------------------------------- Done till here



