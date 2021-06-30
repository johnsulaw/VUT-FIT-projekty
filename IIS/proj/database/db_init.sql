/*
    File: db_init.sql
    Author: Mario Gazo (xgazom00)
    Assignment IIS 2020/21, VUT FIT
*/

/*
    RESET DATABASE
*/
DROP TABLE reservation;
DROP TABLE receptions;
DROP TABLE user;
DROP TABLE guest;
DROP TABLE room_details;
DROP TABLE room;
DROP TABLE hotel;

/*
    CREATE TABLES
*/
CREATE TABLE hotel (
    id          INT         NOT NULL PRIMARY KEY AUTO_INCREMENT ,
    name        CHAR(50)    NOT NULL ,
    city        CHAR(50) ,
    street      CHAR(50) ,
    description CHAR(150) ,
    stars       INT ,
    rating      FLOAT ,
    image       CHAR(150),
    gallery     CHAR(50)
);

CREATE TABLE room (
    floor_room  INT         NOT NULL ,
    type        CHAR(20)    NOT NULL,

    hotel_id    INT         NOT NULL ,
    FOREIGN KEY (hotel_id) REFERENCES hotel(id) ON DELETE CASCADE,

    PRIMARY KEY (floor_room, hotel_id)
);

CREATE TABLE room_details (
    type        CHAR(20) NOT NULL ,
    beds        INT ,
    facility    CHAR(150) ,
    image       CHAR(100) ,
    price       INT ,

    hotel_id    INT         NOT NULL ,
    FOREIGN KEY (hotel_id) REFERENCES hotel(id) ,

    PRIMARY KEY (type, hotel_id)
);

CREATE TABLE guest (
    mail_address    CHAR(100)   NOT NULL PRIMARY KEY ,
    first_name      CHAR(50)    NOT NULL ,
    last_name       CHAR(50)    NOT NULL ,
    birth_date      DATE        NOT NULL ,
    city            CHAR(50)    NOT NULL ,
    street          CHAR(50)    NOT NULL
);

CREATE TABLE user (
    password        CHAR(100) NOT NULL,
    role            INT NOT NULL,

    guest_mail      CHAR(100) NOT NULL,
    FOREIGN KEY (guest_mail) references guest(mail_address)
);

CREATE TABLE reservation (
    id          INT NOT NULL PRIMARY KEY AUTO_INCREMENT ,
    from_date   DATE NOT NULL ,
    to_date     DATE NOT NULL ,
    price       INT NOT NULL ,
    payed_off   INT NULL ,

    guest_mail CHAR(100) NOT NULL ,
    FOREIGN KEY (guest_mail) REFERENCES guest(mail_address) ,

    room_id     INT NOT NULL ,
    FOREIGN KEY (room_id) REFERENCES room(floor_room) ,

    hotel_id    INT NOT NULL ,
    FOREIGN KEY (hotel_id) REFERENCES hotel(id)
);

CREATE TABLE receptions (
    guest_mail CHAR(100) NOT NULL ,
    FOREIGN KEY (guest_mail) REFERENCES guest(mail_address) ,

    hotel_id INT NOT NULL ,
    FOREIGN KEY (hotel_id) REFERENCES hotel(id)
);
/*
    INSERT EXAMPLE DATA
*/
-- Hotels
INSERT INTO hotel(name, city, street, description, stars, rating, image, gallery )
    VALUES ('Magnolia', -- name
            'Piestany', -- city
            'Nalepkova 20', -- street
            'Beautiful hotel in spa town', -- description
            3, -- stars
            4.1, -- rating
            'hotel1.jpg', -- image
            'hotel1' -- gallery
    );

INSERT INTO hotel(name, city, street, description, stars, rating, image, gallery )
    VALUES ('Chemes', -- name
            'Kaluza', -- city
            'Zemplinska 13', -- street
            'Hotel with great view on damn Zempínska Šírava', -- description
            4, -- stars
            2.4, -- rating
            'hotel2.jpg', -- image
            'hotel2' -- gallery
    );

INSERT INTO hotel(name, city, street, description, stars, rating, image, gallery )
    VALUES ('Donly', -- name
            'Donovaly', -- city
            'Lyziarska 15', -- street
            'Accomodation nearby the hill of Switzerland with the best rated spa', -- description
            5, -- stars
            4.4, -- rating
            'hotel3.jpg', -- image
            'hotel3' -- gallery
    );

INSERT INTO hotel(name, city, street, description, stars, rating, image, gallery )
VALUES ('Harrods', -- name
        'London', -- city
        'Oxford street 19', -- street
        'Luxury hotel in the center of London', -- description
        5, -- stars
        4.9, -- rating
        'hotel4.jpg', -- image
        'hotel4' -- gallery
       );

