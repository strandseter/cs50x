SELECT people.name from people
INNER JOIN directors ON directors.person_id = people.id
INNER JOIN movies ON directors.movie_id = movies.id
INNER JOIN ratings ON ratings.movie_id = movies.id
WHERE ratings.rating >= 9.0
GROUP BY people.id;
