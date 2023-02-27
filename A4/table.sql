CREATE TABLE accounts_db_admin (
    username varchar(255) primary key,
    password varchar(255) not null
);

CREATE TABLE accounts_department (
    DepartmentID int primary key,
    Name varchar(255) not null,
    Head int not null
);

CREATE TABLE accounts_affiliated_with (
    id int AUTO_INCREMENT,
    Physician int not null,
    Department int not null,
    PrimaryAffiliation TINYINT(1) not null,
    PRIMARY KEY (id, Physician, Department)
);
-- 
CREATE TABLE accounts_trained_in (
    id int AUTO_INCREMENT,
    Physician int not null,
    Treatment int not null,
    CertificationDate DATETIME not null,
    CertificationExpires DATETIME not null,
    PRIMARY KEY (id,Physician, Treatment)
);

CREATE TABLE accounts_procedures (
    Code int primary key,
    Name varchar(255) not null,
    Cost int not null  
);


CREATE TABLE  accounts_physician (
    EmployeeID int primary key,
    Name varchar(255) not null,
    Position varchar(255) not null,
    SSN int not null,
    password varchar(255) not null
);

CREATE TABLE  accounts_front_desk (
    name varchar(255) not null,
    surname varchar(255) not null,
    reg_id int primary key,
    password varchar(255) not null
);

CREATE TABLE  accounts_data_entry (
    name varchar(255) not null,
    surname varchar(255) not null,
    reg_id int primary key,
    password varchar(255) not null
);



CREATE TABLE accounts_patient (
    SSN int primary key,
    Name varchar(255) not null,
    Address varchar(255) not null,
    Phone varchar(255) not null,
    InsuranceID int not null,
    PCP int not null,
    Status int not null
);

CREATE TABLE accounts_undergoes (
    id int AUTO_INCREMENT,
    Patient int not null,
    Procedures int not null,
    Stay int not null,
    Date DATETIME not null,
    Physician int not null,
    PRIMARY KEY (id, Patient, Procedures, Stay, Date)
);
CREATE TABLE accounts_stay (
    StayID int not null AUTO_INCREMENT,
    Patient int not null,
    Room int not null,
    Start DATETIME not null,
    End DATETIME not null,
    PRIMARY KEY (StayID)
);

CREATE TABLE accounts_room (
    id int AUTO_INCREMENT,
    Number int not null,
    Type varchar(255) not null,
    Room_name varchar(255) not null,
    Capacity int not null,
    PRIMARY KEY (id, Number, Room_name)
);

CREATE TABLE accounts_admission (
    AdmissionID int not null AUTO_INCREMENT,
    Patient int not null,
    Room int not null,
    Start DATETIME not null,
    End DATETIME not null,
    PRIMARY KEY (AdmissionID)
);

CREATE TABLE accounts_prescribes (
    id int AUTO_INCREMENT,
    Physician int not null,
    Patient int not null,
    Medication int not null,
    Date DATETIME not null,
    Appointment int,
    Dose varchar(255) not null,
    PRIMARY KEY (id,Physician, Patient, Medication, Date)
);


CREATE TABLE accounts_medication (
    Code int not null,
    Name varchar(255) not null,
    Brand varchar(255) not null,
    Description varchar(255) not null,
    PRIMARY KEY (Code)
);

CREATE TABLE accounts_appointment (
    AppointmentID int not null,
    Patient int not null,
    Physician int not null,
    Start DATETIME not null,
    End DATETIME not null,
    ExaminationRoom varchar(255) not null,
    PRIMARY KEY (AppointmentID)
);





-- foreign key setting
ALTER TABLE accounts_department 
ADD FOREIGN KEY (Head) REFERENCES accounts_physician(EmployeeID);


ALTER TABLE accounts_affiliated_with 
ADD FOREIGN KEY (Physician) REFERENCES accounts_physician(EmployeeID);


ALTER TABLE accounts_affiliated_with 
ADD FOREIGN KEY (Department) REFERENCES accounts_department(DepartmentID);


ALTER TABLE accounts_trained_in 
ADD FOREIGN KEY (Physician) REFERENCES accounts_physician(EmployeeID);


ALTER TABLE accounts_trained_in
ADD FOREIGN KEY (Treatment) REFERENCES accounts_procedures(Code);


ALTER TABLE accounts_undergoes 
ADD FOREIGN KEY (Patient) REFERENCES accounts_patient(SSN);


ALTER TABLE accounts_undergoes 
ADD FOREIGN KEY (Procedures) REFERENCES accounts_procedures(Code);


ALTER TABLE accounts_undergoes 
ADD FOREIGN KEY (Stay) REFERENCES accounts_stay(StayID);


ALTER TABLE accounts_patient 
ADD FOREIGN KEY (PCP) REFERENCES accounts_physician(EmployeeID);

ALTER TABLE accounts_undergoes 
ADD FOREIGN KEY (Stay) REFERENCES accounts_stay(StayID);


ALTER TABLE accounts_prescribes 
ADD FOREIGN KEY (Physician) REFERENCES accounts_physician(EmployeeID);


ALTER TABLE accounts_prescribes 
ADD FOREIGN KEY (Patient) REFERENCES accounts_patient(SSN);

ALTER TABLE accounts_prescribes 
ADD FOREIGN KEY (Medication) REFERENCES accounts_medication(Code);

ALTER TABLE accounts_prescribes 
ADD FOREIGN KEY (Appointment) REFERENCES accounts_appointment(AppointmentID);


ALTER TABLE accounts_stay 
ADD FOREIGN KEY (Patient) REFERENCES accounts_patient(SSN);

ALTER TABLE accounts_stay 
ADD FOREIGN KEY (Room) REFERENCES Room(Number);



ALTER TABLE accounts_appointment 
ADD FOREIGN KEY (Patient) REFERENCES accounts_patient(SSN);


ALTER TABLE accounts_appointment 
ADD FOREIGN KEY (Physician) REFERENCES accounts_physician(EmployeeID);

ALTER TABLE accounts_admission
ADD FOREIGN KEY (Patient) REFERENCES accounts_patient(SSN);
ALTER TABLE accounts_admission
ADD FOREIGN KEY (Room) REFERENCES accounts_room(id);
ALTER TABLE accounts_admission AUTO_INCREMENT=1000;


DROP TABLE accounts_admission;
DROP TABLE accounts_prescribes;
DROP TABLE accounts_appointment;
DROP TABLE accounts_front_desk;
DROP TABLE accounts_medication;
DROP TABLE accounts_data_entry;
DROP TABLE accounts_room;
DROP TABLE accounts_undergoes;
DROP TABLE accounts_trained_in;
DROP TABLE accounts_procedures;
DROP TABLE accounts_affiliated_with;
DROP TABLE accounts_department;
DROP TABLE accounts_stay;
DROP TABLE accounts_patient;
DROP TABLE accounts_physician;

