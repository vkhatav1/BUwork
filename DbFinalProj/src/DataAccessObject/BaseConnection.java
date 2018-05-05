package DataAccessObject;

import oracle.jdbc.pool.OracleDataSource;
import java.sql.Connection;
import java.sql.SQLException;

/**
 * For connecting to Oracle Database
 */
public class BaseConnection {

    private String username;
    private String password;
    private String url;

    public String getUrl() {
        return url;
    }

    public void setUrl(String url) {
        this.url = url;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public BaseConnection() {
        this.username = "vkhatav1";
        this.password = "bmwGT360";
        this.url = "jdbc:oracle:thin:@castor.cc.binghamton.edu:1521:ACAD111";
    }

    public Connection setUpConnection(){


        Connection con = null;

        try {

            OracleDataSource ds = new oracle.jdbc.pool.OracleDataSource();

            ds.setURL(url);
            con = ds.getConnection(getUsername(), getPassword());
        }
        catch (SQLException ex){

            ex.printStackTrace();
        }
        catch (Exception ex){
            ex.printStackTrace();
        }

        return con;
    }


}
