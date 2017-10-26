<html>
    <body>
    <a href="locais.php">Recuar para gestao de locais</a>
<?php
	$morada = $_REQUEST['morada'];
    $codigo = $_REQUEST['codigo'];
    try
    {
        $host = "db.ist.utl.pt";
        $user ="XXXXXXXXXX";
        $password = "XXXXXXXXXX";
        $dbname = $user;
        $db = new PDO("mysql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);


        $sql = "delete from posto WHERE morada = '$morada' AND codigo = '$codigo';
				delete from alugavel WHERE morada = '$morada' AND codigo = '$codigo';";

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
