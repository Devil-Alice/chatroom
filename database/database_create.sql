create database if not exists chatroom;

use chatroom;

create table if not exists user (
    id int primary key auto_increment,
    uid varchar(40) not null,
    name varchar(30) not null,
    phone varchar(20) not null,
    password varchar(255) not null
);