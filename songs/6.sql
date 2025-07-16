SELECT s.name FROM songs as s LEFT JOIN artists as a ON s.artist_id=a.id WHERE a.name="Post Malone";
