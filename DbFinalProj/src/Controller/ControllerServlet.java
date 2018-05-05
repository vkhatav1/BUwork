package Controller;

import DataAccessObject.*;
import DataTransferObject.*;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.RequestDispatcher;
import java.io.IOException;
import java.sql.SQLException;
import java.util.ArrayList;

@WebServlet(name = "ControllerServlet")
/**
 * controller to map views to their respective models
 * MVC architecture followed
 */
public class ControllerServlet extends HttpServlet {


    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        doGet(request, response);
    }

    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

        String queryNo = request.getServletPath();

        try {
            switch (queryNo) {

                case "/query1":
                    executeQuery1(request, response);
                    break;
                case "/query2":
                    executeQuery2(request, response);
                    break;
                case "/query3":
                    executeQuery3(request, response);
                    break;
                case "/query4":
                    executeQuery4(request, response);
                    break;
                case "/query5":
                    executeQuery5(request, response);
                    break;
                case "/query6":
                    executeQuery6(request, response);
                    break;
                case "/query7":
                    executeQuery7(request, response);
                    break;
                case "/query8":
                    executeQuery8(request, response);
                    break;
                case "/query9":
                    executeQuery9(request, response);
                    break;
                case "/query10":
                    executeQuery10(request, response);

                    default:
                        break;
            }
        }
        catch (SQLException ex) {
            throw new ServletException(ex);
        }
    }

    private void executeQuery1(HttpServletRequest request, HttpServletResponse response)
            throws SQLException, IOException, ServletException {

        LogsDAO query1 = new LogsDAO();

        ArrayList<LogsDTO> query1ArrayList = query1.listQuery1();

        request.setAttribute("listQuery1", query1ArrayList);

        RequestDispatcher dispatcher = request.getRequestDispatcher("Views/query1.jsp");

        dispatcher.forward(request, response);
    }

    private void executeQuery2(HttpServletRequest request, HttpServletResponse response)
            throws SQLException, IOException, ServletException {

        // Students
        Query2DAO query2DAO = new Query2DAO();

        ArrayList<StudentsDTO> studentsDTOArrayList = query2DAO.listQuery2Students();

        //Classes
        ArrayList<ClassesDTO> classesDTOArrayList = query2DAO.listQuery2Classes();

        //Courses
        ArrayList<CoursesDTO> coursesDTOArrayList = query2DAO.listQuery2Courses();

        //Prerequisities
        ArrayList<PrerequisitesDTO> prerequisitesDTOArrayList = query2DAO.listQuery2Prerequisites();

        //Enrollments
        ArrayList<EnrollmentsDTO> enrollmentsDTOArrayList = query2DAO.listQuery2Enrollments();

        //Logs
        ArrayList<LogsDTO> logsDTOArrayList = query2DAO.listQuery2Logs();

        request.setAttribute("studentList", studentsDTOArrayList);

        request.setAttribute("classesList",classesDTOArrayList);

        request.setAttribute("coursesList",coursesDTOArrayList);

        request.setAttribute("prerequisitesList",prerequisitesDTOArrayList);

        request.setAttribute("enrollmentsList",enrollmentsDTOArrayList);

        request.setAttribute("logsList",logsDTOArrayList);

        RequestDispatcher dispatcher = request.getRequestDispatcher("Views/Query2.jsp");

        dispatcher.forward(request,response);
    }

    private void executeQuery3(HttpServletRequest request, HttpServletResponse response)
            throws SQLException, IOException, ServletException {
        
        Query3DAO query3DAO = new Query3DAO();

         String fname =  request.getParameter("fname");
        String lname =  request.getParameter("lname");
        String sid =  request.getParameter("sid");
        String status =  request.getParameter("status");
        String gpa =  request.getParameter("gpa");
        String email =  request.getParameter("email");
        
        ArrayList<StudentsDTO> query3DTOArrayList = query3DAO.insert_student_details(fname,lname,sid,status,gpa,email);

        request.setAttribute("studentList", query3DTOArrayList);

        RequestDispatcher dispatcher = request.getRequestDispatcher("Views/Students.jsp");

        dispatcher.forward(request, response);

    }

    private void executeQuery4(HttpServletRequest request, HttpServletResponse response)
            throws SQLException, IOException, ServletException {

        String string;
        RequestDispatcher dispatcher = null;

        Query4DAO query4DAO = new Query4DAO();

        String sid = request.getParameter("sid");

        ArrayList<SpecialDTO> query4DTOArrayList = query4DAO.getSid(sid);

        if(query4DAO.getFlag() == 1){
            string = "The Sid in Invalid..!!";

            request.setAttribute("message", string);
            dispatcher = request.getRequestDispatcher("Views/Error.jsp");
        }

        else if (query4DAO.getFlag() == 2){
            string = "The student has not taken any courses..!!";

            request.setAttribute("message", string);
            dispatcher = request.getRequestDispatcher("Views/Error.jsp");
        }

        else {

            request.setAttribute("searchResult", query4DTOArrayList);
            dispatcher = request.getRequestDispatcher("Views/Query4.jsp");
        }

        dispatcher.forward(request, response);
    }

    private void executeQuery5(HttpServletRequest request, HttpServletResponse response)
            throws SQLException, IOException, ServletException {

    }

    private void executeQuery6(HttpServletRequest request, HttpServletResponse response)
            throws SQLException, IOException, ServletException {
        String string;
        RequestDispatcher dispatcher = null;

        Query6DAO query6DAO = new Query6DAO();

        String classid = request.getParameter("find_student");

        ArrayList<SpecialDTO> query6DTOArrayList = query6DAO.getClassid(classid);

        if(query6DAO.getFlag() == 1){
            string = "The Class Id is Invalid..!!";

            request.setAttribute("message", string);
            dispatcher = request.getRequestDispatcher("Views/Error.jsp");
        }

        else if (query6DAO.getFlag() == 2){
            string = "No student is enrolled in this class!!";

            request.setAttribute("message", string);
            dispatcher = request.getRequestDispatcher("Views/Error.jsp");
        }

        else {
            request.setAttribute("searchResult", query6DTOArrayList);
            dispatcher = request.getRequestDispatcher("Views/Query6.jsp");
        }
        dispatcher.forward(request, response);
    }

    private void executeQuery7(HttpServletRequest request, HttpServletResponse response)
            throws SQLException, IOException, ServletException {

    }

    private void executeQuery8(HttpServletRequest request, HttpServletResponse response)
            throws SQLException, IOException, ServletException {

    }

    private void executeQuery9(HttpServletRequest request, HttpServletResponse response)
            throws SQLException, IOException, ServletException {

        String string;
        RequestDispatcher dispatcher = null;

        Query9DAO query9DAO = new Query9DAO();

        String sid = request.getParameter("delete_student");

        StudentsDTO studentsDTO = new StudentsDTO();

        ArrayList<StudentsDTO>query9DTOArrayList = query9DAO.getSid(sid);

        if(query9DAO.getFlag() == 1){
            string = "The Sid in Invalid..!!";

            request.setAttribute("message", string);
            dispatcher = request.getRequestDispatcher("Views/Error.jsp");
        }

        else if (query9DAO.getFlag() == 2){
            string = "The student has not taken any courses..!!";

            request.setAttribute("message", string);
            dispatcher = request.getRequestDispatcher("Views/Error.jsp");
        }

        else {
            request.setAttribute("studentList", query9DTOArrayList);

            dispatcher = request.getRequestDispatcher("Views/Students.jsp");
        }

        dispatcher.forward(request, response);

    }

    private void executeQuery10(HttpServletRequest request, HttpServletResponse response)
            throws SQLException, IOException, ServletException {

        Query10DAO query10 = new Query10DAO();

        ArrayList<LogsDTO> query10ArrayList = query10.listQuery10();

        request.setAttribute("listQuery10", query10ArrayList);

        RequestDispatcher dispatcher = request.getRequestDispatcher("Views/Query10.jsp");

        dispatcher.forward(request, response);

    }



}
