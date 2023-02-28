CREATE TABLE accounts_db_admin (
    username varchar(255) primary key,
    password varchar(255) not null
);



CREATE TABLE  accounts_physician (
    Email_id varchar(255) primary key,
    EmployeeID int not null,
    FirstName varchar(255) not null,
    LastName varchar(255) not null,
    Department varchar(255) not null,
    Position varchar(255) not null,
    password varchar(255) not null
);

CREATE TABLE  accounts_front_desk (
    Email varchar(255) primary key,
    FirstName varchar(255) not null,
    LastName varchar(255) not null,
    EmployeeID int not null,
    password varchar(255) not null
);

CREATE TABLE  accounts_data_entry (
    Email varchar(255) primary key,
    FirstName varchar(255) not null,
    LastName varchar(255) not null,
    EmployeeID int not null,
    password varchar(255) not null
);


CREATE TABLE accounts_patient (
    Email_id varchar(255) primary key,
    SSN int not null,
    FirstName varchar(255) not null,
    LastName varchar(255) not null,
    Address varchar(255) not null,
    InsuranceID int not null,
    Phone varchar(255) not null,
    Age int not null,
    BloodGroup varchar(255) not null,
    Status int not null
);

CREATE TABLE accounts_tests (
    TestID int AUTO_INCREMENT primary key,
    TestName varchar(255) not null,
    Cost int not null  
);


CREATE TABLE accounts_treatment (
    TreatmentID int AUTO_INCREMENT primary key,
    TreatmentName varchar(255) not null,
    Cost int not null
);

CREATE TABLE accounts_undergoes (
    UndergoesID int AUTO_INCREMENT,
    Patient varchar(255) not null,
    TreatmentID int not null,
    Physician varchar(255) not null,
    Date DATETIME not null,
    PRIMARY KEY (UndergoesID, Patient, TreatmentID, Physician, Date)
);


CREATE TABLE accounts_room (
    id int AUTO_INCREMENT,
    Number int not null,
    Type varchar(255) not null,
    Room_name varchar(255) not null,
    Capacity int not null,
    Cost int not null,
    PRIMARY KEY (id, Number, Room_name)
);

CREATE TABLE accounts_admission (
    AdmissionID int not null AUTO_INCREMENT,
    Patient varchar(255) not null,
    Room int not null,
    Start DATETIME not null,
    End DATETIME not null,
    PCP_email varchar(255) not null,
    Total_Cost int ,
    PRIMARY KEY (AdmissionID)
);

CREATE TABLE accounts_prescribes (
    PrescribeID int AUTO_INCREMENT,
    Physician varchar(255) not null,
    Patient varchar(255) not null,
    Date DATETIME not null,
    Prescription text not null,
    PRIMARY KEY (PrescribeID,Physician, Patient, Date)
);

CREATE TABLE accounts_appointment (
    AppointmentID int not null AUTO_INCREMENT,
    Patient varchar(255) not null,
    Physician varchar(255) not null,
    Start DATETIME not null,
    AppointmentFee int ,
    PRIMARY KEY (AppointmentID)
);


CREATE TABLE accounts_health_record (
    RecordID int not null AUTO_INCREMENT,
    Patient varchar(255) not null,
    Date DATETIME not null,
    Vitals text,
    Remarks text,
    PRIMARY KEY (RecordID)
);

CREATE TABLE accounts_patient_test (
    ID int primary key AUTO_INCREMENT,
    Patient varchar(255) not null,
    TestID int not null,
    Date DATETIME not null,
    Test_result text
);



-- foreign key setting
ALTER TABLE accounts_admission 
ADD FOREIGN KEY (Patient) REFERENCES accounts_patient(Email_id);


ALTER TABLE accounts_admission 
ADD FOREIGN KEY (PCP_email) REFERENCES accounts_physician(Email_id);

ALTER TABLE accounts_admission 
ADD FOREIGN KEY (Room) REFERENCES accounts_room(id);


ALTER TABLE accounts_prescribes 
ADD FOREIGN KEY (Patient) REFERENCES accounts_patient(Email_id);


ALTER TABLE accounts_prescribes 
ADD FOREIGN KEY (Physician) REFERENCES accounts_physician(Email_id);


ALTER TABLE accounts_appointment 
ADD FOREIGN KEY (Patient) REFERENCES accounts_patient(Email_id);


ALTER TABLE accounts_appointment 
ADD FOREIGN KEY (Physician) REFERENCES accounts_physician(Email_id);


ALTER TABLE accounts_health_record 
ADD FOREIGN KEY (Patient) REFERENCES accounts_patient(Email_id);


ALTER TABLE accounts_patient_test 
ADD FOREIGN KEY (Patient) REFERENCES accounts_patient(Email_id);

ALTER TABLE accounts_undergoes 
ADD FOREIGN KEY (TreatmentID) REFERENCES accounts_treatment(TreatmentID);

ALTER TABLE accounts_undergoes 
ADD FOREIGN KEY (Patient) REFERENCES accounts_patient(Email_id);

ALTER TABLE accounts_undergoes 
ADD FOREIGN KEY (Physician) REFERENCES accounts_physician(Email_id);





DROP TABLE accounts_patient_test;
DROP TABLE accounts_health_record;
DROP TABLE accounts_admission;
DROP TABLE accounts_prescribes;
DROP TABLE accounts_appointment;
DROP TABLE accounts_front_desk;
DROP TABLE accounts_data_entry;
DROP TABLE accounts_room;
DROP TABLE accounts_undergoes;
DROP TABLE accounts_patient;
DROP TABLE accounts_physician;
DROP TABLE accounts_tests;
DROP TABLE accounts_treatment;