INSERT INTO hotel(name, city, street, description, stars, rating, image, gallery)
VALUES ('Villenza', -- name
        'London', -- city
        'Charlestone 522', -- street
        'Cozy accommodation for every purpose of your stay in London', -- description
        3, -- stars
        4.5, -- rating
        'hotel5.jpg', -- image
        'hotel5' -- gallery
       );

-- Users
    -- ADMIN
    INSERT INTO `guest` (`mail_address`, `first_name`, `last_name`, `birth_date`, `city`, `street`)
        VALUES          ('admin',        'admin',      'admin',     '2020-01-01', 'admin','admin 2020');
    INSERT INTO `user` (`password`, `role`, `guest_mail`)
        VALUES         ('admin',     1,     'admin');

    -- OWNER
    INSERT INTO `guest` (`mail_address`, `first_name`, `last_name`, `birth_date`, `city`, `street`)
        VALUES          ('owner',        'Mark',       'Ownish',    '1999-03-30', 'London', 'Hotel street 42');
    INSERT INTO `user` (`password`, `role`, `guest_mail`)
        VALUES         ('owner',     2     , 'owner');

    -- RECEPTIONIST
    INSERT INTO `guest` (`mail_address`, `first_name`, `last_name`, `birth_date`, `city`, `street`)
        VALUES          ('reception1',   'David',      'Stork',     '2020-02-03', 'Piestany', 'Partizanska 12');
    INSERT INTO `user` (`password`, `role`, `guest_mail`)
        VALUES         ('abcd',     3     , 'reception1');
    INSERT INTO `guest` (`mail_address`, `first_name`, `last_name`, `birth_date`, `city`, `street`)
        VALUES          ('reception2',   'Michal',     'Straka',    '1950-01-01', 'Piestany', 'Lipova 24');
    INSERT INTO `user` (`password`, `role`, `guest_mail`)
        VALUES         ('efgh',     3     , 'reception2');
    INSERT INTO `guest` (`mail_address`, `first_name`, `last_name`, `birth_date`, `city`, `street`)
        VALUES          ('reception3',   'Patrik',     'Vrbovsky',  '1999-01-01', 'Vrbove', 'Teplicka 42');
    INSERT INTO `user` (`password`, `role`, `guest_mail`)
        VALUES         ('ijkl',      3    , 'reception3');

    -- USER
    INSERT INTO `guest` (`mail_address`, `first_name`, `last_name`, `birth_date`, `city`, `street`)
        VALUES          ('mato.otcenas@gmail.com', 'Matej', 'Otcenas', '1990-12-24', 'Piestany', 'Razusova 42');
    INSERT INTO `user` (`password`, `role`, `guest_mail`)
        VALUES         ('ijkl',     4,      'mato.otcenas@gmail.com');

    -- Receptions
    INSERT INTO receptions (guest_mail, hotel_id)
        VALUES ('reception1', 1);
    INSERT INTO receptions (guest_mail, hotel_id)
        VALUES ('reception2', 2);
    INSERT INTO receptions (guest_mail, hotel_id)
        VALUES ('reception3', 3);

-- Rooms
INSERT INTO room (floor_room, hotel_id, type) VALUES (101, 1, 'Business');
INSERT INTO room (floor_room, hotel_id, type) VALUES (102, 1, 'Business');
INSERT INTO room (floor_room, hotel_id, type) VALUES (103, 1, 'Business');

INSERT INTO room_details(hotel_id, type, beds, facility, image, price)
    VALUES (1, -- hotel
            'Business', -- type
            4, -- beds
            'Upper floors accessible by elevator, Linens, Wardrobe or closet, Minibar, Safe, Heating, TV, Telephone, Satellite channels', -- facility
            'room1.jpg', -- image
            100 -- price
    );

INSERT INTO room (floor_room, hotel_id, type) VALUES (201, 1, 'Standard');
INSERT INTO room (floor_room, hotel_id, type) VALUES (202, 1, 'Standard');
INSERT INTO room (floor_room, hotel_id, type) VALUES (203, 1, 'Standard');

INSERT INTO room_details (hotel_id, type, beds, facility, image, price)
    VALUES (1, -- hotel
            'Standard', -- type
            2, -- beds
            'Swimming pool, Non-smoking rooms, Spa, Fitness Center, Facilities for disabled guests, Bar, Very Good Breakfast', -- facility
            'room2.jpg', -- image
            150 -- price
    );

