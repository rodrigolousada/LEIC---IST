<html>
    <body><h2>Gestao de locais</h2>
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


    //edificios

    	echo("<h3>Edificios</h3>");
        $sql = "SELECT * FROM edificio;";

        $result = $db->query($sql);

        echo("<table border=\"1\"><tbody><tr><td><b>morada</b></td><td></td></tr><tr>");

        foreach($result as $row)
        {
            echo("<td>{$row['morada']}</td>\n");
			echo("<td><a href=\"consultartotal.php?morada={$row['morada']}\">Consultar total</a></td>\n");
            echo("<td><a href=\"removeredificio.php?morada={$row['morada']}\">Remover</a></td>\n");
            echo("</tr>\n");
        }
        echo("<td><a href='criaredificio.php'>inserir novo edificio</a></td>");
        echo("</table>\n");


    //espacos
    	echo("<h3>Espacos</h3>");

    	$sql = "SELECT * FROM espaco;";

        $result = $db->query($sql);

        echo("<table border=\"1\"><tbody><tr><td><b>codigo</b></td><td><b>morada</b></td><td></td></tr><tr>");

        foreach($result as $row)
        {
        	echo("<td>{$row['codigo']}</td>\n");
            echo("<td>{$row['morada']}</td>\n");
            echo("<td><a href=\"removerespaco.php?codigo={$row['codigo']}&morada={$row['morada']}\">Remover</a></td>\n");
            echo("</tr>\n");
        }
        echo("<td><a href='criarespaco.php'>inserir novo espaco</a></td>");
        echo("</table>\n");


    	//postos
    	echo("<h3>Posto</h3>");

    	$sql = "SELECT * FROM posto;";

        $result = $db->query($sql);

        echo("<table border=\"1\">\n");
		echo("<tr><td><b>codigo</b></td><td><b>morada</b></td><td><b>codigo_espaco</td><td></td></tr><tr>");

        foreach($result as $row)
        {
			echo("<td>{$row['morada']}</td>\n");
        	echo("<td>{$row['codigo']}</td>\n");
			echo("<td>{$row['codigo_espaco']}</td>\n");
            echo("<td><a href=\"removerposto.php?codigo={$row['codigo']}&morada={$row['morada']}\">Remover</a></td>\n");
            echo("</tr>\n");
        }
        echo("<td><a href='criarposto.php'>inserir novo posto</a></td>");
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
