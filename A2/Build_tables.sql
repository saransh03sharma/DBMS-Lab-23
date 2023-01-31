CREATE TABLE Block (
    Floor int NOT NULL,
    Code int NOT NULL,
    CONSTRAINT PK_Block PRIMARY KEY (Floor,Code)
);
CREATE TABLE Medication (
    Code int NOT NULL,
    Name TINYTEXT NOT NULL,
    Brand TINYTEXT NOT NULL,
    Description TINYTEXT NOT NULL,
    CONSTRAINT PK_Medication PRIMARY KEY (Code)
);
CREATE TABLE Nurse (
    EmployeeID int NOT NULL,
    Name TINYTEXT NOT NULL,
    Position TINYTEXT NOT NULL,
    Registered BOOLEAN NOT NULL,
    SSN int NOT NULL,
    CONSTRAINT PK_Nurse PRIMARY KEY (EmployeeID)
);
CREATE TABLE Physician (
    EmployeeID int NOT NULL,
    Name TINYTEXT NOT NULL,
    Position TINYTEXT NOT NULL,
    SSN int NOT NULL,
    CONSTRAINT PK_Physician PRIMARY KEY (EmployeeID)
);
CREATE TABLE Procedures (
    Code int NOT NULL,
    Name TINYTEXT NOT NULL,
    Cost int NOT NULL,
    CONSTRAINT PK_Procedures PRIMARY KEY (Code)
);
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
CREATE TABLE Department (
    DepartmentID int NOT NULL,
    Name TINYTEXT NOT NULL,
    Head int NOT NULL,
    CONSTRAINT PK_Department PRIMARY KEY (DepartmentID),
    CONSTRAINT FK_Department_Physician FOREIGN KEY (Head) REFERENCES Physician(EmployeeID)
);
CREATE TABLE Room (
    Number int NOT NULL,
    Type TINYTEXT NOT NULL,
    BlockFloor int NOT NULL,
    BlockCode int NOT NULL,
    Unavailable boolean NOT NULL,
    CONSTRAINT PK_Room PRIMARY KEY (Number),
    CONSTRAINT FK_Room_Block FOREIGN KEY (BlockFloor,BlockCode) REFERENCES Block(Floor,Code)
);
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
CREATE TABLE Affiliated_With (
    Physician int NOT NULL,
    Department int NOT NULL,
    PrimaryAffiliation boolean NOT NULL,
    CONSTRAINT PK_Affiliated_With PRIMARY KEY (Physician,Department),
    CONSTRAINT FK_Affiliated_With_Physician FOREIGN KEY (Physician) REFERENCES Physician(EmployeeID),
    CONSTRAINT FK_Affiliated_With_Department FOREIGN KEY (Department) REFERENCES Department(DepartmentID)
);
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
CREATE TABLE Trained_In (
    Physician int NOT NULL,
    Treatment int NOT NULL,
    CertificationDate datetime NOT NULL,
    CertificationExpires datetime NOT NULL,
    CONSTRAINT PK_Trained_In PRIMARY KEY (Physician,Treatment),
    CONSTRAINT FK_Trained_In_Physician FOREIGN KEY (Physician) REFERENCES Physician(EmployeeID),
    CONSTRAINT FK_Trained_In_Procedures FOREIGN KEY (Treatment) REFERENCES Procedures(Code)
);
CREATE TABLE Undergoes (
    Patient int NOT NULL,
    `Procedure` int NOT NULL,
    Stay int NOT NULL,
    Date datetime NOT NULL,
    Physician int NOT NULL,
    AssistingNurse int,
    CONSTRAINT PK_Undergoes PRIMARY KEY (Patient,`Procedure`,Stay,Date),
    CONSTRAINT FK_Undergoes_Patient FOREIGN KEY (Patient) REFERENCES Patient(SSN),
    CONSTRAINT FK_Undergoes_Procedures FOREIGN KEY (`Procedure`) REFERENCES Procedures(Code),
    CONSTRAINT FK_Undergoes_Stay FOREIGN KEY (Stay) REFERENCES Stay(StayID),
    CONSTRAINT FK_Undergoes_Physician FOREIGN KEY (Physician) REFERENCES Physician(EmployeeID),
    CONSTRAINT FK_Undergoes_AssistingNurse FOREIGN KEY (AssistingNurse) REFERENCES Nurse(EmployeeID)
);
