<%-- Created by IntelliJ IDEA. --%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<!DOCTYPE html>
<html>
  <head>
    <title>Database Student Registration Project</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
  </head>

  <body>

  <div style="position: relative;left: 38%;top: 40px;">


    <h1 style="position:  relative; left: 500px; width: 525px;">Welcome to Database Project 2</h1>

  <p style="position: relative; top: 40px; width: 400px; left: inherit;"> Developers: Vikram Khatavkar and Savio Vincent</p>

  <div style="position:  relative; top: 50px; left: 250px;">

  <form method="post">
    <button type="submit" formaction="query1" class="btn btn-primary" style="margin-bottom: 15px;">QUERY 1</button>
  </form>

    <form method="post">
  <button type="submit" formaction="query2" class="btn btn-success" style="margin-bottom: 15px;">QUERY 2</button>
    </form>

    <form method="post">

      <div style="position: relative;left: 250px;">

        <div class="input-group">
          <span class="input-group-addon" style="width: 90px;">Sid</span>
          <input id="sid" type="text" class="form-control" name="sid" placeholder="Enter Sid" required>
        </div>

        <div class="input-group" >
          <span class="input-group-addon"style="width: 90px;">FirstName</span>
          <input id="fname" type="text" class="form-control" name="fname" placeholder="Enter first name" required>
        </div>

        <div class="input-group">
          <span class="input-group-addon" style="width: 90px;">LastName</span>
          <input id="lname" type="text" class="form-control" name="lname" placeholder="Enter last name" required>
        </div>

        <div class="input-group" >
          <span class="input-group-addon" style="width: 90px;">Status</span>
          <input id="status" type="text" class="form-control" name="status" placeholder="Enter Status" required>
        </div>

        <div class="input-group" >
          <span class="input-group-addon" style="width: 90px;">Gpa</span>
          <input id="gpa" type="text" class="form-control" name="gpa" placeholder="Enter gpa" required>
        </div>

        <div class="input-group" >
          <span class="input-group-addon" style="width: 90px;">Email</span>
          <input id="email" type="text" class="form-control" name="email" placeholder="Enter email" required>
        </div>

      </div>

      <button type="submit" formaction="query3" class="btn btn-info" style="position: relative; top: -125px;">QUERY 3</button>
    </form>

    <form method="post">

      <div style="position: relative;left: 250px;">

        <div class="input-group">
          <span class="input-group-addon" style="width: 90px;">Sid</span>
          <input id="find_sid" type="text" class="form-control" name="sid" placeholder="Enter Sid" required>
        </div>

      </div>

      <button type="submit" formaction="query4" class="btn btn-warning" style="position: relative; top: -35px;">QUERY 4</button>
    </form>

    <form method="post">
  <button type="submit" formaction="query5" class="btn btn-danger" style="margin-bottom: 15px;">QUERY 5</button>
    </form>

    <form method="post">

      <div style="position: relative;left: 250px;">

        <div class="input-group">
          <span class="input-group-addon" style="width: 90px;">classid</span>
          <input id="find_student" type="text" class="form-control" name="find_student" placeholder="Enter Class ID" required>
        </div>

      </div>
  <button type="submit" formaction="query6" class="btn btn-primary" style="position: relative; top: -35px;">QUERY 6</button>
    </form>

    <form method="post">
  <button type="submit" formaction="query7" class="btn btn-success" style="margin-bottom: 15px;">QUERY 7</button>
    </form>

    <form method="post">
  <button type="submit" formaction="query8" class="btn btn-info" style="margin-bottom: 15px;">QUERY 8</button>
    </form>

    <form method="post">

      <div style="position: relative;left: 250px; top: 35px;">

        <div class="input-group">
          <span class="input-group-addon" style="width: 90px;">SID</span>
          <input id="delete_student" type="text" class="form-control" name="delete_student" placeholder="Enter SID" required>
        </div>

      </div>
  <button type="submit" formaction="query9" class="btn btn-warning" style="margin-bottom: 15px;">QUERY 9</button>
    </form>

    <form method="post">
  <button type="submit" formaction="query10" class="btn btn-danger" style="margin-bottom: 15px;">QUERY 10</button>
    </form>

  </div>

  <script>

      $(document).ready(function(){
          $('body').css('background-color', 'bisque');
      });

  </script>
  </body>
</html>