create database if not exists chatroom;

use chatroom;

-- todo: keyi 加上创建时间、更新时间等

create table if not exists user (
    id int primary key auto_increment,
    uid varchar(40) not null unique,
    name varchar(30) not null,
    phone varchar(20) not null unique,
    password varchar(255) not null,
    avatar varchar(255) not null default '',
    is_delete boolean not null default false,
    index (uid asc) using btree,
    index (phone),
    index (name)
);

create table if not exists friend_relation (
    id int primary key auto_increment,
    uid varchar(40) not null,
    friend_uid varchar(40) not null,
    remark_name varchar(30) default '',
    is_delete boolean not null default false,
    foreign key(uid) references user(uid),
    foreign key(friend_uid) references user(uid)
    -- index (uid) -- mysql会自动为外键创建索引
);

create table if not exists friend_apply (
    id int primary key auto_increment,
    from_uid varchar(40) not null,
    to_uid varchar(40) not null,
    remark_name varchar(30) default '',
    apply_message varchar(255) default '',
    status int not null default 0 , -- 状态字段，0表示未处理， 1表示同意， 2表示拒绝
    is_delete boolean not null default false,
    foreign key(to_uid) references user(uid),
    foreign key(from_uid) references user(uid),
    unique (from_uid, to_uid)
    -- index (from_uid asc, to_uid asc) using btree -- unique约束会自动创建索引
);


