create database genshin;
use genshin;
create table person(
	name varchar(40),
    attr varchar(40),
    person_level int,
    person_artifact int,
    primary key(name),
    check(attr in('风', '雷', '火', '水', '岩', '草', '冰')),
    check(person_level > 0 AND person_level <= 90),
    check(person_artifact >= 0 AND person_artifact <= 6)
);

create table weapon(
	name varchar(40),
    weapon_name varchar(40),
    weapon_level int,
    weapon_artifact int,
    foreign key(name) references person(name),
    check(weapon_level > 0 AND weapon_level <= 90)
);

create table relics(
	name varchar(40),
    relics_name varchar(40),
    relics_number int,
    foreign key(name) references person(name),
    check(relics_number >= 0 AND relics_number <= 4)
);

drop table genshin.person;
drop table genshin.weapon;
drop table genshin.relics;

insert person values('神里绫华', '冰', 90, 0);
insert person values('钟离', '岩', 80, 1);
insert person values('刻晴', '雷', 75, 2);

insert person values('神里绫华', '冰', 90, 6);

insert weapon values('神里绫华', '雾切之回光', 90, 0);

delete from person
where name = "神里绫华";

use genshin;
update person
set name = '甘雨'
where attr = '冰';

insert person values('纳西妲', '草', 90, 6);

create assertion level_range
check
(not exists(select* from person)
where level > 90
);

 delimiter //
create trigger level_present
	after update on person
    for each row
begin
	update weapon set weapon_artifact = 6
    where weapon.name in (select name from person where person_level < 80);
end; //
 delimiter ;
 
 select * from person;
 select * from weapon;
 insert weapon values('神里绫华', '无锋剑', 10, 1);
 insert weapon values('刻晴', '雾切之回光', 20, 0);
 insert weapon values('钟离', '黑缨枪', 30, 0);

update person set person_level = 30
where name = '神里绫华';















