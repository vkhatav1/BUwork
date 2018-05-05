package DataAccessObject;

import DataTransferObject.CoursesDTO;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;


public class CoursesDAO extends BaseConnection{

    /**
     * courses Data Access Object to access DTO
     * @return Array list to Controller servlet
     */
    public ArrayList<CoursesDTO> listCourses(){

        ArrayList<CoursesDTO> coursesDTOArrayList = new ArrayList<CoursesDTO>();
        Connection con = setUpConnection();

        try {
            Statement stmt= con.createStatement();
            String str = "select * from Courses";
            ResultSet result= stmt.executeQuery(str);
            while(result.next()){
                CoursesDTO coursesDTO = new CoursesDTO();
                coursesDTO.setDept_code(result.getString("DEPT_CODE"));
                coursesDTO.setCourse_no(result.getInt("COURSE_NO"));
                coursesDTO.setTitle(result.getString("TITLE"));

                coursesDTOArrayList.add(coursesDTO);
            }
            stmt.close();
            result.close();
            con.close();
        }
        catch (SQLException ex){
            ex.printStackTrace();
        }

        return coursesDTOArrayList;
    }
}
