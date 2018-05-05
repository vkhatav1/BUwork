package DataTransferObject;

/**
 * This maps to Classes table in database
 */
public class ClassesDTO {

    private String classid;
    private String dept_code;
    private String  course_no;
    private int sect_no;
    private int year;
    private String semester;
    private int limit;
    private int class_size;

    public ClassesDTO(String classid, String dept_code, String  course_no, int sect_no, int year, String semester, int limit, int class_size) {
        this.classid = classid;
        this.dept_code = dept_code;
        this.course_no = course_no;
        this.sect_no = sect_no;
        this.year = year;
        this.semester = semester;
        this.limit = limit;
        this.class_size = class_size;
    }

    public ClassesDTO(){

    }

    public String getClassid() {
        return classid;
    }

    public void setClassid(String classid) {
        this.classid = classid;
    }

    public String getDept_code() {
        return dept_code;
    }

    public void setDept_code(String dept_code) {
        this.dept_code = dept_code;
    }

    public String  getCourse_no() {
        return course_no;
    }

    public void setCourse_no(String  course_no) {
        this.course_no = course_no;
    }

    public int getSect_no() {
        return sect_no;
    }

    public void setSect_no(int sect_no) {
        this.sect_no = sect_no;
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

    public int getLimit() {
        return limit;
    }

    public void setLimit(int limit) {
        this.limit = limit;
    }

    public int getClass_size() {
        return class_size;
    }

    public void setClass_size(int class_size) {
        this.class_size = class_size;
    }

}
