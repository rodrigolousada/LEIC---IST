drop table if exists reserva_fact;
drop table if exists user_dimension;
drop table if exists localizacao_dimension;
drop table if exists tempo_dimension;
drop table if exists data_dimension;


create table user_dimension(
    nif varchar(9) not null unique,
    nome varchar(80) not null,
    telefone varchar(26) not null,
    primary key(nif)
);

create table localizacao_dimension(
	localizacao_id varchar(255) not null unique,
    edificio varchar(255) not null,
    espaco varchar(255) not null,
    posto varchar(255),
    primary key(localizacao_id)
);

create table tempo_dimension(
	tempo_id varchar(255) not null unique,
	hora int not null,
	minuto int not null,
	primary key(tempo_id)
);

create table data_dimension(
	data_id varchar(255) not null unique,
	ano int not null,
	semestre int not null,
	mes int not null,
	semana int not null,
	dia int not null,
	primary key(data_id)
);

create table reserva_fact(
	numero_reserva varchar(255) not null unique,
	nif varchar(9) not null,
	localizacao_id varchar(255) not null,
	tempo_id varchar(255) not null,
	data_id varchar(255) not null,
	duracao_dias int not null,
	montante_pago numeric(19,4) not null,
	tarifa numeric(19,4) not null,
	primary key(numero_reserva),
	foreign key(nif) references user_dimension(nif),
	foreign key(localizacao_id) references localizacao_dimension(localizacao_id),
	foreign key(tempo_id) references tempo_dimension(tempo_id),
	foreign key(data_id) references data_dimension(data_id)
);
