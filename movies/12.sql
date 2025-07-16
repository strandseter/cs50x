SELECT movies.title FROM movies
INNER JOIN stars as stars1 ON stars1.movie_id = movies.id
INNER JOIN stars as stars2 ON stars2.movie_id = movies.id
INNER JOIN people as people1 ON stars1.person_id = people1.id
INNER JOIN people as people2 ON stars2.person_id = people2.id
WHERE people1.name = 'Bradley Cooper'
AND people2.name = 'Jennifer Lawrence';
