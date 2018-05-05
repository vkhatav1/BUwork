<%--
  Created by IntelliJ IDEA.
  User: khatav
  Date: 4/7/18
  Time: 8:46 PM
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="y" %>

<html>
<head>
    <title>Query10</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
<h1 class="text-danger" style="position: relative; left: 40%">Logs Details</h1>

<div align="center">
    <table class="table table-striped table-bordered">
        <thead>
        <tr>
            <th scope="col">LogID</th>
            <th scope="col">Who</th>
            <th scope="col">Date and Time</th>
            <th scope="col">Table Name</th>
            <th scope="col">Operations</th>
            <th scope="col">Key Value</th>
        </tr>
        </thead>

        <y:forEach var="logs" items="${listQuery10}">
            <tr scope="row">
                <td><y:out value="${logs.logId}" /></td>
                <td><y:out value="${logs.who}" /></td>
                <td><y:out value="${logs.time}" /></td>
                <td><y:out value="${logs.table_name}" /></td>
                <td><y:out value="${logs.operation}" /></td>
                <td><y:out value="${logs.key_value}" /></td>

            </tr>
        </y:forEach>
    </table>
</div>

<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>

<script>

    $(document).ready(function(){
        $('body').css('background-color', 'bisque');
    });

</script>
</body>
</html>
