-- Keep a log of any SQL queries you execute as you solve the mystery.

.tables
.schema

select * from crime_scene_reports where year=2023 and month=7 and day=28;
--------------------------------------------------------------------------------------------------------------------+
| 295 | 2023 | 7     | 28  | Humphrey Street | Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery. |
+-----+------+-------+-----+-----------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

select * from interviews where year=2023 and month=7 and day=28 and transcript LIKE '%bakery%';
---------------------------------------------------+
| id  |  name   | year | month | day | transcript                                                                                                                                                      |
+-----+---------+------+-------+-----+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 161 | Ruth    | 2023 | 7     | 28  | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
| 162 | Eugene  | 2023 | 7     | 28  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
| 163 | Raymond | 2023 | 7     | 28  | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |
+-----+---------+------+-------+-----+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+



select bsl.id, p.name, p.passport_number, p.license_plate
from bakery_security_logs as bsl
join people as p on p.license_plate=bsl.license_plate
where bsl.year=2023
and bsl.month=7
and bsl.day=28
and bsl.activity='exit'
and bsl.hour=10
and bsl.minute between 5 and 25;
// The thief got into a car within 10 minutes of the theft
// Exits of vehicles with drivers:
+-----+---------+-----------------+---------------+
| id  |  name   | passport_number | license_plate |
+-----+---------+-----------------+---------------+
| 260 | Vanessa | 2963008352      | 5P2BI95       |
| 261 | Bruce   | 5773159633      | 94KL13X       |
| 262 | Barry   | 7526138472      | 6P58WS2       |
| 263 | Luca    | 8496433585      | 4328GD8       |
| 264 | Sofia   | 1695452385      | G412CB7       |
| 265 | Iman    | 7049073643      | L93JTIZ       |
| 266 | Diana   | 3592750733      | 322W7JE       |
| 267 | Kelsey  | 8294398571      | 0NTHK55       |
+-----+---------+-----------------+---------------+

select at.id, at.amount, p.name from atm_transactions as at
join bank_accounts as ba on ba.account_number=at.account_number
join people as p on p.id=ba.person_id
where at.year=2023
and at.month=7
and at.day=28
and at.atm_location='Leggett Street'
and at.transaction_type='withdraw';
// Someone a witness recognised (maybe the thief?) was widthdrawing money the morning of the theft
+-----+--------+---------+
| id  | amount |  name   |
+-----+--------+---------+
| 267 | 50     | Bruce   |
| 336 | 35     | Diana   |
| 269 | 80     | Brooke  |
| 264 | 20     | Kenny   |
| 288 | 20     | Iman    |
| 246 | 48     | Luca    |
| 266 | 60     | Taylor  |
| 313 | 30     | Benista |
+-----+--------+---------+

select pc.caller, pc.receiver, p1.name as caller_person, p2.name as receiver_person
from phone_calls as pc join people as p1 on p1.phone_number=pc.caller
join people as p2 on p2.phone_number=pc.receiver
where pc.year=2023
and pc.month=7
and pc.day=28
and pc.duration < 60;
// The thief called someone for under a minute asking to book flight tickets after the theft
+----------------+----------------+---------------+-----------------+
|     caller     |    receiver    | caller_person | receiver_person |
+----------------+----------------+---------------+-----------------+
| (130) 555-0289 | (996) 555-8899 | Sofia         | Jack            |
| (499) 555-9472 | (892) 555-8872 | Kelsey        | Larry           |
| (367) 555-5533 | (375) 555-8161 | Bruce         | Robin           |
| (499) 555-9472 | (717) 555-1342 | Kelsey        | Melissa         |
| (286) 555-6063 | (676) 555-6554 | Taylor        | James           |
| (770) 555-1861 | (725) 555-3243 | Diana         | Philip          |
| (031) 555-6622 | (910) 555-3251 | Carina        | Jacqueline      |
| (826) 555-1652 | (066) 555-9701 | Kenny         | Doris           |
| (338) 555-6650 | (704) 555-2131 | Benista       | Anna            |
+----------------+----------------+---------------+-----------------+

select * from flights where year=2023 and month=7 and day=29 order by hour, minute limit 1;
+----+-------------------+------------------------+------+-------+-----+------+--------+
| id | origin_airport_id | destination_airport_id | year | month | day | hour | minute |
+----+-------------------+------------------------+------+-------+-----+------+--------+
| 36 | 8                 | 4                      | 2023 | 7     | 29  | 8    | 20     |
+----+-------------------+------------------------+------+-------+-----+------+--------+

select pa.flight_id, p.phone_number, p.passport_number, p.license_plate
from passengers as pa
join people as p on p.passport_number=pa.passport_number
where pa.flight_id=36;
+-----------+----------------+-----------------+---------------+
| flight_id |  phone_number  | passport_number | license_plate |
+-----------+----------------+-----------------+---------------+
| 36        | (066) 555-9701 | 7214083635      | M51FA04       |
| 36        | (130) 555-0289 | 1695452385      | G412CB7       |
| 36        | (367) 555-5533 | 5773159633      | 94KL13X       |
| 36        | (328) 555-1152 | 1540955065      | 130LD9Z       |
| 36        | (499) 555-9472 | 8294398571      | 0NTHK55       |
| 36        | (286) 555-6063 | 1988161715      | 1106N58       |
| 36        | (826) 555-1652 | 9878712108      | 30G67EN       |
| 36        | (389) 555-5198 | 8496433585      | 4328GD8       |
+-----------+----------------+-----------------+---------------+

select * from airports where id=4;
+----+--------------+-------------------+---------------+
| id | abbreviation |     full_name     |     city      |
+----+--------------+-------------------+---------------+
| 4  | LGA          | LaGuardia Airport | New York City |
+----+--------------+-------------------+---------------+

