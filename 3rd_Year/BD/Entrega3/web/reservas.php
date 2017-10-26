<html>
    <body><h2>Criar reservas</h2>
    <a href="index.html">Recuar para o indice</a>
	<p></p>
<?php
    try
    {
        $host = "db.ist.utl.pt";
        $user ="XXXXXXXXXX";
        $password = "XXXXXXXXXX";
        $dbname = $user;

        $db = new PDO("mysql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);


    //reservas

    	echo("<h3>Numero das reservas</h3>");
        $sql = "select * from reserva";

        $result = $db->query($sql);

        echo("<table border=\"1\"><tbody><tr><td><b>numero</b></td></tr>");

        foreach($result as $row)
        {
            echo("<td>{$row['numero']}</td>\n");

            echo("</tr>\n");
        }
        echo("<td><a href='criareserva.php'>inserir novo numero</a></td>");
        echo("</table>\n");


    //reservas sobre ofertas (aluga)
    	echo("<h3>Reservas sobre ofertas</h3>");

    	$sql = "select * from aluga;";

        $result = $db->query($sql);

        echo("<table border=\"1\"><tbody><tr><td><b>numero</b></td><td><b>morada</b></td><td><b>codigo</b></td><td><b>data_inicio</b></td><td><b>nif</b></td><td></td></tr><tr>");

        foreach($result as $row)
        {
        	echo("<td>{$row['numero']}</td>\n");
        	echo("<td>{$row['morada']}</td>\n");
        	echo("<td>{$row['codigo']}</td>\n");
            echo("<td>{$row['data_inicio']}</td>\n");
            echo("<td>{$row['nif']}</td>\n");
            echo("<td><a href=\"pagarreserva.php?numero={$row['numero']}\">Pagar</a></td>\n");
            echo("</tr>\n");
        }
        echo("<td><a href='criareservaoferta.php'>inserir nova reserva</a></td>");
        echo("</table>\n");

    }
    catch (PDOException $e)
    {
        echo("<p>ERROR: {$e->getMessage()}</p>");
    }
?>
    </body>
</html>
