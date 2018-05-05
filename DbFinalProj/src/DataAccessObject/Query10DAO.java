package DataAccessObject;

import java.sql.*;
import DataTransferObject.LogsDTO;
import oracle.jdbc.driver.OracleTypes;

import java.util.ArrayList;

public class Query10DAO extends BaseConnection{

    /**
     * Handles trigger to enter the logs in the log table for every activity
     * @return Arraylist of logs table to the controller servlet
     */
    public ArrayList<LogsDTO> listQuery10(){

        ArrayList<LogsDTO> query10ArrayList = new ArrayList<LogsDTO>();

        Connection con = setUpConnection();

        try{
//            String strDeleteStud1 = "delete from students where SID='B779' ";
            String strInsertStud = "insert into students values ('B779', 'Ane171', 'Boroder', 'junior', 3.17, 'berrds@bu.edu') ";

            PreparedStatement stmt = con.prepareStatement(strInsertStud);
//            stmt.executeQuery(strDeleteStud1);
            stmt.executeQuery(strInsertStud);

           // String showLogs = "select * from logs";

            String strDeleteStud = "delete from students where SID='B779' ";
            stmt.executeQuery(strDeleteStud);



            String strInsertEnroll = "insert into enrollments values ('B006','c0004', 'A') ";

            stmt.executeQuery(strInsertEnroll);

            String strDeleteEnroll = "delete from enrollments where  sid = 'B006' and classid = 'c0004' ";

            stmt.executeQuery(strDeleteEnroll);


            CallableStatement cs = con.prepareCall("{call list_all_tables.show_logs(?)}");

            cs.registerOutParameter(1, OracleTypes.CURSOR);

            cs.execute();

            ResultSet result = (ResultSet)cs.getObject(1);

            if(null != result) {
                while (result.next()) {

                    LogsDTO logsDTO = new LogsDTO();

                    logsDTO.setLogId(result.getInt("LOGID"));
                    logsDTO.setWho(result.getString("WHO"));
                    logsDTO.setTime(result.getTimestamp("TIME"));
                    logsDTO.setTable_name(result.getString("TABLE_NAME"));
                    logsDTO.setOperation(result.getString("OPERATION"));
                    logsDTO.setKey_value(result.getString("KEY_VALUE"));

                    query10ArrayList.add(logsDTO);
                }
            }

            stmt.close();
            con.close();
        }
        catch (SQLException ex){
            ex.printStackTrace();
        }


        return query10ArrayList;
    }
}