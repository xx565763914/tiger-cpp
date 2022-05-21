
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


select 
    (select case when sum(traded) is null then 0 else sum(traded) end from strategy_order where direction = 'long' and comb_offset = 'open') - (select case when sum(traded) is null then 0 else sum(traded) end from strategy_order where direction = 'short' and comb_offset = 'close') as long,
    (select case when sum(traded) is null then 0 else sum(traded) end from strategy_order where direction = 'short' and comb_offset = 'open') - (select case when sum(traded) is null then 0 else sum(traded) end from strategy_order where direction = 'long' and comb_offset = 'close') as short;
    

create table bar_store(
    instrument_id varchar(30),
    date_time varchar(30),
    open_price double precision,
    close_price double precision,
    high_price double precision,
    low_price double precision,
    volume bigint,
    turnover double precision,
    open_interest double precision,
    ask_price1 double precision,
    ask_volume1 bigint,
    bid_price1 double precision,
    bid_volume1 bigint,
    PRIMARY KEY (instrument_id, date_time)
);