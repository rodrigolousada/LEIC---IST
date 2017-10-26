<html>
    <body><h2>Gestao de ofertas</h2>
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


    //ofertas

    	echo("<h3>Edificios</h3>");
        $sql = "SELECT * FROM oferta;";

        $result = $db->query($sql);

        echo("<table border=\"1\"><tbody><tr><td><b>codigo</b></td><td><b>morada</b></td><td><b>data_inicio</b></td><td><b>data_fim</b></td><td><b>tarifa</b></td><td></td></tr><tr>");

        foreach($result as $row)
        {
            echo("<td>{$row['morada']}</td>\n");
			echo("<td>{$row['codigo']}</td>\n");
            echo("<td>{$row['data_inicio']}</td>\n");
            echo("<td>{$row['data_fim']}</td>\n");
            echo("<td>{$row['tarifa']}</td>\n");
            echo("<td><a href=\"removeroferta.php?morada={$row['morada']}&codigo={$row['codigo']}&data_inicio={$row['data_inicio']}\">Remover</a></td>\n");
            echo("</tr>\n");
        }
        echo("<td><a href='criaroferta.php'>inserir nova oferta</a></td>");
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
