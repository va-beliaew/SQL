select distinct genre, count (distinct songer) from gensong
group by genre
order by count (distinct songer)
;

select count (track.name) from track
right join album on track.album = album.name
where album.year between 2019 and 2020;

select a.name, avg (t.time) from track t 
join album a on t.album = a.name
group by a.name
order by avg(t.time);

select sa.songer from songalb sa
join album a on a.name = sa.album
where a.year != 2020
group by sa.songer;

select tc.collection from tracoll tc
join track t on tc.track = t.name
join album a on t.album = a.name
where a.songer = 'Egor Bortnik';

select a.album, gs.songer, count(*) from songalb a
join gensong gs on a.songer = gs.songer
group by a.album, gs.songer
having count(*) > 1
;

select t.name from track t
left join tracoll tc on t.name = tc.track
where t.name not in (select track from tracoll)
;

select songer from songalb s
join track t on s.album = t.album 
where t.time = (select min(time) from track)
;

select album from (select album, count(*) from track group by album) s
 where (select min(count) from (select album, count(*) from track group by album)) = s.count
;