CREATE TABLE  accounts_physician (
    EmployeeID int primary key,
    Name varchar(255) not null,
    Position varchar(255) not null,
    SSN int not null
);

CREATE TABLE  accounts_front_desk (
    name varchar(255) not null,
    surname varchar(255) not null,
    reg_id int primary key
);

CREATE TABLE  accounts_data_entry (
    name varchar(255) not null,
    surname varchar(255) not null,
    reg_id int primary key
);

DROP TABLE accounts_physician;
DROP TABLE accounts_front_desk;
DROP TABLE accounts_data_entry;