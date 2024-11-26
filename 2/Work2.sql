create table if not exists Genre (
id serial primary key,
name varchar(30) not null unique
);

create table if not exists Songer (
id serial primary key,
name varchar (40) not null unique
);

create table if not exists Gensong (
id serial primary key,
genre varchar(30) not null references Genre (name),
songer varchar(40) not null references Songer (name)
);

create table if not exists Album (
id serial primary key,
name varchar (40) not null unique,
year numeric not null,
songer varchar (40) not null
);

create table if not exists Songalb (
id serial primary key,
album varchar (40) not null references Album (name),
songer varchar (40) not null references Songer (name)
);

create table if not exists Track (
id serial primary key,
name varchar(40) not null unique,
time time not null,
album varchar (40) not null references Album (name)
);

create table if not exists Collection (
id serial primary key,
name varchar (40) not null unique,
year numeric not null
);


create table if not exists Tracoll (
id serial primary key,
track varchar (40) not null references Track (name),
collection varchar (40) not null references Collection (name)
);
