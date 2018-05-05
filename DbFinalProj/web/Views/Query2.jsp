<%--
  Created by IntelliJ IDEA.
  User: khatav
  Date: 4/8/18
  Time: 7:58 AM
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="i" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="j" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="k" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="l" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="m" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="n" %>

<html>
<head>
    <title>Query2</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
<h1 class="text-danger" style="position: relative; left: 40%">Students Details</h1>

<div align="center">
    <table class="table table-striped table-bordered">
        <thead>
        <tr>
            <th scope="col">SID</th>
            <th scope="col">FirstName</th>
            <th scope="col">LastName</th>
            <th scope="col">Status</th>
            <th scope="col">GPA</th>
            <th scope="col">Email</th>
        </tr>
        </thead>

        <i:forEach var="student" items="${studentList}">
            <tr scope="row">
                <td><i:out value="${student.sid}" /></td>
                <td><i:out value="${student.firstName}" /></td>
                <td><i:out value="${student.lastName}" /></td>
                <td><i:out value="${student.status}" /></td>
                <td><i:out value="${student.gpa}" /></td>
                <td><i:out value="${student.email}" /></td>

            </tr>
        </i:forEach>
    </table>
</div>

<h1 class="text-danger" style="position: relative; left: 40%">Classes Details</h1>

<div align="center">
    <table class="table table-striped table-bordered">
        <thead>
        <tr>
            <th scope="col">ClassID</th>
            <th scope="col">Class_size</th>
            <th scope="col">Course_No</th>
            <th scope="col">Dept_Code</th>
            <th scope="col">Limit</th>
            <th scope="col">Sect_No</th>
            <th scope="col">Semester</th>
            <th scope="col">Year</th>
        </tr>
        </thead>

        <j:forEach var="classes" items="${classesList}">
            <tr scope="row">
                <td><j:out value="${classes.classid}" /></td>
                <td><j:out value="${classes.class_size}" /></td>
                <td><j:out value="${classes.course_no}" /></td>
                <td><j:out value="${classes.dept_code}" /></td>
                <td><j:out value="${classes.limit}" /></td>
                <td><j:out value="${classes.sect_no}" /></td>
                <td><j:out value="${classes.semester}" /></td>
                <td><j:out value="${classes.year}" /></td>

            </tr>
        </j:forEach>
    </table>
</div>

<h1 class="text-danger" style="position: relative; left: 40%">Courses Details</h1>

<div align="center">
    <table class="table table-striped table-bordered">
        <thead>
        <tr>
            <th scope="col">Title</th>
            <th scope="col">Dept_Code</th>
            <th scope="col">Course_No</th>

        </tr>
        </thead>

        <k:forEach var="courses" items="${coursesList}">
            <tr scope="row">
                <td><k:out value="${courses.title}" /></td>
                <td><k:out value="${courses.dept_code}" /></td>
                <td><k:out value="${courses.course_no}" /></td>

            </tr>
        </k:forEach>
    </table>
</div>

<h1 class="text-danger" style="position: relative; left: 40%">Prerequisites Details</h1>

<div align="center">
    <table class="table table-striped table-bordered">
        <thead>
        <tr>
            <th scope="col">Course_No</th>
            <th scope="col">Dept_Code</th>
            <th scope="col">Pre_Course_No</th>
            <th scope="col">Pre_Dept_Code</th>

        </tr>
        </thead>

        <l:forEach var="prerequisite" items="${prerequisitesList}">
            <tr scope="row">
                <td><l:out value="${prerequisite.course_no}" /></td>
                <td><l:out value="${prerequisite.dept_code}" /></td>
                <td><l:out value="${prerequisite.pre_course_no}" /></td>
                <td><l:out value="${prerequisite.pre_dept_code}" /></td>

            </tr>
        </l:forEach>
    </table>
</div>

<h1 class="text-danger" style="position: relative; left: 40%">Enrollments Details</h1>

<div align="center">
    <table class="table table-striped table-bordered">
        <thead>
        <tr>
            <th scope="col">Sid</th>
            <th scope="col">ClassID</th>
            <th scope="col">Lgrade</th>

        </tr>
        </thead>

        <m:forEach var="enrollments" items="${enrollmentsList}">
            <tr scope="row">
                <td><m:out value="${enrollments.sid}" /></td>
                <td><m:out value="${enrollments.classId}" /></td>
                <td><m:out value="${enrollments.lgrade}" /></td>

            </tr>
        </m:forEach>
    </table>
</div>

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

        <n:forEach var="logs" items="${logsList}">
            <tr scope="row">
                <td><n:out value="${logs.logId}" /></td>
                <td><n:out value="${logs.who}" /></td>
                <td><n:out value="${logs.time}" /></td>
                <td><n:out value="${logs.table_name}" /></td>
                <td><n:out value="${logs.operation}" /></td>
                <td><n:out value="${logs.key_value}" /></td>

            </tr>
        </n:forEach>
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
