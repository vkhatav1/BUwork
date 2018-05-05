package DataTransferObject;


/**
 * DTO created to store values of different tables together
 */
public class SpecialDTO {

    private String sid;
    private String lastName;
    private String status;
    private String title;
    private String  course_no;
    private String classid;
    private int year;
    private String semester;



    private String dept_code;

    public SpecialDTO(String sid,String dept_code, String lastName, String status, String title, String course_no, String classid, int year, String semester) {
        this.sid = sid;
        this.lastName = lastName;
        this.status = status;
        this.title = title;
        this.course_no = course_no;
        this.classid = classid;
        this.year = year;
        this.semester = semester;
        this.dept_code = dept_code;
    }

    public SpecialDTO(){

    }

    public String getSid() {
        return sid;
    }

    public String getDept_code() {
        return dept_code;
    }

    public void setDept_code(String dept_code) {
        this.dept_code = dept_code;
    }

    public void setSid(String sid) {
        this.sid = sid;
    }

    public String getLastName() {
        return lastName;
    }

    public void setLastName(String lastName) {
        this.lastName = lastName;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getCourse_no() {
        return course_no;
    }

    public void setCourse_no(String course_no) {
        this.course_no = course_no;
    }

    public String getClassid() {
        return classid;
    }

    public void setClassid(String classid) {
        this.classid = classid;
    }

    public int getYear() {
        return year;
    }

    public void setYear(int year) {
        this.year = year;
    }

    public String getSemester() {
        return semester;
    }

    public void setSemester(String semester) {
        this.semester = semester;
    }
}
