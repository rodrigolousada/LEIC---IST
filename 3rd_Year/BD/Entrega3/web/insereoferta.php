<html>
    <body>
    <a href="ofertas.php">Recuar para gestao de ofertas</a>
<?php
    $morada = $_REQUEST['morada'];
    $codigo = $_REQUEST['codigo'];
    $data_inicio = $_REQUEST['data_inicio'];
    $data_fim = $_REQUEST['data_fim'];
    $tarifa = $_REQUEST['tarifa'];
    try
    {
        $host = "db.ist.utl.pt";
        $user ="XXXXXXXX";
        $password = "XXXXXXXX";
        $dbname = $user;
        $db = new PDO("mysql:host=$host;dbname=$dbname", $user, $password);
        $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);


        $sql = "insert into oferta values('$morada', '$codigo', '$data_inicio', '$data_fim', $tarifa);";

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
