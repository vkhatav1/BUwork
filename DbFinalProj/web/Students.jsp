<%--
  Created by IntelliJ IDEA.
  User: saviovincent
  Date: 4/6/18
  Time: 4:47 AM
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<html>
<head>
    <title>Students Details</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>

    <h1 class="text-danger" style="position: relative; left: 40%">Students Details</h1>

<div align="center">
    <table class="table table-striped table-bordered">
        <thead>
        <tr>
            <th scope="col">StudentID</th>
            <th scope="col">FirstName</th>
            <th scope="col">LastName</th>
            <th scope="col">Status</th>
            <th scope="col">GPA</th>
            <th scope="col">Email</th>
        </tr>
        </thead>

        <c:forEach var="student" items="${studentList}">
            <tr scope="row">
                <td><c:out value="${student.sid}" /></td>
                <td><c:out value="${student.firstName}" /></td>
                <td><c:out value="${student.lastName}" /></td>
                <td><c:out value="${student.email}" /></td>
                <td><c:out value="${student.status}" /></td>
                <td><c:out value="${student.gpa}" /></td>

            </tr>
        </c:forEach>
    </table>
</div>
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
</body>
</html>
