<html>
    <body><h2>Montante total realizado em <?=$_REQUEST['morada']?></h2>
    <a href="locais.php">Recuar para gestao de locais</a>
	<p></p>
<?php
	$morada = $_REQUEST['morada'];

    try
    {
        $host = "db.ist.utl.pt";
        $user ="XXXXXXXX";
        $password = "XXXXXXX";
        $dbname = $user;

        $db = new PDO("mysql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);




    //montante total realizado por cada espaco
    	echo("<h3>Espacos de $morada</h3>");

    	$sql = "SELECT codigo, SUM(montante) AS montante_total
				FROM
					(SELECT morada, codigo, 0 AS montante
					FROM espaco
					UNION
					SELECT morada, codigo_espaco AS codigo, datediff(data_fim, data_inicio) * tarifa AS montante
					FROM posto NATURAL JOIN oferta NATURAL JOIN paga NATURAL JOIN aluga
					UNION
					SELECT morada,codigo,datediff(data_fim, data_inicio) * tarifa AS montante
					FROM espaco NATURAL JOIN oferta NATURAL JOIN paga NATURAL JOIN aluga) AS A
				WHERE morada = '$morada'
				GROUP BY morada, codigo;";

        $result = $db->query($sql);

        echo("<table border=\"1\"><tbody><tr><td><b>codigo</b></td><td><b>montante_total</b></td<td></td></tr><tr>");

        foreach($result as $row)
        {
            echo("<td>{$row['codigo']}</td>\n");
            echo("<td>{$row['montante_total']}</td>\n");
            echo("</tr>\n");
        }

        echo("</table>\n");

        $db = null;
    }
    catch (PDOException $e)
    {
        echo("<p>ERROR: {$e->getMessage()}</p>");
    }
?>
    </body>
</html>
