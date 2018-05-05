package DataAccessObject;
import DataTransferObject.ClassesDTO;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;


public class ClassesDAO extends BaseConnection{

    /**
     * Classes Data Access Object to access DTO
     * @return Array list to Controller servlet
     */
    public ArrayList<ClassesDTO> listClasses(){

        ArrayList<ClassesDTO> classesDTOArrayList = new ArrayList<ClassesDTO>();

        Connection con = setUpConnection();

        try{

            Statement stmt = con.createStatement();

            String str = "select * from classes";

            ResultSet result = stmt.executeQuery(str);

            if(null != result) {
                while (result.next()) {

                    ClassesDTO classesDTO = new ClassesDTO();

                    classesDTO.setClassid(result.getString("CLASSID"));

                    classesDTO.setDept_code(result.getString("DEPT_CODE"));

                    classesDTO.setCourse_no(result.getString("COURSE_NO"));

                    classesDTO.setSect_no(result.getInt("SECT_NO"));

                    classesDTO.setYear(result.getInt("YEAR"));

                    classesDTO.setSemester(result.getString("SEMESTER"));

                    classesDTO.setLimit(result.getInt("LIMIT"));

                    classesDTO.setClass_size(result.getInt("CLASS_SIZE"));

                    classesDTOArrayList.add(classesDTO);
                }
            }
            stmt.close();
            result.close();
            con.close();
        }
        catch (SQLException ex){
            ex.printStackTrace();
        }


        return classesDTOArrayList;
    }
}