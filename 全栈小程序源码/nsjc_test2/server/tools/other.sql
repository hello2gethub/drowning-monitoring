DROP table if exists `opinions`;

create table `opinions`(
    `id` int(11) not null auto_increment,
    `openid` varchar(100) not null,
    `src` text default null,
    `wechat` varchar(100) default null,
    `opinion` text not null,
    `create_time` timestamp not null default current_timestamp,
    primary key(`id`)
)default charset=`utf8`;

create table `records`(
    `id` int(11) not null auto_increment,
    `openid` varchar(100) not null,
    `Temp` float(20) not null,
    `Press` int(11) not null,
    `Heart` int(11) not null,
    `Beep` varchar(100) not null,
    `name` varchar(100) default null,
    `latitude` float(20) default null,
    `longitude` float(20) default null,
    `note` varchar(100) default null,
    `create_time` timestamp not null default current_timestamp,
    primary key(`id`)
)default charset=`utf8`;