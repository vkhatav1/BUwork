package DataAccessObject;

import DataTransferObject.ClassesDTO;
import DataTransferObject.CoursesDTO;
import DataTransferObject.StudentsDTO;
import oracle.jdbc.driver.OracleTypes;

import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;

public class Query9DAO extends BaseConnection {
    int flag = 0;

    /**
     * Takes sid as parameter to delete the record
     * @param sid sid entered through form
     * @return Arraylist of students table is returned to the controller servlet
     */
    public ArrayList<StudentsDTO> getSid(String sid){

        ArrayList<StudentsDTO> query9DTOArrayList = new ArrayList<>();

        Connection con = setUpConnection();

        try {

            CallableStatement cs = con.prepareCall("{call del_student.delete_student_info(?,?)}");

            cs.setString(1, sid);
            cs.registerOutParameter(2, OracleTypes.INTEGER);

            cs.execute();

            if(null !=  cs.getObject(2))
                flag = (int)cs.getObject(2);

            cs = con.prepareCall("{call list_all_tables.show_students(?)}");

            cs.registerOutParameter(1, OracleTypes.CURSOR);

            cs.execute();

            ResultSet result = (ResultSet)cs.getObject(1);

            if(null != result) {
                while (result.next()) {

                    StudentsDTO studentsDTO = new StudentsDTO();

                    studentsDTO.setSid(result.getString("SID"));

                    studentsDTO.setFirstName(result.getString("FIRSTNAME"));

                    studentsDTO.setLastName(result.getString("LASTNAME"));

                    studentsDTO.setStatus(result.getString("STATUS"));

                    studentsDTO.setGpa(result.getDouble("GPA"));

                    studentsDTO.setEmail(result.getString("EMAIL"));

                    query9DTOArrayList.add(studentsDTO);
                }
            }
            cs.close();
            result.close();
            con.close();
        } catch (SQLException ex) {
            ex.printStackTrace();
        }

        return query9DTOArrayList;
    }

    public int getFlag(){
        return flag;
    }

}
