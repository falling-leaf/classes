use honkai3;
/* 注释用这个可以打 */
-- 这个也行 --
/* sql的关键字奇奇怪怪的 */
/* 这里创建一个崩三人物的表 */
CREATE TABLE person
(name varchar(40),
 attr varchar(10),
 level int);
 
 use honkai3;
 ALTER TABLE person ADD weapon bool;
 
 use honkai3;
 DROP TABLE person;
 
 use honkai3;
 CREATE INDEX nameex ON person(name);
 
 use honkai3;
DROP INDEX nameex ON person;

CREATE VIEW see_person
AS
SELECT name, attr, level
FROM person
WHERE name = '爱莉希雅';

use honkai3;
drop view see_person;

use honkai3;
insert into person values('终焉之律者', '虚数', 80);
insert into person values('理之律者', '机械', 80);
insert into person values('空之律者', '生物', 80);
insert into person values('彼岸双生', '量子', 80);

use honkai3;
select* from person;

use honkai3;
update person
set level = 100
where name = '爱莉希雅';

use honkai3;
delete from person
where name = '爱莉希雅';

select attr, level
from honkai3.person
where name = '彼岸双生';

use honkai3;
create table other
(name varchar(40),
 realname varchar(40)
);
insert into other values('终焉之律者', '琪亚娜');
insert into other values('彼岸双生', '希儿');
insert into other values('空之律者', '琪亚娜');

drop table other;

select* from person;
select* from other;

use honkai3;
select person.name, attr, realname
from person, other
where person.name = other.name;

update person
set level = 100
where name = '彼岸双生';

use honkai3;
select max(level) as level_max
from (select person.name, attr, level, realname
	from person, other
    where person.name = other.name)
    as information
where name = '彼岸双生';

CREATE VIEW see_person
AS
SELECT name, attr, level
FROM person







