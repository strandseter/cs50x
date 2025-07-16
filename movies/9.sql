SELECT people.name FROM people
INNER JOIN stars ON stars.person_id = people.id
INNER JOIN movies ON movies.id = stars.movie_id
WHERE movies.year = 2004
AND people.birth IS NOT NULL
GROUP BY people.id
ORDER BY people.birth ASC;
