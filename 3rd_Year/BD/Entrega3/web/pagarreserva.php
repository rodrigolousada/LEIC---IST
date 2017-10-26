<html>
    <body>
       <h3>Efetuar pagamento sobre a reserva <?=$_REQUEST['numero']?></h3>
        <form action="inserepagamento.php" method="post">
        	<p><input type="hidden" name="numero" value="<?=$_REQUEST['numero']?>"/></p>
            <p>Data de Pagamento: <input type="text" name ="data"/></p>
            <p>Metodo: <input type="text" name="metodo"/></p>
            <p><input type="submit" value="Submit"/></p>
        </form>
    </body>
</html>
