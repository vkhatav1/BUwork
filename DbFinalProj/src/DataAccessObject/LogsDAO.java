package DataAccessObject;

import DataTransferObject.LogsDTO;
import oracle.jdbc.driver.OracleTypes;

import java.sql.*;
import java.util.ArrayList;

public class LogsDAO extends BaseConnection{

    /**
     * Logs Data Access Object used for query 1
     * @return Array list to Controller servlet
     */
    public ArrayList<LogsDTO> listQuery1(){

        ArrayList<LogsDTO> query1ArrayList = new ArrayList<LogsDTO>();

        Connection con = setUpConnection();

        try {

            String str2 = "CREATE SEQUENCE log_sequence START WITH 100 INCREMENT BY 1 MAXVALUE 999 NOCYCLE ";

            PreparedStatement stmt = con.prepareStatement(str2);

            String str3 = "insert into logs values (LOG_SEQUENCE.NEXTVAL, 'savio', sysdate, 'TABLE1', 'INSERT','KEY_VALUE') ";
            stmt.executeQuery(str3);

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

                    query1ArrayList.add(logsDTO);
                }
            }
            stmt.close();
            result.close();
            con.close();

        }catch (SQLException ex){
            ex.printStackTrace();
        }

        return query1ArrayList;
    }
}
