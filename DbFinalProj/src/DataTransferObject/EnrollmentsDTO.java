package DataTransferObject;

/**
 * This maps to Enrollments table in database
 */
public class EnrollmentsDTO {

    private String sid;
    private String classId;
    private String lgrade;

    public EnrollmentsDTO(String sid, String classId, String lgrade) {
        this.sid = sid;
        this.classId = classId;
        this.lgrade = lgrade;
    }
    public EnrollmentsDTO(){

    }

    public String getSid() {
        return sid;
    }

    public void setSid(String sid) {
        this.sid = sid;
    }

    public String getClassId() {
        return classId;
    }

    public void setClassId(String classId) {
        this.classId = classId;
    }

    public String getLgrade() {
        return lgrade;
    }

    public void setLgrade(String lgrade) {
        this.lgrade = lgrade;
    }

}
