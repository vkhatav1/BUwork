package DataAccessObject;

import DataTransferObject.StudentsDTO;
import oracle.jdbc.driver.OracleTypes;

import java.sql.*;
import java.util.ArrayList;

public class Query3DAO extends BaseConnection {

    /**
     *
     * @param fname first name entered through form
     * @param lname last name entered through form
     * @param sid sid enterered through form
     * @param status status entered through form
     * @param gpa gpa entered through form
     * @param email email entered through form
     * @return Array list returned to the controller servlet
     */
    public ArrayList<StudentsDTO> insert_student_details(String fname, String lname, String sid, String status, String gpa, String email) {

        ArrayList<StudentsDTO> query3DTOArrayList = new ArrayList<StudentsDTO>();

        Connection con = setUpConnection();

        try {

            CallableStatement cs = con.prepareCall("{call add_student.add_student_info(?, ?, ?, ?, ?, ?)}");

            cs.setString(1, sid);
            cs.setString(2, fname);
            cs.setString(3, lname);
            cs.setString(4, status);
            cs.setDouble(5,Double.parseDouble(gpa));
            cs.setString(6, email);

            cs.executeUpdate();


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

                    query3DTOArrayList.add(studentsDTO);
                }
            }
            cs.close();
            result.close();
            con.close();

        } catch (SQLException ex) {
            ex.printStackTrace();
        }

        return query3DTOArrayList;
    }
}