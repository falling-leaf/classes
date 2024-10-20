create database examming;

use examming;
create table realscore(
	stuid int,
    stuname varchar(40),
    stuscore int
);

create view see_score
as
select stuid, stuscore
from realscore;

select* from see_score;

grant select on examming.see_score to 'student1'@'localhost';

revoke select on examming.see_score from 'student1'@'localhost';

use examming;
insert realscore values(322000, '张三', 100);
insert realscore values(322001, '李四', 59);

use examming;
show grants for 'root'@'localhost';


create user 'student1'@'localhost' identified by '654321';

create user 'student2' @ '%' identified by '123456';

grant create on examming.* to 'student1'@'localhost';

insert fakescore values(322001, '李四', 0);

revoke create on examming.* from 'student1'@'localhost';

grant insert on fakescore to 'student1'@'localhost';

select* from fakescore;

revoke insert on fakescore from 'student1'@'localhost';