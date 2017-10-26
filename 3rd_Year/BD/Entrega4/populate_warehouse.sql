DROP PROCEDURE IF EXISTS load_data_time;
									
delimiter //
CREATE PROCEDURE load_data_time()
BEGIN
	DECLARE v_date DATE;
	DECLARE v_time DATETIME;
	DECLARE v_semestre INT;
	SET v_date = '2016-01-01';
	SET v_time = '2016-01-01 00:00:00';
	WHILE v_date < '2018-01-01' DO
		IF (MONTH(v_date) < 7) 
			THEN SET v_semestre = 1;
			ELSE SET v_semestre = 2;
		END IF;
		
		INSERT INTO data_dimension(
			data_id,
			ano,
			semestre,
			mes,
			semana,
			dia
		) VALUES (
			v_date,
			YEAR(v_date),
			v_semestre,
			MONTH(v_date),
			WEEK(v_date),
			DAY(v_date)
		);
		SET v_date = DATE_ADD(v_date, INTERVAL 1 DAY);
	END WHILE;
	
	WHILE v_time < '2016-01-02 00:00:00' DO
		INSERT INTO tempo_dimension(
			tempo_id,
			hora,
			minuto
		) VALUES (
			DATE_FORMAT(v_time, '%H:%i'),
			HOUR(v_time),
			MINUTE(v_time)
		);
		SET v_time = DATE_ADD(v_time, INTERVAL 1 MINUTE);
	END WHILE;
END//
delimiter ;

CALL load_data_time();

INSERT INTO user_dimension 	SELECT * 
							FROM user;
							
INSERT INTO localizacao_dimension 	SELECT concat(morada, '->', codigo) as localizacao_id, morada AS edificio, codigo_espaco AS espaco, codigo AS posto 
									FROM posto 
									UNION 
									SELECT concat(morada, '->', codigo) as localizacao_id , morada AS edificio, codigo AS espaco, ' ' AS posto 
									FROM espaco;

INSERT INTO reserva_fact		SELECT 	numero,
										nif, 
										concat(morada, '->', codigo) AS localizacao_id, 
										DATE_FORMAT(data, '%H:%i') AS tempo_id, 
										DATE(data) AS data_id, 
										DATEDIFF(data_fim, data_inicio) AS duracao_dias, 
										DATEDIFF(data_fim, data_inicio)*tarifa AS montante_pago,
										tarifa
								FROM aluga NATURAL JOIN paga NATURAL JOIN oferta;