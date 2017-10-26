SELECT dia, mes, espaco, posto, avg_montante_pago
FROM 
	(SELECT dia, mes, espaco, posto, AVG(montante_pago) AS avg_montante_pago
	FROM reserva_fact NATURAL JOIN data_dimension NATURAL JOIN localizacao_dimension
	GROUP BY dia, mes, espaco, posto WITH ROLLUP

	UNION

	SELECT dia, mes, espaco, posto, AVG(montante_pago) AS avg_montante_pago
	FROM reserva_fact NATURAL JOIN data_dimension NATURAL JOIN localizacao_dimension
	GROUP BY mes, espaco, posto, dia WITH ROLLUP

	UNION

	SELECT dia, mes, espaco, posto, AVG(montante_pago) AS avg_montante_pago
	FROM reserva_fact NATURAL JOIN data_dimension NATURAL JOIN localizacao_dimension
	GROUP BY espaco, posto, dia, mes WITH ROLLUP

	UNION

	SELECT dia, mes, espaco, posto, AVG(montante_pago) AS avg_montante_pago
	FROM reserva_fact NATURAL JOIN data_dimension NATURAL JOIN localizacao_dimension
	GROUP BY posto, dia, mes, espaco WITH ROLLUP) AS A
ORDER BY dia, mes, espaco, posto;