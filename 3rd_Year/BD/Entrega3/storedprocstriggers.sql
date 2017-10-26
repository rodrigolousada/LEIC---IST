DROP TRIGGER IF EXISTS ofertas_sobrepostas;
DROP TRIGGER IF EXISTS pagamento_timestamp;

DELIMITER //
DROP TRIGGER IF EXISTS ofertas_sobrepostas;
CREATE TRIGGER ofertas_sobrepostas
BEFORE INSERT ON oferta
FOR EACH ROW
BEGIN
	IF EXISTS	(SELECT * 
				FROM oferta 
				WHERE codigo = NEW.codigo 
					AND morada = NEW.morada 
					AND ((NEW.data_inicio >= data_inicio AND NEW.data_inicio < data_fim)
						OR(NEW.data_fim > data_inicio AND NEW.data_fim <= data_fim)
						OR(NEW.data_inicio <= data_inicio AND NEW.data_fim >= data_fim))
				)
		THEN CALL ERROR_OFERTA;
	END IF;
END //

DELIMITER ;


DELIMITER //

CREATE TRIGGER pagamento_timestamp
BEFORE INSERT ON paga
FOR EACH ROW
BEGIN
	IF EXISTS 	(SELECT *
				FROM estado
				WHERE new.numero = numero AND new.data <= (SELECT MAX(time_stamp) FROM estado WHERE new.numero = numero))
		THEN CALL ERROR_PAYMENT;
	END IF;
END //

DELIMITER ;