package DataAccessObject;
import DataTransferObject.SpecialDTO;
import oracle.jdbc.driver.OracleTypes;

import java.sql.*;
import java.util.ArrayList;

public class Query4DAO extends BaseConnection{

    int flag = 0;

    /**
     * Method takes sid as parameter to find the student
     * @param sid entered through form
     * @return arraylist returned to the controller servlet
     */
    public ArrayList<SpecialDTO> getSid(String sid) {

        Connection con = setUpConnection();

        ArrayList<SpecialDTO> query4DTOArrayList = new ArrayList<>();

        try {

            CallableStatement cs = con.prepareCall("{call find_student_details.find_student(?,?,?)}");

            cs.setString(1, sid);
            cs.registerOutParameter(2, OracleTypes.INTEGER);
            cs.registerOutParameter(3, OracleTypes.CURSOR);

            cs.execute();

            if(null !=  cs.getObject(2))
                flag = (int)cs.getObject(2);

            ResultSet result = (ResultSet)cs.getObject(3);

            if(null != result){
                while (result.next()) {

                    SpecialDTO specialDTO = new SpecialDTO();

                    specialDTO.setSid(result.getString("sid"));

                    specialDTO.setLastName(result.getString("lastname"));

                    specialDTO.setStatus(result.getString("status"));

                    specialDTO.setCourse_no(result.getString("course_no"));

                    specialDTO.setTitle(result.getString("title"));

                    specialDTO.setClassid(result.getString("classid"));

                    specialDTO.setYear(result.getInt("year"));

                    specialDTO.setSemester(result.getString("semester"));

                    query4DTOArrayList.add(specialDTO);

                }
            }


            cs.close();
            con.close();

        } catch (SQLException ex) {
            ex.printStackTrace();
        }

        return query4DTOArrayList;
    }

    public int getFlag(){
        return flag;
    }

}