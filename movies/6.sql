SELECT AVG(ratings.rating) from ratings LEFT JOIN movies ON ratings.movie_id = movies.id WHERE movies.year = 2012;
