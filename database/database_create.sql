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

INSERT INTO `user` VALUES (1,'bf8a63cd-b750-46fc-9d9b-bbd757d187c8','alice','18912345678','202cb962ac59075b964b07152d234b70','',0),(2,'3bde5a18-1339-4d32-8eeb-442f623748fb','barbara','18987654321','202cb962ac59075b964b07152d234b70','',0),(3,'a5c26eb5-598d-46c4-b1ad-e34bce4e490f','ciri','18987654322','202cb962ac59075b964b07152d234b70','',0),(4,'7943e367-2fb9-45fb-ba4e-83939f4519a9','diana','18987654323','202cb962ac59075b964b07152d234b70','',0);

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

INSERT INTO `friend_apply` VALUES (1,'bf8a63cd-b750-46fc-9d9b-bbd757d187c8','3bde5a18-1339-4d32-8eeb-442f623748fb','bba','hi~',0,0),(2,'a5c26eb5-598d-46c4-b1ad-e34bce4e490f','bf8a63cd-b750-46fc-9d9b-bbd757d187c8','a','hello alice, i am ciri',0,0),(3,'7943e367-2fb9-45fb-ba4e-83939f4519a9','bf8a63cd-b750-46fc-9d9b-bbd757d187c8','al','hello alice, i am diana',0,0);


