<html>
    <body>
    <a href="reservas.php">Recuar para gestao de reservas</a>
<?php
    $numero = $_REQUEST['numero'];
    $data = $_REQUEST['data'];
    $metodo = $_REQUEST['metodo'];
    try
    {
        $host = "db.ist.utl.pt";
        $user ="XXXXXXXXXX";
        $password = "XXXXXXXXXX";
        $dbname = $user;
        $db = new PDO("mysql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);


		//insere nas reservas pagas
        $sql = "insert into paga values('$numero', '$data', '$metodo');
				insert into estado values('$numero', '$data', 'Paga');";

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
