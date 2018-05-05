package DataTransferObject;

/**
 * This maps to Courses table in database
 */
public class CoursesDTO {

    private String dept_code;
    private String title;
    private int course_no;

    public CoursesDTO(String dept_code, String title, int course_no) {
        this.dept_code = dept_code;
        this.title = title;
        this.course_no = course_no;
    }
    public CoursesDTO(){

    }

    public String getDept_code() {
        return dept_code;
    }

    public void setDept_code(String dept_code) {
        this.dept_code = dept_code;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public int getCourse_no() {
        return course_no;
    }

    public void setCourse_no(int course_no) {
        this.course_no = course_no;
    }

}