INSERT INTO room (floor_room, hotel_id, type) VALUES (101, 2, 'Business');
INSERT INTO room (floor_room, hotel_id, type) VALUES (102, 2, 'Business');
INSERT INTO room (floor_room, hotel_id, type) VALUES (103, 2, 'Business');

INSERT INTO room_details (hotel_id, type, beds, facility, image, price)
    VALUES (2, -- hotel
            'Business', -- type
            5, -- beds
            'Pool with a view, Air conditioning, Flat-screen TV, Soundproof, Coffee machine, Minibar, Electric kettle', -- facility
            'room3.jpg', -- image
            80 -- price
    );

INSERT INTO room (floor_room, hotel_id, type) VALUES (201, 2, 'Standard');
INSERT INTO room (floor_room, hotel_id, type) VALUES (202, 2, 'Standard');
INSERT INTO room (floor_room, hotel_id, type) VALUES (203, 2, 'Standard');

INSERT INTO room_details(hotel_id, type, beds, facility, image, price)
    VALUES (2, -- hotel
            'Standard', -- type
            3, -- beds
            'Non-smoking rooms, Fitness Center, Wardrobe or closet, Safe', -- facility
            'room4.jpg', -- image
            50 -- price
    );

INSERT INTO room (floor_room, hotel_id, type) VALUES (201, 3, 'Standard');
INSERT INTO room (floor_room, hotel_id, type) VALUES (202, 3, 'Standard');
INSERT INTO room (floor_room, hotel_id, type) VALUES (203, 3, 'Standard');

INSERT INTO room_details (hotel_id, type, beds, facility, image, price)
   VALUES (3, -- hotel
           'Standard', -- type
           2, -- beds
           'Swimming pool, Non-smoking rooms, Spa, Fitness Center, Facilities for disabled guests', -- facility
           'room5.jpg', -- image
           200 -- price
    );

INSERT INTO room (floor_room, hotel_id, type) VALUES (101, 4, 'Standard');
INSERT INTO room (floor_room, hotel_id, type) VALUES (102, 4, 'Standard');
INSERT INTO room (floor_room, hotel_id, type) VALUES (103, 4, 'Standard');

INSERT INTO room_details (hotel_id, type, beds, facility, image, price)
VALUES (4, -- hotel
        'Standard', -- type
        2, -- beds
        'Swimming pool, Non-smoking rooms, Spa, Fitness Center, Facilities for disabled guests', -- facility
        'room8.jpg', -- image
        200 -- price
       );

INSERT INTO room (floor_room, hotel_id, type) VALUES (201, 4, 'Business');
INSERT INTO room (floor_room, hotel_id, type) VALUES (202, 4, 'Business');
INSERT INTO room (floor_room, hotel_id, type) VALUES (203, 4, 'Business');

INSERT INTO room_details (hotel_id, type, beds, facility, image, price)
VALUES (4, -- hotel
        'Bussiness', -- type
        2, -- beds
        'Swimming pool, Non-smoking rooms, Spa, Fitness Center, Facilities for disabled guests', -- facility
        'room6.jpg', -- image
        200 -- price
       );


INSERT INTO room (floor_room, hotel_id, type) VALUES (101, 5, 'Standard');
INSERT INTO room (floor_room, hotel_id, type) VALUES (102, 5, 'Standard');
INSERT INTO room (floor_room, hotel_id, type) VALUES (103, 5, 'Standard');

INSERT INTO room_details (hotel_id, type, beds, facility, image, price)
VALUES (5, -- hotel
        'Standard', -- type
        2, -- beds
        'Non-smoking rooms, Fitness Center, Facilities for disabled guests, Breakfast included', -- facility
        'room7.jpg', -- image
        200 -- price
       );

-- Reservations 
INSERT INTO reservation (from_date, to_date, guest_mail, room_id, hotel_id, price)
    VALUES ('2020-10-20', -- from_date
            '2020-10-30', -- to_date
            'mato.otcenas@gmail.com', -- guest_mail
            101, -- room_id
            1, -- hotel_id
            300 -- price
    );
INSERT INTO reservation (from_date, to_date, guest_mail, room_id, hotel_id, price)
    VALUES ('2020-11-25', -- from_date
            '2020-12-05', -- to_date
            'reception2', -- guest_mail
            101, -- room_id
            1, -- hotel_id
            400 -- price
    );
INSERT INTO reservation (from_date, to_date, guest_mail, room_id, hotel_id, price)
    VALUES ('2020-10-10', -- from_date
            '2020-10-20', -- to_date
            'mato.otcenas@gmail.com', -- guest_mail
            102, -- room_id
            1, -- hotel_id
            200 -- price
    );
