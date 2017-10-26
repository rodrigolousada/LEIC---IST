<html>
    <body>
    <a href="ofertas.php">Recuar para gestao de ofertas</a>
<?php
	$codigo = $_REQUEST['codigo'];
    $morada = $_REQUEST['morada'];
    $data_inicio = $_REQUEST['data_inicio'];
    try
    {
        $host = "db.ist.utl.pt";
        $user ="XXXXXXXXXX";
        $password = "XXXXXXXXXX";
        $dbname = $user;
        $db = new PDO("mysql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);


        $sql = "delete from oferta WHERE codigo = '$codigo' AND morada = '$morada' AND data_inicio = '$data_inicio';";

        echo("<p>$sql</p>");

		$db->query("start transaction;");

        $db->query($sql);

        $db->query("commit;");

        $db = null;
    }
    catch (PDOException $e)
    {
        $db->query("rollback;");
        echo("<p>ERROR: {$e->getMessage()}</p>");
    }
?>
    </body>
</html>
