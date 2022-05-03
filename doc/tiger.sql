
create database tiger;

create table strategy_order(
    order_id varchar(30) primary key,
    direction varchar(10),
    comb_offset varchar(10),
    instrument_id varchar(50),
    price double precision,
    volume int,
    traded int,
    order_status varchar(20),
    strategy_name varchar(50),
    create_time bigint,
    update_time bigint
);
