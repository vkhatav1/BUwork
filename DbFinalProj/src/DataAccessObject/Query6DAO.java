package DataAccessObject;

import DataTransferObject.SpecialDTO;
import oracle.jdbc.driver.OracleTypes;

import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;

public class Query6DAO extends BaseConnection{
    int flag = 0;

    /**
     * Accepts classid as parameter to find the student
     * @param classid entered through form
     * @return Arraylist of student details returned to the controller servlet
     */
    public ArrayList<SpecialDTO> getClassid(String classid){

        ArrayList<SpecialDTO> query6DTOArrayList = new ArrayList<>();

        Connection con = setUpConnection();

        try {

            CallableStatement cs = con.prepareCall("{call list_students_in_class.find_students(?,?,?)}");

            cs.setString(1, classid);
            cs.registerOutParameter(2, OracleTypes.INTEGER);
            cs.registerOutParameter(3, OracleTypes.CURSOR);

            cs.execute();

            if(null !=  cs.getObject(2))
                flag = (int)cs.getObject(2);

            ResultSet result = (ResultSet)cs.getObject(3);

            if(null != result){
                while (result.next()) {

                    SpecialDTO specialDTO = new SpecialDTO();

                    specialDTO.setClassid(result.getString("Classid"));

                    specialDTO.setTitle(result.getString("Title"));

                    specialDTO.setSemester(result.getString("Semester"));

                    specialDTO.setYear(result.getInt("Year"));

                    specialDTO.setSid(result.getString("sid"));

                    specialDTO.setLastName(result.getString("lastname"));

                    query6DTOArrayList.add(specialDTO);

                }
            }


            cs.close();
            con.close();
        } catch (SQLException ex) {
            ex.printStackTrace();
        }

        return query6DTOArrayList;
    }

    public int getFlag(){
        return flag;
    }

}