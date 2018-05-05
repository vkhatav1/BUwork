package DataAccessObject;

import DataTransferObject.StudentsDTO;
import oracle.jdbc.driver.OracleTypes;

import java.sql.*;
import java.util.ArrayList;

public class StudentsDAO extends BaseConnection{

    /**
     * Listing out the students table
     * @return Arraylist of students table to the controller servlet
     */
    public ArrayList<StudentsDTO> listStudents(){

        ArrayList<StudentsDTO> studentsDTOArrayList = new ArrayList<StudentsDTO>();

        Connection con = setUpConnection();

        try{

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

                    studentsDTOArrayList.add(studentsDTO);
                }
            }

            result.close();
            con.close();
        }
        catch (SQLException ex){
            ex.printStackTrace();
        }


        return studentsDTOArrayList;
    }
}
