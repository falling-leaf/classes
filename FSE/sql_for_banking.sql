/* create database banking;
use banking;
create table branch(
	branch_name char(40) primary key,
    branch_city char(40),
    assets int
);
create table customer(
	ID int primary key,
    customer_name char(40),
    customer_street char(40),
    customer_city char(40)
);
create table loan(
	loan_number int primary key,
    branch_name char(40),
    amount int
);
create table borrower(
	ID int,
    loan_number int,
    primary key(ID, loan_number)
);
create table account(
	account_number int primary key,
    branch_name char(40),
    balance int
);
create table depositor(
	ID int,
    account_number int,
    primary key(ID, account_number)
);
*/

/*
insert into branch values('nameA', 'cityA', 10000);
insert into branch values('nameB', 'cityA', 20000);
insert into branch values('nameC', 'cityB', 30000);
insert into customer values(12345, 'cA', 'sA', 'cityA');
insert into customer values(23456, 'cB', 'sB', 'cityA');
insert into customer values(34567, 'cC', 'sC', 'cityB');
insert into customer values(123, 'cD', 'sA', 'cityA');
insert into loan values(54321, 'nameA', 5000);
insert into loan values(65432, 'nameC', 4000);
insert into borrower values(12345, 54321);
insert into borrower values(23456, 65432);
insert into account values(98765, 'nameA', 3000);
insert into account values(76543, 'nameB', 5000);
insert into depositor values(12345, 98765);
insert into depositor values(34567, 76543);

*/
insert into customer values(11111, 'cE', 'sD', 'cityD');
insert into depositor values(11111, 11112);
insert into depositor values(11111, 11113);
insert into account values(11112, 'nameA', 1000);
insert into account values(11113, 'nameB', 2000);

select *
from customer as C, branch as B, account as A, depositor as D
where C.ID = D.ID and
	D.account_number = A.account_number and
    B.branch_city = 'cityA' and
    A.branch_name = B.branch_name;