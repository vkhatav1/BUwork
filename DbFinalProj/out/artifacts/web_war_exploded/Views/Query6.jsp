<%--
  Created by IntelliJ IDEA.
  User: saviovincent
  Date: 4/6/18
  Time: 4:47 AM
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="d" %>

<html>
<head>
    <title>Search Result</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>

<h1 class="text-danger" style="position: relative; left: 40%">Search Result</h1>

<div align="center">
    <table class="table table-striped table-bordered">
        <thead>
        <tr>
            <th scope="col">ClassID</th>
            <th scope="col">Title</th>
            <th scope="col">Semester</th>
            <th scope="col">Year</th>
            <th scope="col">sid</th>
            <th scope="col">Lname</th>
        </tr>
        </thead>

        <c:forEach var="student" items="${searchResult}">

            <tr>
                <td><d:out value="${student.classid}" /></td>
                <td><c:out value="${student.title}" /></td>
                <td><c:out value="${student.semester}" /></td>
                <td><c:out value="${student.year}" /></td>
                <td><c:out value="${student.sid}" /></td>
                <td><c:out value="${student.lastName}" /></td>
            </tr>

        </c:forEach>
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
