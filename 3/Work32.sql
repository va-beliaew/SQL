SELECT name, year FROM album WHERE year = 2018;
select name, time from track order by time desc limit 1;
select name, time from track where time >= '3:30';
select name from collection where year between 2018 and 2021;
select name from songer where name not like ('% %');
select name from track where name like ('%My%') or name like ('%my%');