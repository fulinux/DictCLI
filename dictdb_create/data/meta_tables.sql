drop table if exists oxford;

create table oxford
(
id integer primary key,
word varchar(255) not null,
length integer not null,
first_letter char(1) not null,
meanings text
)
