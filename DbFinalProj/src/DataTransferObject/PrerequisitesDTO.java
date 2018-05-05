package DataTransferObject;

/**
 * This maps to Prerequisites table in database
 */
public class PrerequisitesDTO {

    private String dept_code;
    private int course_no;
    private String pre_dept_code;
    private int pre_course_no;

    public PrerequisitesDTO(String dept_code, int course_no, String pre_dept_code, int pre_course_no) {
        this.dept_code = dept_code;
        this.course_no = course_no;
        this.pre_dept_code = pre_dept_code;
        this.pre_course_no = pre_course_no;
    }
    public PrerequisitesDTO(){

    }

    public String getDept_code() {
        return dept_code;
    }

    public void setDept_code(String dept_code) {
        this.dept_code = dept_code;
    }

    public int getCourse_no() {
        return course_no;
    }

    public void setCourse_no(int course_no) {
        this.course_no = course_no;
    }

    public String getPre_dept_code() {
        return pre_dept_code;
    }

    public void setPre_dept_code(String pre_dept_code) {
        this.pre_dept_code = pre_dept_code;
    }

    public int getPre_course_no() {
        return pre_course_no;
    }

    public void setPre_course_no(int pre_course_no) {
        this.pre_course_no = pre_course_no;
    }



}
