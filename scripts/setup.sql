drop database if exists entry_task;
create database entry_task;
use entry_task;
create table users (
	username int auto_increment,
	password varchar(255),
	nickname varchar(255),
	primary key (username)
);
insert into users (password, nickname) values ('pswd', 'test user 1');
insert into users (password, nickname) values ('pswd', 'test user 2');
insert into users (password, nickname) values ('pswd', 'test user 3');
insert into users (password, nickname) values ('pswd', 'test user 4');
insert into users (password, nickname) values ('pswd', 'test user 5');
