<%--
  Created by IntelliJ IDEA.
  User: khatav
  Date: 4/8/18
  Time: 1:24 AM
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="x" %>
<html>
<head>
    <title>Classes Details</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
<h1 class="text-danger" style="position: relative; left: 40%">Classes Details</h1>

<div align="center">
    <table class="table table-striped table-bordered">
        <thead>
        <tr>
            <th scope="col">Class</th>
            <th scope="col">Department</th>
            <th scope="col">Course Number</th>
            <th scope="col">Section Number</th>
            <th scope="col">Year</th>
            <th scope="col">Semester</th>
            <th scope="col">Limit</th>
            <th scope="col">Class Size</th>
        </tr>
        </thead>

        <x:forEach var="classes" items="${classesList}">
            <tr scope="row">
                <td><x:out value="${classes.classid}" /></td>
                <td><x:out value="${classes.dept_code}" /></td>
                <td><x:out value="${classes.course_no}" /></td>
                <td><x:out value="${classes.sect_no}" /></td>
                <td><x:out value="${classes.year}" /></td>
                <td><x:out value="${classes.semester}" /></td>
                <td><x:out value="${classes.limit}" /></td>
                <td><x:out value="${classes.class_size}" /></td>

            </tr>
        </x:forEach>
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
