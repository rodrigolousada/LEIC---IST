EXPLAIN SELECT A.nif
		FROM arrenda A
			INNER JOIN fiscaliza F
			ON A.morada = F.morada
			AND A.codigo = F.codigo
		GROUP BY A.nif
		HAVING COUNT(DISTINCT F.id) = 1;
		
EXPLAIN	SELECT DISTINCT P.morada, P.codigo_espaco
		FROM posto P
		WHERE (P.morada, P.codigo_espaco) NOT IN (
			SELECT P.morada, P.codigo_espaco
			FROM posto P
				NATURAL JOIN aluga A
				NATURAL JOIN estado E
			WHERE E.estado = 'aceite'
		);

CREATE INDEX IndexId ON fiscaliza(id) USING BTREE;

CREATE INDEX IndexArrenda ON arrenda(morada, codigo) USING BTREE;
CREATE INDEX IndexFiscaliza ON fiscaliza(morada, codigo) USING BTREE;

CREATE INDEX IndexPosto ON posto(morada, codigo_espaco) USING BTREE;
CREATE INDEX IndexEstado ON estado(estado) USING BTREE;

EXPLAIN SELECT A.nif
		FROM arrenda A
			INNER JOIN fiscaliza F
			ON A.morada = F.morada
			AND A.codigo = F.codigo
		GROUP BY A.nif
		HAVING COUNT(DISTINCT F.id) = 1;
		
EXPLAIN	SELECT DISTINCT P.morada, P.codigo_espaco
		FROM posto P
		WHERE (P.morada, P.codigo_espaco) NOT IN (
			SELECT P.morada, P.codigo_espaco
			FROM posto P
				NATURAL JOIN aluga A
				NATURAL JOIN estado E
			WHERE E.estado = 'aceite'
		);