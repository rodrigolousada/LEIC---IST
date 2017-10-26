<html>
    <body>
    <a href="reservas.php">Recuar reservas</a>
<?php
    $numero = $_REQUEST['numero'];
    try
    {
        $host = "db.ist.utl.pt";
        $user ="XXXXXXXXXX";
        $password = "XXXXXXXXXX";
        $dbname = $user;
        $db = new PDO("mysql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);


        $sql = "insert into reserva values('$numero');";

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
