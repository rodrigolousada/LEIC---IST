<html>
    <body>
    <a href="reservas.php">Recuar para reservas</a>
<?php
    $numero = $_REQUEST['numero'];
    $morada = $_REQUEST['morada'];
    $codigo = $_REQUEST['codigo'];
    $data_inicio = $_REQUEST['data_inicio'];
    $nif = $_REQUEST['nif'];
	$time_stamp = $_REQUEST['time_stamp'];
    try
    {
        $host = "db.ist.utl.pt";
        $user ="XXXXXXXXXX";
        $password = "XXXXXXXXXX";
        $dbname = $user;
        $db = new PDO("mysql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);


        $sql = "insert into reserva values('$numero');
				insert into aluga values('$morada', '$codigo', '$data_inicio', '$nif', '$numero');
				insert into estado values('$numero', '$time_stamp', 'Pendente');";

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
