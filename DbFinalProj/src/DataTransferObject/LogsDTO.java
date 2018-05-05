package DataTransferObject;

import java.util.Date;

/**
 * This maps to Logs table in database
 */
public class LogsDTO {

    private String who;
    private int logId;
    private Date time;
    private String table_name;
    private String operation;
    private String key_value;

    public String getWho() {
        return who;
    }

    public void setWho(String who) {
        this.who = who;
    }

    public int getLogId() {
        return logId;
    }

    public void setLogId(int logId) {
        this.logId = logId;
    }

    public Date getTime() {
        return time;
    }

    public void setTime(Date time) {
        this.time = time;
    }

    public String getTable_name() {
        return table_name;
    }

    public void setTable_name(String table_name) {
        this.table_name = table_name;
    }

    public String getOperation() {
        return operation;
    }

    public void setOperation(String operation) {
        this.operation = operation;
    }

    public String getKey_value() {
        return key_value;
    }

    public void setKey_value(String key_value) {
        this.key_value = key_value;
    }

    public LogsDTO(){

    }

    public LogsDTO(String who, int logId, Date time, String table_name, String operation, String key_value) {
        this.who = who;
        this.logId = logId;
        this.time = time;
        this.table_name = table_name;
        this.operation = operation;
        this.key_value = key_value;
    }

}
