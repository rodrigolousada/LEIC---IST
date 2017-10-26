-- Queries
-- os espacos com postos nunca alugados
SELECT DISTINCT morada, codigo_espaco AS codigo
FROM posto
WHERE (morada, codigo) NOT IN
    (SELECT morada, codigo
    FROM aluga NATURAL JOIN posto);


-- edificios com numero de reservas superior a media
SELECT morada, S.num_reservas
FROM
	(SELECT edificio.morada, count(aluga.morada) AS num_reservas
	FROM edificio LEFT OUTER JOIN aluga ON edificio.morada = aluga.morada
	GROUP BY edificio.morada) AS S
HAVING S.num_reservas > (SELECT AVG(num_reservas) FROM	(SELECT edificio.morada, count(aluga.morada) AS num_reservas
														FROM edificio LEFT OUTER JOIN aluga ON edificio.morada = aluga.morada
														GROUP BY edificio.morada) AS A);

-- users que foram fiscalizados sempre pelo mesmo fiscal
SELECT nif
FROM
  (SELECT DISTINCT id, nif
   FROM fiscaliza NATURAL JOIN arrenda
   GROUP BY id, nif) AS A
GROUP BY nif
HAVING count(*) = 1;

-- montante total realizado por espaco

SELECT morada, codigo, SUM(montante) AS montante_total
FROM 
	(SELECT morada, codigo, 0 AS montante
	FROM espaco
	UNION
	SELECT morada, codigo_espaco AS codigo, datediff(data_fim, data_inicio) * tarifa AS montante
	FROM posto NATURAL JOIN oferta NATURAL JOIN paga NATURAL JOIN aluga
	WHERE year(data) = 2016
	UNION
	SELECT morada,codigo,datediff(data_fim, data_inicio) * tarifa AS montante
	FROM espaco NATURAL JOIN oferta NATURAL JOIN paga NATURAL JOIN aluga
	WHERE year(data) = 2016) AS A
GROUP BY morada, codigo;

-- espacos de trabalho cujos postos nele contidos foram todos alugados
SELECT DISTINCT morada, codigo_espaco AS codigo
FROM posto
WHERE (morada, codigo_espaco) NOT IN
	(SELECT morada, codigo_espaco
	FROM posto
	WHERE (morada, codigo) NOT IN
		(SELECT morada, codigo
		FROM estado NATURAL JOIN aluga NATURAL JOIN posto
		WHERE estado = 'Aceite'));