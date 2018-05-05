package DataAccessObject;

import DataTransferObject.*;
import com.sun.xml.internal.ws.api.ha.StickyFeature;
import oracle.jdbc.driver.OracleTypes;

import java.sql.*;
import java.util.ArrayList;

public class Query2DAO extends BaseConnection {

    Connection con = setUpConnection();

    /**
     *Query2 Data Access Object to list the students
     * @return Array list to Controller servlet
     */
    public ArrayList<StudentsDTO> listQuery2Students() {

        ArrayList<StudentsDTO> query2ArrayListStudents = new ArrayList<StudentsDTO>();

        try {

            CallableStatement cs = con.prepareCall("{call list_all_tables.show_students(?)}");

            cs.registerOutParameter(1, OracleTypes.CURSOR);

            cs.execute();

            ResultSet result = (ResultSet)cs.getObject(1);

            if(null != result) {
                while (result.next()) {

                    StudentsDTO studentsDTO = new StudentsDTO();

                    studentsDTO.setSid(result.getString("SID"));
                    studentsDTO.setFirstName(result.getString("FirstName"));
                    studentsDTO.setLastName(result.getString("LastName"));
                    studentsDTO.setStatus(result.getString("STATUS"));
                    studentsDTO.setGpa(result.getDouble("GPA"));
                    studentsDTO.setEmail(result.getString("EMAIL"));

                    query2ArrayListStudents.add(studentsDTO);
                }
            }

        } catch (SQLException ex) {
            ex.printStackTrace();
        }
        return query2ArrayListStudents;
    }

    /**
     *Query2 Data Access Object to list the classes table
     * @return Array list to Controller servlet
     */
    public ArrayList<ClassesDTO> listQuery2Classes() {

        ArrayList<ClassesDTO> query2ArrayListClasses = new ArrayList<ClassesDTO>();

        try {

            CallableStatement cs = con.prepareCall("{call list_all_tables.show_classes(?)}");

            cs.registerOutParameter(1, OracleTypes.CURSOR);

            cs.execute();

            ResultSet result = (ResultSet)cs.getObject(1);

            while (result.next()) {

                ClassesDTO classesDTO = new ClassesDTO();

                classesDTO.setClassid(result.getString("ClassID"));
                classesDTO.setClass_size(result.getInt("Class_size"));
                classesDTO.setCourse_no(result.getString("Course_No"));
                classesDTO.setDept_code(result.getString("Dept_Code"));
                classesDTO.setLimit(result.getInt("Limit"));
                classesDTO.setSect_no(result.getInt("Sect_No"));
                classesDTO.setSemester(result.getString("Semester"));
                classesDTO.setYear(result.getInt("Year"));

                query2ArrayListClasses.add(classesDTO);
            }

        } catch (SQLException ex) {
            ex.printStackTrace();
        }
        return query2ArrayListClasses;
    }

    /**
     *Query2 Data Access Object to list courses table
     * @return Array list to Controller servlet
     */
    public ArrayList<CoursesDTO> listQuery2Courses() {

        ArrayList<CoursesDTO> query2ArrayListCourses = new ArrayList<CoursesDTO>();

       // Connection con = setUpConnection();

        try {
            CallableStatement cs = con.prepareCall("{call list_all_tables.show_courses(?)}");

            cs.registerOutParameter(1, OracleTypes.CURSOR);

            cs.execute();

            ResultSet result = (ResultSet)cs.getObject(1);

            while (result.next()) {

                CoursesDTO coursesDTO = new CoursesDTO();

                coursesDTO.setTitle(result.getString("Title"));
                coursesDTO.setDept_code(result.getString("Dept_Code"));
                coursesDTO.setCourse_no(result.getInt("Course_No"));

                query2ArrayListCourses.add(coursesDTO);
            }

        } catch (SQLException ex) {
            ex.printStackTrace();
        }
        return query2ArrayListCourses;
    }

    /**
     *Query2 Data Access Object to list prerequisites table
     * @return Array list to Controller servlet
     */
    public ArrayList<PrerequisitesDTO> listQuery2Prerequisites() {

        ArrayList<PrerequisitesDTO> query2ArrayListPrerequisites = new ArrayList<PrerequisitesDTO>();

       // Connection con = setUpConnection();

        try {
            CallableStatement cs = con.prepareCall("{call list_all_tables.show_prerequisites(?)}");

            cs.registerOutParameter(1, OracleTypes.CURSOR);

            cs.execute();

            ResultSet result = (ResultSet)cs.getObject(1);

            while (result.next()) {

                PrerequisitesDTO prerequisitesDTO = new PrerequisitesDTO();

                prerequisitesDTO.setCourse_no(result.getInt("Course_No"));
                prerequisitesDTO.setDept_code(result.getString("Dept_Code"));
                prerequisitesDTO.setPre_course_no(result.getInt("Pre_Course_No"));
                prerequisitesDTO.setPre_dept_code(result.getString("Pre_Dept_Code"));

                query2ArrayListPrerequisites.add(prerequisitesDTO);
            }

        } catch (SQLException ex) {
            ex.printStackTrace();
        }
        return query2ArrayListPrerequisites;
    }

    /**
     *Query2 Data Access Object to list enrollments table
     * @return Array list to Controller servlet
     */
    public ArrayList<EnrollmentsDTO> listQuery2Enrollments() {

        ArrayList<EnrollmentsDTO> query2ArrayListEnrollments = new ArrayList<EnrollmentsDTO>();

       // Connection con = setUpConnection();

        try {
            CallableStatement cs = con.prepareCall("{call list_all_tables.show_enrollments(?)}");

            cs.registerOutParameter(1, OracleTypes.CURSOR);

            cs.execute();

            ResultSet result = (ResultSet)cs.getObject(1);

            while (result.next()) {

                EnrollmentsDTO enrollmentsDTO = new EnrollmentsDTO();

                enrollmentsDTO.setSid(result.getString("Sid"));
                enrollmentsDTO.setClassId(result.getString("ClassID"));
                enrollmentsDTO.setLgrade(result.getString("Lgrade"));

                query2ArrayListEnrollments.add(enrollmentsDTO);
            }

        } catch (SQLException ex) {
            ex.printStackTrace();
        }
        return query2ArrayListEnrollments;
    }

    /**
     *Query2 Data Access Object to list logs table
     * @return Array list to Controller servlet
     */
    public ArrayList<LogsDTO> listQuery2Logs() {

        ArrayList<LogsDTO> query2ArrayListLogs = new ArrayList<LogsDTO>();

      //  Connection con = setUpConnection();

        try {
            CallableStatement cs = con.prepareCall("{call list_all_tables.show_logs(?)}");

            cs.registerOutParameter(1, OracleTypes.CURSOR);

            cs.execute();

            ResultSet result = (ResultSet)cs.getObject(1);

            while (result.next()) {

                LogsDTO logsDTO = new LogsDTO();

                logsDTO.setLogId(result.getInt("LOGID"));
                logsDTO.setWho(result.getString("WHO"));
                logsDTO.setTime(result.getTimestamp("TIME"));
                logsDTO.setTable_name(result.getString("TABLE_NAME"));
                logsDTO.setOperation(result.getString("OPERATION"));
                logsDTO.setKey_value(result.getString("KEY_VALUE"));

                query2ArrayListLogs.add(logsDTO);
            }

        } catch (SQLException ex) {
            ex.printStackTrace();
        }
        return query2ArrayListLogs;
    }
}